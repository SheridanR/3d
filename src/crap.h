#ifndef CRAP_H
#define CRAP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#ifdef __APPLE__
#define INCLUDE_SDL2_IMAGE <SDL2_image/SDL_image.h>
#else
#define INCLUDE_SDL2_IMAGE <SDL2/SDL_image.h>
#endif

void init();
void events();
void refresh();
void term();

extern bool keystatus[SDL_NUM_SCANCODES];

#endif
