#ifndef CRAP_H
#define CRAP_H

#include <stdbool.h>
#include <SDL.h>

void init();
void events();
void refresh();
void term();

extern bool keystatus[SDL_NUM_SCANCODES];

#endif