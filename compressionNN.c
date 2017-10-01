//
// Created by shund on 01.10.2017.
//

#include <malloc.h>
#include "compressionNN.h"
#include "image_parser.h"

void startCompression(char *imagePath, unsigned int rectangleHeight, unsigned int rectangleWidth) {
    MatrixOfImage *matrixOfImage = getMatrixOfImage(imagePath);
    printf("%d", matrixOfImage->matrixOfPixels[0][0].blue);
}

