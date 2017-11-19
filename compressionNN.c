//
// Created by shund on 01.10.2017.
//

#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "compressionNN.h"

void startCompression(char *imagePath, unsigned int rectangleHeight, unsigned int rectangleWidth) {
    MatrixOfImage *matrixOfImage = getMatrixOfImage(imagePath);

    Matrix *X = createMatrixX(matrixOfImage, rectangleHeight, rectangleWidth);

    Matrix *W = createMatrixW(X->width, 100);

    Matrix *Y = createMatrixY(X, W);

    Matrix *_W = createTransposedMatrix(W);

    Matrix *_X = createMatrix_X(Y, _W);

    Matrix *dX = createMatrix_X(Y, _W);

    startLearn(X, W, Y, _W, _X, dX);

    MatrixOfImage *result = toMatrixOfImage(_X, matrixOfImage->height, matrixOfImage->width, rectangleHeight, rectangleWidth);

    formImage(result);
}

Matrix* createMatrixX(MatrixOfImage *matrixOfImage, int rectangleHeight, int rectangleWidth){
    Matrix *X = malloc(sizeof(Matrix));
    X->height = matrixOfImage->height / rectangleHeight * matrixOfImage->width / rectangleWidth;
    X->width = (unsigned int) (rectangleHeight * rectangleWidth * 3);
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

unsigned char toRGB(double convertedColor) {
    return (unsigned char) (((convertedColor + 1) / 2.0) * 255);
}

MatrixOfImage* toMatrixOfImage(Matrix *_X, unsigned int iHeight, unsigned int iWidth, unsigned int rectangleHeight, unsigned int rectangleWidth) {
    MatrixOfImage *matrixOfImage = malloc(sizeof(MatrixOfImage));
    matrixOfImage->height = iHeight;
    matrixOfImage->width = iWidth;
    matrixOfImage->matrixOfPixels = toMatrixOfPixels(_X, iHeight, iWidth, rectangleHeight, rectangleWidth);
    return matrixOfImage;
}

RGB** toMatrixOfPixels(Matrix *_X, unsigned int iHeight, unsigned int iWidth, unsigned int rectangleHeight, unsigned int rectangleWidth) {
    int _xHeightIndex = 0;
    int _xWidthIndex = 0;
    int rowNumber = 0;
    int blockNumber = 0;

    RGB **matrixOfPixels = malloc(sizeof(RGB*) * iHeight);
    for (int iHieghtIndex = 0; iHieghtIndex < iHeight; iHieghtIndex++) {

        matrixOfPixels[iHieghtIndex] = malloc(sizeof(RGB) * iWidth);
        for (int iWidthIndex = 0; iWidthIndex < iWidth; iWidthIndex++) {

            matrixOfPixels[iHieghtIndex][iWidthIndex].red = toRGB(_X->mas[_xHeightIndex][_xWidthIndex++]);
            matrixOfPixels[iHieghtIndex][iWidthIndex].green = toRGB(_X->mas[_xHeightIndex][_xWidthIndex++]);
            matrixOfPixels[iHieghtIndex][iWidthIndex].blue = toRGB(_X->mas[_xHeightIndex][_xWidthIndex++]);

            if (_xWidthIndex % rectangleWidth * 3 == 0) {
                _xWidthIndex = rowNumber * rectangleWidth * 3;
                _xHeightIndex++;
            }
        }
        ++rowNumber;
        if (rowNumber % rectangleHeight == 0) {
            rowNumber = 0;
            blockNumber += iWidth / rectangleWidth;
            _xWidthIndex = rowNumber;
            _xHeightIndex = blockNumber;
        } else {
            _xHeightIndex = blockNumber;
        }
    }
    return matrixOfPixels;
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
            mas[indexHieght][indexWidth] = getRandom(-0.1, 0.1);
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
    Y->mas = malloc(sizeof(double) * Y->height);
    for (int indexHeight = 0; indexHeight < Y->height; indexHeight++) {
        Y->mas[indexHeight] = malloc(sizeof(double) * Y->width);
    }
    return Y;
}

Matrix* createTransposedMatrix(Matrix *W) {
    Matrix *_W = malloc(sizeof(Matrix));
    _W->height = W->width;
    _W->width = W->height;
    _W->mas = transposed(W->mas, _W->height, _W->width);
    return _W;
}

double** transposed(double **W, int height, int width) {
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
    _X->mas = malloc(sizeof(double) * _X->height);
    for (int indexHeight = 0; indexHeight < _X->height; indexHeight++) {
        _X->mas[indexHeight] = malloc(sizeof(double) * _X->width);
    }
    return _X;
}

double getDeviation(Matrix *dX) {
    double E = 0.0;
    for (int heightIndex = 0; heightIndex < dX->height; heightIndex++) {
        for (int widthIndex = 0; widthIndex < dX->width; widthIndex++) {
            E += pow(dX->mas[heightIndex][widthIndex], 2);
        }
    }
    return E;
}

void startLearn(Matrix *X, Matrix *W, Matrix *Y, Matrix *_W, Matrix *_X, Matrix *dX) {
    double dev;
    double *mass = malloc(sizeof(double) * _W->height);
    int iteration = 0;
    do {
        printf("Iteration %d\n", iteration++);
        for (int heightIndex = 0; heightIndex < X->height; heightIndex++) {
            adjustmentY(Y, heightIndex, X, W);
            adjustment_X(_X, heightIndex, Y, _W);
            adjustment_dX(dX, heightIndex, _X, X);
            adjustment_W(_W, heightIndex, Y, dX);
            adjustmentW(W, heightIndex, X, dX, _W, mass);
        }
        dev = getDeviation(dX);
        printf("%f\n", dev);
    } while(dev >= 0.1 * W->width);
}

void adjustmentY(Matrix *Y, int currentBlock, Matrix *X, Matrix *W) {
    for (int neuronIndex = 0; neuronIndex < W->width; neuronIndex++) {
        Y->mas[currentBlock][neuronIndex] = 0.0;
        for (int blockIndex = 0; blockIndex < W->height; blockIndex++) {
            Y->mas[currentBlock][neuronIndex] += X->mas[currentBlock][blockIndex] * W->mas[blockIndex][neuronIndex];
        }
    }
}

void adjustment_X(Matrix *_X, int currentBlock, Matrix *Y, Matrix *_W) {
    for (int pixelIndex = 0; pixelIndex < _W->width; pixelIndex++) {
        _X->mas[currentBlock][pixelIndex] = 0.0;
        for (int blockIndex = 0; blockIndex < _W->height; blockIndex++) {
            _X->mas[currentBlock][pixelIndex] += Y->mas[currentBlock][blockIndex] * _W->mas[blockIndex][pixelIndex];
        }
    }
}

void adjustment_dX(Matrix *dX, int currentBlock, Matrix *_X, Matrix *X) {
    for (int widthIndex = 0; widthIndex < X->width; widthIndex++) {
        dX->mas[currentBlock][widthIndex] = _X->mas[currentBlock][widthIndex] - X->mas[currentBlock][widthIndex];
    }
}

void adjustment_W(Matrix *_W, int currentBlock, Matrix *Y, Matrix *dX) {
    double adaptive = createAdaptive(Y->mas[currentBlock], Y->width);
    for (int indexHeight = 0; indexHeight < _W->height; indexHeight++) {
        for (int indexWidth = 0; indexWidth < _W->width; indexWidth++) {
            _W->mas[indexHeight][indexWidth] -= adaptive * Y->mas[currentBlock][indexHeight] * dX->mas[currentBlock][indexWidth];
        }
    }
}

void adjustmentW(Matrix *W, int currentBlock, Matrix *X, Matrix *dX, Matrix *_W, double *mass) {
    double adaptive = createAdaptive(X->mas[currentBlock], X->width);
    for (int indexHeight = 0; indexHeight < _W->height; indexHeight++) {
        mass[indexHeight] = 0.0;
        for (int indexWidth = 0; indexWidth < _W->width; indexWidth++) {
            mass[indexHeight] += dX->mas[currentBlock][indexWidth] * _W->mas[indexHeight][indexWidth];
        }
    }

    for (int heightIndex = 0; heightIndex < W->height; heightIndex++) {
        for (int widthIndex = 0; widthIndex < W->width; widthIndex++) {
            W->mas[heightIndex][widthIndex] -= adaptive * X->mas[currentBlock][heightIndex] * mass[widthIndex];
        }
    }
}


double* multiplication(double *X, double **W, int width, int iterations) {
    double *multiplication = malloc(sizeof(double) * width);
    for (int widthIndex = 0; widthIndex < width; widthIndex++) {
        multiplication[widthIndex] = getSum(X, W, widthIndex, iterations);
    }
    return multiplication;
}

double getSum(double *X, double **W, int wWidthIndex, int iterations) {
    double sum = 0;
    for (int indexIterations = 0; indexIterations < iterations; indexIterations++) {
        sum = sum + X[indexIterations] * W[indexIterations][wWidthIndex];
    }
    return sum;
}

double* difference(double *X, double *_X, int width) {
    double *diff = malloc(sizeof(double) * width);
    for (int widthIndex = 0; widthIndex < width; widthIndex++) {
        diff[widthIndex] = _X[widthIndex] - X[widthIndex];
    }
    return diff;
}

void adjustmentFirstLayer(Matrix *W, double *X, double *dX, Matrix *_W) {
    double adaptive = createAdaptive(X, W->height);
    Matrix *transposed_W = createTransposedMatrix(_W);
    double *mass = multiplication(dX, transposed_W->mas, transposed_W->width, W->height);
    for (int heightIndex = 0; heightIndex < W->height; heightIndex++) {
        for (int widthIndex = 0; widthIndex < W->width; widthIndex++) {
            W->mas[heightIndex][widthIndex] -= adaptive * X[heightIndex] * mass[widthIndex];
        }
    }
}

void adjustmentSecondLayer(Matrix *_W, double *Y, double *dX) {
    double adaptive = createAdaptive(Y, _W->height);
    for (int heightIndex = 0; heightIndex < _W->height; heightIndex++) {
        for (int widthIndex = 0; widthIndex < _W->width; widthIndex++) {
            _W->mas[heightIndex][widthIndex] -= adaptive * Y[heightIndex] * dX[widthIndex];
        }
    }
}

double createAdaptive(double *block, int width) {
    double sum = 2304.0;//TODO: P * N;
    for (int widthIndex = 0; widthIndex < width; widthIndex++) {
        sum = sum + pow(block[widthIndex], 2);
    }
    return 1.0 / sum;
}






