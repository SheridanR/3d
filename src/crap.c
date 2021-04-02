#include "crap.h"
#include "main.h"
#include "sim.h"
#include "draw.h"
#include <SDL2/SDL.h>
#include INCLUDE_SDL2_IMAGE

static SDL_Window* window;
static SDL_Surface* surface;
bool keystatus[SDL_NUM_SCANCODES] = { false };

void init() {
    (void)SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_NOPARACHUTE);
#ifdef TEXTURES
    (void)IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
#endif
    window = SDL_CreateWindow("3d", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, XRES, YRES, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    screen.pixels = (uint32_t*)surface->pixels;
    screen.width = (int)surface->w;
    screen.height = (int)surface->h;
    screen.depth = malloc(sizeof(float) * XRES * YRES);
}

void refresh() {
    SDL_UpdateWindowSurface(window);
}

void events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            running = false;
            break;
        }
        case SDL_KEYDOWN: {
            keystatus[event.key.keysym.scancode] = true;
            break;
        }
        case SDL_KEYUP: {
            keystatus[event.key.keysym.scancode] = false;
            break;
        }
        default: {
            break;
        }
        }
    }
}

void term() {
#ifdef TEXTURES
    IMG_Quit();
#endif
    free(screen.depth);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
