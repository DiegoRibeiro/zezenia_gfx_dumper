cmake_minimum_required (VERSION 2.6)
project (zezenia_sprite_dumper)

# version number
set (ZEZENIA_SPRITE_DUMPER_VERSION_MAJOR 1)
set (ZEZENIA_SPRITE_DUMPER_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

# add the binary tree to the search path for include files
# so that we will find version.h
include_directories("${PROJECT_BINARY_DIR}")

# library 
include_directories ("${PROJECT_SOURCE_DIR}/d_bitmap/src")
add_subdirectory (d_bitmap)

add_executable(dumpZ "${PROJECT_SOURCE_DIR}/src/main.c")
target_link_libraries (dumpZ d_bitmap)

# add the install targets
install (TARGETS dumpZ DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/version.h"        
         DESTINATION include)