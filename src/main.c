#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define MAYBE_UNUSED(x) (void)(x)

#define APPLICATION_TITLE "Conway's Game of Life (SDL2)"

int32_t main(int32_t argc, char const* argv[]) {
    MAYBE_UNUSED(argc);
    MAYBE_UNUSED(argv);

    SDL_Event     event;     // SDL Event storage
    SDL_Window   *window;    // SDL Window handler
    SDL_Renderer *renderer;  // SDL Renderer Handler

    int32_t WIDTH  = 640;
    int32_t HEIGHT = 480;

    SDL_Init(SDL_INIT_VIDEO);  // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        APPLICATION_TITLE,                        // window title
        SDL_WINDOWPOS_UNDEFINED,                  // initial x position
        SDL_WINDOWPOS_UNDEFINED,                  // initial y position
        WIDTH,                                    // width,  in pixels
        HEIGHT,                                   // height, in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE  // flags
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

    int32_t width    = 256;
    int32_t height   = 256;
    int32_t channels = 4;
    uint8_t* pixels = (uint8_t*)malloc(width * height * channels * sizeof(uint8_t));

    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width, height);

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            int32_t index = (i * channels) * width + (j * channels);

            if (i % 10 == 0 && j % 10 == 0) {
                pixels[index + 0] = 255;
                pixels[index + 1] = 255;
                pixels[index + 2] = 255;
                pixels[index + 3] = 255;
            } else {
                pixels[index + 0] = 0;
                pixels[index + 1] = 0;
                pixels[index + 2] = 0;
                pixels[index + 3] = 255;
            }
        }
    }
    SDL_UpdateTexture(texture, NULL, pixels, width * channels * sizeof(uint8_t));

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
            case SDL_WINDOWEVENT:
                break;
        }
        SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
        // Update
        int32_t midxoff = WIDTH / 2 - HEIGHT / 2;
        SDL_Rect outrect = {
            midxoff, 0, HEIGHT, HEIGHT
        };

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, &outrect);
        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
    }

    free(pixels);
    // Delete texture memory
    SDL_DestroyTexture(texture);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}

