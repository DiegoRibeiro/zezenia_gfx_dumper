# How to compile

## Get the submodule

```
git submodule update --recursive
```

## Windows (Powershell):

Require mingw32-make, gcc, g++:

```
mkdir build;cd build
```
```
cmake -G "MinGW Makefiles" ../
```
```
mingw32-make all
```
Optional:
```
mingw32-make install
```

## Linux

```
mkdir build && cd build
```
```
cmake ../
```
```
make all
```
Optional: 
```
make install
```

# How to use
After compilation do the following steps:
- Copy the zezenia gfx to the executable folder. 
- Create a folder named "output" on the executable folder.
- Run the executable.

# GFX structure
This file is using the little endian notation.
```
[5A][F7][93][1A][5F]     -- 5 bytes signature?
[FD][27][00][00]         -- 4 bytes number of sprites    10237	

[20][00]                 -- width sprite                 32
[20][00]                 -- height sprite                32
[00][10][00][00]         -- length data pixel            4096

[2C][53][92][FF]         -- bgra
```

## important
- First value of a pixel starting with [01] shows that the next byte is the number of transparent pixel you need to add.
- The photo of the login screen has a size reserved of 2048x2048. But only a small portion is being utilized. You can skip when you find the first 4 zeros (0x00 0x00 0x00 0x00).
- You have to add the rest of the pixels if the length of the data is less than the total data (lengthData < width * height * 4). Add  (width * height * 4) - lengthData transparent Pixels. 

Thanks to this (https://tpforums.org/forum/archive/index.php/t-6351.html) topic i figure out the structure.
