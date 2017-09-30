//
// Created by shund on 01.10.2017.
//

#include "image_parser.h"

void get_a_matrix_of_pixels() {
    char *image_path = {"resource/image.bmp"};

    BmpHeader bmpHeader;
    BmpImageInfo bmpImageInfo;
    FILE *read_image = fopen(image_path, "rb");
    if (read_image != NULL) {
        fread(&bmpHeader, 1, sizeof(BmpHeader), read_image);
        fread(&bmpImageInfo, 1, sizeof(BmpImageInfo), read_image);
    }
}
