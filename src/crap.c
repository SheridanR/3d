#include "crap.h"
#include "main.h"
#include "sim.h"
#include "draw.h"
#include <SDL.h>

static SDL_Window* window;
static SDL_Surface* surface;
bool keystatus[SDL_NUM_SCANCODES] = { false };

int mind(int a, int b) {
	return a < b ? a : b;
}

int maxd(int a, int b) {
	return a > b ? a : b;
}

void init() {
	(void)SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"3d",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		XRES,
		YRES,
		SDL_WINDOW_SHOWN
	);
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
	free(screen.depth);
	SDL_DestroyWindow(window);
	SDL_Quit();
}