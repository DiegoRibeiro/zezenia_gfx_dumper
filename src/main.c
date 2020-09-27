#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "d_bitmap_v4.h"

int main()
{
	FILE *fp = NULL,		// file pointer for the zezena.gfx
		*fb = NULL;			// file pointer for each bmp created
	char name[100];			// buffer to store the filename of each bmp
	long offset = 0;		// offset in decimal for debugging purposes
	uint32_t sprites;		// number of sprites in the file
	uint16_t width;			// width of sprite
	uint16_t height;		// height of sprite
	uint32_t dataLength;	// length of data for a specific sprite
	D_PIXEL_A p;			// struct that holds the pixel colored data
	uint8_t v1, v2, v3, v4; // aux variable to hold the pixel data, i create this because you have to check for 0x1 on the first pixel data to add transparent pixel
	uint32_t total,			// total bytes that a sprite have the formula is width * height * 4. (rgba)
		processed;			// the sprite data doesn't always have the full data,
							// if the left over of the sprite is transparent you have to complete it based on total - processed

	// opening the gfx in binary mode
	fp = fopen("zezenia.gfx", "rb");

	// checking for errors opening the file
	if (fp == NULL)
	{
		printf("Place the zezenia.gfx in the same folder as the executable!");
		return 1;
	}

	// skipping first 5 bytes, could be a signature or other thing
	fseek(fp, 5, SEEK_SET);
	offset += 5;

	// reading the number of sprites
	fread(&sprites, sizeof(uint32_t), 1, fp);
	offset += sizeof(uint32_t);

	printf("sprite count %u.\n", sprites);
	printf("offset %u.\n", offset);

	// iterate over all sprites
	for (uint32_t j = 0; j < sprites; j++)
	{
		printf("processing sprite %u from offset %u.\n", j + 1, offset);

		// reading the information of sprite j
		fread(&width, sizeof(uint16_t), 1, fp);
		fread(&height, sizeof(uint16_t), 1, fp);
		fread(&dataLength, sizeof(uint32_t), 1, fp);
		offset += sizeof(uint8_t) * 8;

		printf("width: %u height %u size %u\n\n", width, height, dataLength);

		// creating the filename for sprite j
		sprintf(name, "./output/%u.bmp", j + 1);
		// opening a file for write the sprite data
		fb = fopen(name, "wb");

		// calculating the size of the bmp
		uint32_t rawSize = width * height * PIXEL_V4_SIZE;
		// creating the header for the bmp
		d_write_bmp_header_v4(fb, HEADER_V4_SIZE + rawSize);
		d_write_dib_header_v4(fb, width, height, rawSize);

		total = width * height * 4;
		processed = 0;

		while (dataLength > 0)
		{
			// reading the first two bytes to check transparency insertion
			fread(&v1, sizeof(uint8_t), 1, fp);
			fread(&v2, sizeof(uint8_t), 1, fp);

			// if the first pixel data has a 0x1 value we need to add v2 times of transparent pixels
			//&& dataLength < total
			if (v1 == 0x1)
			{
				// transparent pixel
				p.r = 0x00;
				p.g = 0x00;
				p.b = 0x00;
				p.a = 0x00;
				for (int i = 0; i < v2; i++)
				{
					// saving in the bmp file
					d_write_pixel_v4_n(fb, p);
					processed += 4;
				}

				// update dataLenght left to process and offset
				offset += 2 * sizeof(uint8_t);
				dataLength -= 2 * sizeof(uint8_t);
			}
			// read the other 2 bytes and write the pixel rgba data
			else
			{
				// reading the last 2 bytes of a pixel
				fread(&v3, sizeof(uint8_t), 1, fp);
				fread(&v4, sizeof(uint8_t), 1, fp);

				// case for the background image of the client it has a reserved size of 2048x2048, but the size of the image is less than this resolution
				// so you can read until you find 0x0 0x0 0x0 0x0 for optimization
				if (v1 == 0x0 && v2 == 0x0 && v3 == 0x0 && v4 == 0x0)
				{
					offset += 4 * sizeof(uint8_t);
					dataLength -= 4 * sizeof(uint8_t);
					offset += dataLength;
					// skipping all the zeros
					fseek(fp, dataLength, SEEK_CUR);
					break;
				}

				// populate the pixel struct
				p.r = v1;
				p.g = v2;
				p.b = v3;
				p.a = v4;
				// saving in the bmp file
				d_write_pixel_v4_n(fb, p);

				// update processed/offset/datalength
				processed += 4;
				offset += 4 * sizeof(uint8_t);
				dataLength -= 4 * sizeof(uint8_t);
			}
		}

		// fill the rest of sprite with transparent pixels
		if (processed < total)
		{
			// creating the transparent pixel
			p.r = 0x00;
			p.g = 0x00;
			p.b = 0x00;
			p.a = 0x00;
			for (uint32_t i = 0; i < total - processed; i++)
			{
				// write pixel in the bmp
				d_write_pixel_v4_n(fb, p);
			}
		}

		fclose(fb);
	}

	fclose(fp);
	return 0;
}