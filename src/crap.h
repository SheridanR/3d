#ifndef CRAP_H
#define CRAP_H

#include <SDL.h>

void init();
void update();
void refresh();
void term();

extern SDL_Window* window;
extern SDL_Surface* surface;

#endif