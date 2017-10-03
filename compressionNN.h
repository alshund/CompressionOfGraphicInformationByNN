//
// Created by shund on 01.10.2017.
//

#ifndef COMPRESSIONOFGRAPHICINFORMATIONBYNN_COMPRESSIONNN_H
#define COMPRESSIONOFGRAPHICINFORMATIONBYNN_COMPRESSIONNN_H

#include "image_parser.h"


typedef struct {
    double **mas;
    unsigned int height;
    unsigned int width;
} Matrix;

void startCompression(char *image_path, float rectangleHeight, float rectangleWidth);

Matrix* createMatrixX(MatrixOfImage *matrixOfImage);
double** createMasX(RGB **matrixOfPixels, int height, int width);
double getConvertColor(unsigned char color);
double getRandom(double min, double max);

Matrix* createMatrixW(int height, int width);
double** createMasW(int height, int width);

Matrix* createMatrix_W(Matrix *W);
double** createMas_W(double **W, int width, int height);

Matrix* createMatrixY(Matrix *X, Matrix *W);
double** multiplication(double **X, double **W, int height, int width, int iterations);
double getSum(double **X, double **W, int xHeightIndex, int wWidthIndex, int iterations);

Matrix* createMatrix_X(Matrix *Y, Matrix *_W);

double getDeviation(Matrix *X, Matrix *_X);
#endif //COMPRESSIONOFGRAPHICINFORMATIONBYNN_COMPRESSIONNN_H
