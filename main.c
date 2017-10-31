#include <stdlib.h>
#include <time.h>
#include "compressionNN.h"

int main() {
    char *imagePath = {"image.bmp"};
    int rectangleHeight;
    int rectangleWidth;
    printf("Rectangle height:\n");
    scanf("%d", &rectangleHeight);
    printf("Rectangle width:\n");
    scanf("%d", &rectangleWidth);
    startCompression(imagePath, rectangleHeight, rectangleWidth);
    return 0;
}