#include <SDL.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "crap.h"
#include "camera.h"

uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);
}

void pixel(uint32_t x, uint32_t y, uint32_t color) {
	*(uint32_t*)(&((uint8_t*)surface->pixels)[y * surface->pitch + x * surface->format->BytesPerPixel]) = color;
}

void check_pixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x < 0 || x >= XRES || y < 0 || y >= YRES) {
		return;
	} else {
		pixel(x, y, color);
	}
}

void clear() {
	memset(surface->pixels, color(0, 0, 0, 255), surface->pitch * surface->h);
}

void draw(const camera_t* camera) {

	vec4_t p = vec4(0.f);
	vec4_t sp = world_to_screen_coords(&p, camera);

	SDL_LockSurface(surface);
	check_pixel(sp.x, sp.y, color(255, 255, 255, 255));
	SDL_UnlockSurface(surface);
}