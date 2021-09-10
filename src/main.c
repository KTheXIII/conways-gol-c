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

    SDL_Surface* uv_surf = SDL_LoadBMP("assets/uvgrid.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, uv_surf);
    SDL_FreeSurface(uv_surf);

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

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_PollEvent(&event);
    }

    // Delete texture memory
    SDL_DestroyTexture(texture);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}

