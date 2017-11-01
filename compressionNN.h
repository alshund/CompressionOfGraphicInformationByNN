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

void startCompression(char *image_path, unsigned int rectangleHeight, unsigned int rectangleWidth);

Matrix* createMatrixX(MatrixOfImage *matrixOfImage, int rectangleHeight, int rectangleWidth);
double** createMasX(MatrixOfImage *matrixOfImage, int height, int width, int rectangleHeight, int recTangleWidth);
double* createBlock(RGB **matrixOfPixels, int startHeight, int finalHeight, int startWidth, int finalWidth, int width);
double getConvertColor(unsigned char color);
double getRandom(double min, double max);

Matrix* createMatrixW(int height, int width);
double** createMasW(int height, int width);

Matrix* createTransposedMatrix(Matrix *W);
double** transposed(double **W, int width, int height);

Matrix* createMatrixY(Matrix *X, Matrix *W);

double* multiplication(double *X, double **W, int width, int iterations);
double getSum(double *X, double **W, int wWidthIndex, int iterations);
double* difference(double *X, double *_X, int width);

void adjustmentFirstLayer(Matrix *W, double *X, double *dX, Matrix *_W);
void adjustmentSecondLayer(Matrix *_W, double *Y, double *dX);
double createAdaptive(double *block, int width);

Matrix* createMatrix_X(Matrix *Y, Matrix *_W);

double getDeviation(Matrix *dX);

void adjustmentY(Matrix *Y, int currentBlock, Matrix *X, Matrix *W);
void adjustment_X(Matrix *_X, int currentBlock, Matrix *Y, Matrix *_W);
void adjustment_dX(Matrix *dX, int currentBlock, Matrix *_X, Matrix *X);
void adjustment_W(Matrix *_W, int currentBlock, Matrix *Y, Matrix *dX);
void adjustmentW(Matrix *W, int currentBlock, Matrix *X, Matrix *dX, Matrix *_W, double *mass);

void startLearn(Matrix *X, Matrix *W, Matrix *Y, Matrix *_W, Matrix *_X, Matrix *dX);
#endif //COMPRESSIONOFGRAPHICINFORMATIONBYNN_COMPRESSIONNN_H
