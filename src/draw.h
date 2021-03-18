#ifndef DRAW_H
#define DRAW_H

#include "camera.h"
#include <stdint.h>

typedef struct window {
	uint32_t* pixels;
	int width;
	int height;
} window_t;

extern window_t screen;
extern inline uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
extern inline void pixel(uint32_t x, uint32_t y, uint32_t color);
extern inline void check_pixel(uint32_t x, uint32_t y, uint32_t color);
extern inline void clear();
void draw(const camera_t* camera);

#endif