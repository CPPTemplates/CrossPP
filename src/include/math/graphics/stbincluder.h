#pragma once
#pragma warning(push, 0)
#pragma gcc diagnostics push
//define to get rid of the deprecation errors
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-function"

#define __STDC_LIB_EXT1__
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION//needs to be BEFORE #include "ImageLoader.h" (ifndef)
#include "imageLoader.h"
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imageWriter.h"

#pragma gcc diagnostics pop
#pragma warning(pop)