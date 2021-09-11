#include "GOL_Image.h"

#include <stdlib.h>

GOL_Image* GOL_ConstructImage(int32_t width, int32_t height, int32_t channels) {
    GOL_Image* image = (GOL_Image*)malloc(sizeof(GOL_Image));
    image->width     = width;
    image->height    = height;
    image->channels  = channels;
    image->buffer    = (uint8_t*)malloc(width * height * channels * sizeof(uint8_t));
    return image;
}
void GOL_DestructImage(GOL_Image* image) {
    free(image->buffer);
    free(image);
}
void GOL_ImageSetPixel(GOL_Image* image,
                       int32_t x, int32_t y,
                       uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    if (x > -1 && x < image->width && y > -1 && y < image->height) {
        int32_t index = (y * image->channels) * image->width + (x * image->channels);
        image->buffer[index + 0] = red;
        image->buffer[index + 1] = green;
        image->buffer[index + 2] = blue;
        image->buffer[index + 3] = alpha;
    }
}
uint8_t* GOL_ImageGetBuffer(GOL_Image* image) {
    return image->buffer;
}
int32_t GOL_ImageGetHeight(GOL_Image* image) {
    return image->height;
}
int32_t GOL_ImageGetWidth(GOL_Image* image) {
    return image->width;
}
int32_t GOL_ImageGetChannels(GOL_Image* image) {
    return image->channels;
}

