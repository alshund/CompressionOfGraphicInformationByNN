//
// Created by shund on 01.10.2017.
//

//
// Created by shund on 01.10.2017.
//

#ifndef COMPRESSIONOFGRAPHICINFORMATIONBYNN_IMAGEPARSER_H
#define COMPRESSIONOFGRAPHICINFORMATIONBYNN_IMAGEPARSER_H

#include <stdio.h>

typedef struct
{
    char signature[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int offset;
} BmpHeader;

typedef struct
{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planeCount;
    unsigned short bitDepth;
    unsigned int compression;
    unsigned int compressedImageSize;
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    unsigned int colorsCount;
    unsigned int importantColors;

} BmpImageInfo;

void get_a_matrix_of_pixels();
#endif //COMPRESSIONOFGRAPHICINFORMATIONBYNN_IMAGEPARSER_H
