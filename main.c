#include <stdlib.h>
#include <time.h>
#include "compressionNN.h"

int main() {
    char *imagePath = {"resource/image1.bmp"};
    float rectangleHeight;
    float rectangleWidth;
//    printf("Rectangle height:\n");
//    scanf("%f", &rectangleHeight);
//    printf("Rectangle width:\n");
//    scanf("%f", &rectangleWidth);
    startCompression(imagePath, rectangleHeight, rectangleWidth);
    return 0;
}