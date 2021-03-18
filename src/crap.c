#include <SDL.h>
#include "main.h"

SDL_Window* window;
SDL_Surface* surface;

void init() {
	(void)SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"Maze",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		XRES,
		YRES,
		SDL_WINDOW_SHOWN
	);
	surface = SDL_GetWindowSurface(window);
}

void refresh() {
	SDL_UpdateWindowSurface(window);
}

void update() {
	
}

void term() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}