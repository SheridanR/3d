#include "draw.h"
#include "main.h"
#include "camera.h"
#include "sim.h"
#include "crap.h"
#include "triangle.h"
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

void draw() {
	vec4_t* p = cube;
	int count = sizeof(cube) / sizeof(vec4_t);
	for (int c = 0; c < count; c += 3) {
		for (int i = 0; i < 3; ++i) {
			vec4_t diff;
			sub_vec4(&diff, &p[c + i], &camera.pos);
			vec4_t dir = quat_to_vec3(&camera.ang);
			float dot = dot_vec4(&diff, &dir);
			if (dot <= 0.f) {
				goto next;
			}
		}
		vec4_t sp[3];
		sp[0] = world_to_screen_coords(&p[c], &camera);
		sp[1] = world_to_screen_coords(&p[c + 1], &camera);
		sp[2] = world_to_screen_coords(&p[c + 2], &camera);
		point_t t[3];
		t[0] = (point_t){sp[0].x, sp[0].y};
		t[1] = (point_t){sp[1].x, sp[1].y};
		t[2] = (point_t){sp[2].x, sp[2].y};
		uint8_t r, g, b;
		r = (p[c].x + p[c + 1].x + p[c + 2].x) * 255.f / 3.f;
		g = (p[c].y + p[c + 1].y + p[c + 2].y) * 255.f / 3.f;
		b = (p[c].z + p[c + 1].z + p[c + 2].z) * 255.f / 3.f;
		draw_triangle(t[0], t[1], t[2], color(r, g, b, 255));
		//check_pixel(sp[0].x, sp[0].y, color(255, 255, 255, 255));
		//check_pixel(sp[1].x, sp[1].y, color(255, 255, 255, 255));
		//check_pixel(sp[2].x, sp[2].y, color(255, 255, 255, 255));
next:
		continue;
	}
}