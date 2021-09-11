#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "GOL_Image.h"
#include "GOL_Board.h"

#define MAYBE_UNUSED(x) (void)(x)

#define APPLICATION_TITLE "Conway's Game of Life (SDL2)"

int32_t main(int32_t argc, char const* argv[]) {
    MAYBE_UNUSED(argc);
    MAYBE_UNUSED(argv);

    SDL_Event     event;     // SDL Event storage
    SDL_Window   *window;    // SDL Window handler
    SDL_Renderer *renderer;  // SDL Renderer Handler

    int32_t WIDTH  = 1280;
    int32_t HEIGHT = 720;

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

    //int32_t width  = 1280;
    //int32_t height = 720;
    int32_t width  = 320;
    int32_t height = 180;
    //int32_t width  = 160;
    //int32_t height = 90;
    //int32_t width  = 80;
    //int32_t height = 45;

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width, height);
    GOL_Board* board = GOL_ConstructBoard(width, height);
    GOL_Image* image = GOL_ConstructImage(width, height, 4);

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            GOL_ImageSetPixel(image, j, i, 255, 0, 255, 255);
        }
    }
    SDL_UpdateTexture(texture, NULL, GOL_ImageGetBuffer(image), width * 4 * sizeof(uint8_t));

    srand(time(NULL));
    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            int32_t v = rand() % 2;
            GOL_BoardSetCellState(board, j, i, v);
        }
    }

    uint32_t current_time = SDL_GetTicks();
    uint32_t last_update  = current_time;
    uint32_t delay_update = 125;  // ms

    uint8_t last_r = 0;
    uint8_t current_r = 0;
    uint8_t last_s = 0;
    uint8_t current_s = 0;

    uint8_t is_running = 1;
    while(is_running) {
        current_time = SDL_GetTicks();
        last_r = current_r;
        last_s = current_s;

        // Handle inputs
        switch(event.type) {
            case SDL_QUIT:
                is_running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_R)
                    current_r = 1;
                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                    current_s = 1;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE
                        || event.key.keysym.scancode == SDL_SCANCODE_Q)
                    is_running = 0;

                if (event.key.keysym.scancode == SDL_SCANCODE_R)
                    current_r = 0;
                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                    current_s = 0;
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
        if (current_r == 0 && last_r == 1)
            for (int32_t i = 0; i < height; i++) {
                for (int32_t j = 0; j < width; j++) {
                    int32_t v = rand() % 2;
                    GOL_BoardSetCellState(board, j, i, v);
                }
            }
        if (current_s == 0 && last_s == 1)
            GOL_BoardSimulate(board);

        if (current_time - last_update > delay_update) {
            GOL_BoardSimulate(board);
            last_update = current_time;
        }

        // Render
        for (int32_t i = 0; i < height; i++) {
            for (int32_t j = 0; j < width; j++) {
                if (GOL_BoardGetCellState(board, j, i)) {
                    GOL_ImageSetPixel(image, j, i, 255, 255, 255, 255);
                } else {
                    GOL_ImageSetPixel(image, j, i, 0, 0, 0, 255);
                }
            }
        }
        SDL_UpdateTexture(texture, NULL, GOL_ImageGetBuffer(image),
                          width * GOL_ImageGetChannels(image) * sizeof(uint8_t));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
    }

    // Free resources
    GOL_DestructImage(image);
    GOL_DestructBoard(board);

    // Delete texture memory
    SDL_DestroyTexture(texture);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}

