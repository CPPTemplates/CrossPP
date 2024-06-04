#include "texture.h"
#pragma once
struct doubleBuffer
{
    inline void swap()
    {
        thread0DoubleBufferIndex = 1 - thread0DoubleBufferIndex;
    }
    // 2 contexts and rendertextures, 1 for each thread. this is to prevent contexts from being created and deleted constantly
    byte thread0DoubleBufferIndex = 0;
    texture *buffer[2]{};
    texture *&operator[](cbyte &threadNumber)
    {
        return buffer[threadNumber ^ thread0DoubleBufferIndex];
    }
};