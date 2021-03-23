#include "draw.h"
#include "main.h"
#include "camera.h"
#include "sim.h"
#include <string.h>

window_t screen;

uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);
}

void pixel(uint32_t x, uint32_t y, uint32_t color) {
	screen.pixels[y * screen.width + x] = color;
}

void check_pixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x < 0 || x >= XRES || y < 0 || y >= YRES) {
		return;
	} else {
		pixel(x, y, color);
	}
}

void clear() {
	memset(screen.depth, 0, screen.width * screen.height * sizeof(float));
	memset(screen.pixels, 0, screen.width * screen.height * sizeof(uint32_t));
}

void draw() {
	draw_obj(&obj, &camera);
}