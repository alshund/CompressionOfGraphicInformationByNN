#include "compressionNN.h"

int main() {
    char *imagePath = {"resource/image1.bmp"};
    unsigned int rectangleHeight;
    unsigned int rectangleWidth;
    printf("Rectangle height:\n");
    scanf("%d", &rectangleHeight);
    printf("Rectangle width:\n");
    scanf("%d", &rectangleWidth);

    startCompression(imagePath, rectangleHeight, rectangleWidth);

    return 0;
}