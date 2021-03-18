#include "draw.h"
#include "main.h"
#include "camera.h"
#include "crap.h"
#include <stdint.h>
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
	memset(screen.pixels, color(0, 0, 0, 255), screen.width * screen.height * sizeof(uint32_t));
}

void draw(const camera_t* camera) {
	vec4_t p[4];
	int count = sizeof(p) / sizeof(p[0]);
	for (int c = 0; c < count; ++c) {
		vec4_t* wp = &p[c];
		*wp = (vec4_t){0.f, 0.f, (float)c, 0.f};
		vec4_t diff;
		sub_vec4(&diff, wp, &camera->pos);
		float dot = dot_vec4(&diff, &(vec4_t){0.f, 0.f, -1.f, 0.f});
		if (dot > 0.f) {
			vec4_t sp = world_to_screen_coords(wp, camera);
			check_pixel(sp.x, sp.y, color(255, 255, 255, 255));
		}
	}
}