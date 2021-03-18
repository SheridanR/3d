#include <SDL.h>
#include <stdint.h>
#include <string.h>
#include "crap.h"

uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);
}

void pixel(SDL_Surface* surf, uint32_t x, uint32_t y, uint32_t color) {
	*(uint32_t*)((uint8_t*)surf->pixels)[y * surf->pitch + x * surf->format->BytesPerPixel] = color;
}

void clear() {
	memset(surface->pixels, color(0, 0, 0, 255), surface->pitch * surface->h);
}