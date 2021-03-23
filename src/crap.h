#ifndef CRAP_H
#define CRAP_H

#include <stdbool.h>
#include <SDL.h>

void init();
void events();
void refresh();
void term();

extern bool keystatus[SDL_NUM_SCANCODES];
extern inline int mind(int a, int b);
extern inline int maxd(int a, int b);

#endif