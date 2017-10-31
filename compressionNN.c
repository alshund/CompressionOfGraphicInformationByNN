//
// Created by shund on 01.10.2017.
//

#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "compressionNN.h"
#include "image_parser.h"

void startCompression(char *imagePath, unsigned int rectangleHeight, unsigned int rectangleWidth) {
    MatrixOfImage *matrixOfImage = getMatrixOfImage(imagePath);
    Matrix *X = createMatrixX(matrixOfImage, rectangleHeight, rectangleWidth);

    printf("%f\n", X->mas[0][0]);

    int blockHeight = (matrixOfImage->height / rectangleHeight) * (matrixOfImage->width / rectangleWidth);
    double **mass = malloc(sizeof(double*) * blockHeight);
    int bHeight = matrixOfImage->height / rectangleHeight;
    int bWidth = matrixOfImage->width / rectangleWidth;
    int bCurrentHeight = -1;
    for (int bHeightIndex = 0; bHeightIndex < bHeight; bHeightIndex++) {

        int startRecHeightIndex = rectangleHeight * bHeightIndex;
        int finalRecHeightIndex = startRecHeightIndex + rectangleHeight;

        for (int bWidthIndex = 0; bWidthIndex < bWidth; bWidthIndex++) {

            int statRecWidthIndex = rectangleWidth * bWidthIndex;
            int finalRecWidthIndex = statRecWidthIndex + rectangleWidth;

            bCurrentHeight++;

            mass[bCurrentHeight] = malloc(sizeof(double) * rectangleHeight * rectangleWidth * 3);
            int bCurrentWidth = 0;

            for (int recHeightIndex = startRecHeightIndex; recHeightIndex < finalRecHeightIndex; recHeightIndex++) {

                for (int recWidthIndex = statRecWidthIndex; recWidthIndex < finalRecWidthIndex; recWidthIndex++) {
                    mass[bCurrentHeight][bCurrentWidth++] = getConvertColor(matrixOfImage->matrixOfPixels[recHeightIndex][recWidthIndex].red);
                    mass[bCurrentHeight][bCurrentWidth++] = getConvertColor(matrixOfImage->matrixOfPixels[recHeightIndex][recWidthIndex].green);
                    mass[bCurrentHeight][bCurrentWidth++] = getConvertColor(matrixOfImage->matrixOfPixels[recHeightIndex][recWidthIndex].blue);
                }
            }
        }
    }
    printf("%f\n", mass[0][0]);

//    Matrix *W = createMatrixW(matrixOfImage->width * 3, matrixOfImage->width * 6);
//    Matrix *Y = createMatrixY(X, W);
//    Matrix *_W = createMatrix_W(W);
//    Matrix *_X = createMatrix_X(Y, _W);
//    double E = getDeviation(X, _X);
//    printf("%f\n", E);
}

Matrix* createMatrixX(MatrixOfImage *matrixOfImage, int rectangleHeight, int rectangleWidth){
    Matrix *X = malloc(sizeof(Matrix));
    X->height = matrixOfImage->height / rectangleHeight * matrixOfImage->width / rectangleWidth;
    X->width = rectangleHeight * rectangleWidth * 3;
    X->mas = createMasX(matrixOfImage, X->height, X->width, rectangleHeight, rectangleWidth);
    return X;
}

double** createMasX(MatrixOfImage *matrixOfImage, int height, int width, int rectangleHeight, int rectangleWidth) {
    double **X = malloc(sizeof(double*) * height);
    int bHeight = matrixOfImage->height / rectangleHeight;
    int bWidth = matrixOfImage->width / rectangleWidth;
    int bCurrentHeight = 0;

    for (int bHeightIndex = 0; bHeightIndex < bHeight; bHeightIndex++) {
        int startRecHeight = rectangleHeight * bHeightIndex;
        int finalRecHeight = startRecHeight + rectangleHeight;

        for (int bWidthIndex = 0; bWidthIndex < bWidth; bWidthIndex++) {
            int statRecWidth = rectangleWidth * bWidthIndex;
            int finalRecWidth = statRecWidth + rectangleWidth;
            X[bCurrentHeight] = malloc(sizeof(double) * width);
            X[bCurrentHeight++] = createBlock(matrixOfImage->matrixOfPixels, startRecHeight, finalRecHeight, statRecWidth, finalRecWidth, width);
        }
    }
    return X;
}

double* createBlock(RGB **matrixOfPixels, int startHeight, int finalHeight, int startWidth, int finalWidth, int width) {
    double *block = malloc(sizeof(double) * width);
    int widthIndex = 0;
    for (int recHeightIndex = startHeight; recHeightIndex < finalHeight; recHeightIndex++) {
        for (int recWidthIndex = startWidth; recWidthIndex < finalWidth; recWidthIndex++) {
            block[widthIndex++] = getConvertColor(matrixOfPixels[recHeightIndex][recWidthIndex].red);
            block[widthIndex++] = getConvertColor(matrixOfPixels[recHeightIndex][recWidthIndex].green);
            block[widthIndex++] = getConvertColor(matrixOfPixels[recHeightIndex][recWidthIndex].blue);
        }
    }
    return block;
}

double getConvertColor(unsigned char color) {
    return color / 255.0 * 2 - 1;
}

Matrix* createMatrixW(int height, int width) {
    Matrix *W = malloc(sizeof(Matrix));
    W->height = height;
    W->width = width;
    W->mas = createMasW(height, width);
    return W;
}

double** createMasW(int height, int width) {
    double **mas = malloc(sizeof(double*) * height);
    srand(time(NULL));
    for (int indexHieght = 0; indexHieght < height; indexHieght++) {
        mas[indexHieght] = malloc(sizeof(double) * width);
        for (int indexWidth = 0; indexWidth < width; indexWidth++) {
            mas[indexHieght][indexWidth] = getRandom(-1.0, 1.0);
        }
    }
    return mas;
}

double getRandom(double min, double max) {
    return (double)rand()/(double)RAND_MAX * (max - min) + min;
}

Matrix* createMatrixY(Matrix *X, Matrix *W) {
    Matrix *Y = malloc(sizeof(Matrix));
    Y->height = X->height;
    Y->width = W->width;
    Y->mas = multiplication(X->mas, W->mas, Y->height, Y->width, X->width);
    return Y;
}

double** multiplication(double **X, double **W, int height, int width, int iterations) {
    double **Y = malloc(sizeof(double*) * height);
    for (int heightIndex = 0; heightIndex < height; heightIndex++) {
        Y[heightIndex] = malloc(sizeof(double) * width);
        for (int widthIndex = 0; widthIndex < width; widthIndex++) {
            Y[heightIndex][widthIndex] = getSum(X, W, heightIndex, widthIndex, iterations);
        }
    }
    return Y;
}

double getSum(double **X, double **W, int xHeightIndex, int wWidthIndex, int iterations) {
    double sum = 0;
    for (int indexIterations = 0; indexIterations < iterations; indexIterations++) {
        sum += X[xHeightIndex][indexIterations] * W[indexIterations][wWidthIndex];
    }
    return sum;
}

Matrix* createMatrix_W(Matrix *W) {
    Matrix *_W = malloc(sizeof(Matrix));
    _W->height = W->width;
    _W->width = W->height;
    _W->mas = createMas_W(W->mas, _W->height, _W->width);
    return _W;
}

double** createMas_W(double **W, int height, int width) {
    double **_W = malloc(sizeof(double*) * height);
    for (int indexHeight = 0; indexHeight < height; indexHeight++) {
        _W[indexHeight] = malloc(sizeof(double) * width);
        for (int indexWidth = 0; indexWidth < width; indexWidth++) {
            _W[indexHeight][indexWidth] = W[indexWidth][indexHeight];
        }
    }
    return _W;
}

Matrix* createMatrix_X(Matrix *Y, Matrix *_W) {
    Matrix *_X = malloc(sizeof(Matrix));
    _X->height = Y->height;
    _X->width = _W->width;
    _X->mas = multiplication(Y->mas, _W->mas, _X->height, _W->width, Y->width);
    return _X;
}

double getDeviation(Matrix *X, Matrix *_X) {
    double E = 0.0;
    for (int indexHeight = 0; indexHeight < X->height; indexHeight++) {
        for (int indexWidth = 0; indexWidth < X->width; indexWidth++) {
            double dX = _X->mas[indexHeight][indexWidth] - X->mas[indexHeight][indexWidth];
            E += dX * dX;
        }
    }
    return E;
}






