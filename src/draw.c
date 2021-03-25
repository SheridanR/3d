#include "draw.h"
#include "main.h"
#include "camera.h"
#include "sim.h"
#include <string.h>
#include <stdbool.h>
#include <float.h>

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

bool check_depth(uint32_t x, uint32_t y, float depth) {
	return screen.depth[y * screen.width + x] > depth;
}

void write_depth(uint32_t x, uint32_t y, float depth) {
	screen.depth[y * screen.width + x] = depth;
}

void clear() {
	for (size_t c = 0; c < XRES * YRES; ++c) {
		screen.pixels[c] = 0;
		screen.depth[c] = FLT_MAX;
	}
}

void draw() {
	if (obj.valid) {
		draw_obj(&obj, &camera);
	}
}