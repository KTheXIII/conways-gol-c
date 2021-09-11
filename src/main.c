#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>

#define MAYBE_UNUSED(x) (void)(x)

#define APPLICATION_TITLE "Conway's Game of Life (SDL2)"

typedef struct {
    int32_t width;
    int32_t height;
    int32_t channels;

    uint8_t* buffer;
} GOL_Image;
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} GOL_Color;

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
void GOL_ImageSetPixelColor(GOL_Image* image,
                            int32_t x, int32_t y,
                            GOL_Color const* color) {
    if (x > -1 && x < image->width && y > -1 && y < image->height) {
        int32_t index = (y * image->channels) * image->width + (x * image->channels);
        image->buffer[index + 0] = color->red;
        image->buffer[index + 1] = color->green;
        image->buffer[index + 2] = color->blue;
        image->buffer[index + 3] = color->alpha;
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

int32_t main(int32_t argc, char const* argv[]) {
    MAYBE_UNUSED(argc);
    MAYBE_UNUSED(argv);

    SDL_Event     event;     // SDL Event storage
    SDL_Window   *window;    // SDL Window handler
    SDL_Renderer *renderer;  // SDL Renderer Handler

    int32_t WIDTH  = 512;
    int32_t HEIGHT = 512;

    SDL_Init(SDL_INIT_VIDEO);  // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        APPLICATION_TITLE,           // window title
        SDL_WINDOWPOS_UNDEFINED,     // initial x position
        SDL_WINDOWPOS_UNDEFINED,     // initial y position
        WIDTH,                       // width, in pixels
        HEIGHT,                      // height, in pixels
        SDL_WINDOW_OPENGL            // flags - see below
    );
    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //SDL_Surface* uv_surf = SDL_LoadBMP("assets/uvgrid.bmp");
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, uv_surf);
    //SDL_FreeSurface(uv_surf);

    int32_t width  = 128;
    int32_t height = 128;

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width, height);
    GOL_Image* image = GOL_ConstructImage(width, height, 4);

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            GOL_ImageSetPixel(image, j, i, 255, 0, 255, 255);
        }
    }
    SDL_UpdateTexture(texture, NULL, GOL_ImageGetBuffer(image), width * 4 * sizeof(uint8_t));

    uint8_t* board_cp = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    uint8_t* board    = (uint8_t*)malloc(width * height * sizeof(uint8_t));

    srand(time(NULL));
    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            int32_t v = rand() % 100;
            if (v > 50) {
                board[i * width + j] = 1;
            } else {
                board[i * width + j] = 0;
            }
        }
    }

    memcpy(board_cp, board, width * height);

    uint8_t is_running = 1;
    while(is_running) {
        // Handle inputs
        switch(event.type) {
            case SDL_QUIT:
                is_running = 0;
                break;
            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    is_running = 0;
                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_MOUSEWHEEL:
                break;
        }
        // Update
        for (int32_t i = 0; i < height; i++) {
            for (int32_t j = 0; j < width; j++) {

            }
        }

        // Render
        for (int32_t i = 0; i < height; i++) {
            for (int32_t j = 0; j < width; j++) {
                if (board[i * width + j]) {
                    GOL_ImageSetPixel(image, j, i, 255, 255, 255, 255);
                } else {
                    GOL_ImageSetPixel(image, j, i, 0, 0, 0, 255);
                }
            }
        }
        SDL_UpdateTexture(texture, NULL, GOL_ImageGetBuffer(image), width * 4 * sizeof(uint8_t));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
    }

    GOL_DestructImage(image);
    free(board_cp);
    free(board);

    // Delete texture memory
    SDL_DestroyTexture(texture);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}

