#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include "GOL_Image.h"

#define MAYBE_UNUSED(x) (void)(x)

#define APPLICATION_TITLE "Conway's Game of Life (SDL2)"

int32_t main(int32_t argc, char const* argv[]) {
    MAYBE_UNUSED(argc);
    MAYBE_UNUSED(argv);

    SDL_Event     event;     // SDL Event storage
    SDL_Window   *window;    // SDL Window handler
    SDL_Renderer *renderer;  // SDL Renderer Handler

    // Window width and height
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
        SDL_WINDOW_OPENGL            // flags
    );
    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Setup SDL renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Set the play area
    int32_t width  = 64;
    int32_t height = 64;

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width, height);
    // Create image object
    GOL_Image* image = GOL_ConstructImage(width, height, 4);

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            GOL_ImageSetPixel(image, j, i, 255, 0, 255, 255);
        }
    }
    // Upload the image to the texture
    SDL_UpdateTexture(texture, NULL, GOL_ImageGetBuffer(image),
            width * GOL_ImageGetChannels(image) * sizeof(uint8_t));

    // Create two copies of the same board size
    uint8_t* board_cp = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    uint8_t* board    = (uint8_t*)malloc(width * height * sizeof(uint8_t));

    // Generate the seed
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

    // Copy the boards memory onto its copy
    memcpy(board_cp, board, width * height);

    // Setup the time for delay between simulation step
    uint32_t current_time = SDL_GetTicks();
    uint32_t last_update  = current_time;
    uint32_t delay_update = 250;  // ms

    uint8_t is_running = 1;
    while(is_running) {
        current_time = SDL_GetTicks();

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
        if (current_time - last_update > delay_update) {
            // Generate random value onto the board
            // TODO: Create an algoritm for Conway's Game of Life
            for (int32_t i = 0; i < height; i++) {
                for (int32_t j = 0; j < width; j++) {
                    board[i * width + j] = rand() % 2;
                }
            }

            // Reset the last_update
            last_update = current_time;
        }

        // Render

        // Draw the board onto the image
        for (int32_t i = 0; i < height; i++) {
            for (int32_t j = 0; j < width; j++) {
                if (board[i * width + j]) {  // Set 1 to white
                    GOL_ImageSetPixel(image, j, i, 255, 255, 255, 255);
                } else {                     // Set 0 to black
                    GOL_ImageSetPixel(image, j, i, 0, 0, 0, 255);
                }
            }
        }
        // Update the texture data with the new image data
        SDL_UpdateTexture(texture, NULL, GOL_ImageGetBuffer(image),
                          width * GOL_ImageGetChannels(image) * sizeof(uint8_t));

        // Set draw color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Upload the texture to the SDL renderer
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);  // Show image on the screen

        SDL_PollEvent(&event);
    }

    // Memory clean up for the image and the board
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

