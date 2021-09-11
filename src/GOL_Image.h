#ifndef GOL_IMAGE_H_
#define GOL_IMAGE_H_

#include <stdint.h>

typedef struct {
    int32_t width;
    int32_t height;
    int32_t channels;

    uint8_t* buffer;
} GOL_Image;

GOL_Image* GOL_ConstructImage(int32_t width, int32_t height, int32_t channels);
void GOL_DestructImage(GOL_Image* image);

void GOL_ImageSetPixel(GOL_Image* image,
                       int32_t x, int32_t y,
                       uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
uint8_t* GOL_ImageGetBuffer(GOL_Image* image);
int32_t GOL_ImageGetHeight(GOL_Image* image);
int32_t GOL_ImageGetWidth(GOL_Image* image);
int32_t GOL_ImageGetChannels(GOL_Image* image);

#endif
