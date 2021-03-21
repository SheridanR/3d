#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

typedef struct point {
	float x;
	float y;
} point_t;

void draw_triangle(point_t p0, point_t p1, point_t p2, uint32_t color);

#endif