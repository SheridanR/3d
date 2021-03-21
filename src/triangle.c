#include "main.h"
#include "triangle.h"
#include "draw.h"
#include <stdlib.h>

static inline void triangle_top(point_t p[3], uint32_t color) {
	float x0 = p[0].x;
	float x1 = p[0].x;
	float inv_slope0 = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	float inv_slope1 = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	int starty = (int)p[0].y < 0 ? 0 : ((int)p[0].y >= YRES ? YRES - 1 : (int)p[0].y);
	int endy = (int)p[1].y < 0 ? 0 : ((int)p[1].y >= YRES ? YRES - 1 : (int)p[1].y);
	x0 += inv_slope0 * abs(starty - (int)p[0].y);
	x1 += inv_slope1 * abs(starty - (int)p[0].y);
	for (int y = starty; y <= endy; ++y) {
		if (y >= 0 && y < YRES) {
			int startx, endx;
			if (x0 < x1) {
				startx = (int)x0 < 0 ? 0 : ((int)x0 >= XRES ? XRES - 1 : (int)x0);
				endx = (int)x1 < 0 ? 0 : ((int)x1 >= XRES ? XRES - 1 : (int)x1);
			} else {
				startx = (int)x1 < 0 ? 0 : ((int)x1 >= XRES ? XRES - 1 : (int)x1);
				endx = (int)x0 < 0 ? 0 : ((int)x0 >= XRES ? XRES - 1 : (int)x0);
			}
			for (int x = startx; x <= endx; ++x) {
				pixel(x, y, color);
			}
		}
		x0 += inv_slope0;
		x1 += inv_slope1;
	}
}

static inline void triangle_bottom(point_t p[3], uint32_t color) {
	float x0 = p[2].x;
	float x1 = p[2].x;
	float inv_slope0 = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	float inv_slope1 = (p[2].x - p[1].x) / (p[2].y - p[1].y);
	int starty = (int)p[2].y < 0 ? 0 : ((int)p[2].y >= YRES ? YRES - 1 : (int)p[2].y);
	int endy = (int)p[0].y < 0 ? 0 : ((int)p[0].y >= YRES ? YRES - 1 : (int)p[0].y);
	x0 -= inv_slope0 * abs(starty - (int)p[2].y);
	x1 -= inv_slope1 * abs(starty - (int)p[2].y);
	for (int y = starty; y > endy; --y) {
		if (y >= 0 && y < YRES) {
			int startx, endx;
			if (x0 < x1) {
				startx = (int)x0 < 0 ? 0 : ((int)x0 >= XRES ? XRES - 1 : (int)x0);
				endx = (int)x1 < 0 ? 0 : ((int)x1 >= XRES ? XRES - 1 : (int)x1);
			} else {
				startx = (int)x1 < 0 ? 0 : ((int)x1 >= XRES ? XRES - 1 : (int)x1);
				endx = (int)x0 < 0 ? 0 : ((int)x0 >= XRES ? XRES - 1 : (int)x0);
			}
			for (int x = startx; x <= endx; ++x) {
				pixel(x, y, color);
			}
		}
		x0 -= inv_slope0;
		x1 -= inv_slope1;
	}
}

static inline void sort_points(point_t p[3]) {
	int n = 3;
	do {
		int new_n = 0;
		for (int i = 1; i < n; ++i) {
			if (p[i - 1].y > p[i].y) {
				point_t t = p[i];
				p[i] = p[i - 1];
				p[i - 1] = t;
				new_n = i;
			}
		}
		n = new_n;
	} while (n > 1);
}

void draw_triangle(point_t p0, point_t p1, point_t p2, uint32_t color) {
	point_t p[3] = {p0, p1, p2};
	sort_points(p);
	if (p[1].y == p[2].y) {
		triangle_top(p, color);
	} else if (p[0].y == p[1].y) {
		triangle_bottom(p, color);
	} else {
		point_t p3;
		p3.x = p[0].x + ((p[1].y - p[0].y) / (p[2].y - p[0].y)) * (p[2].x - p[0].x);
		p3.y = p[1].y;
		point_t t0[3] = {p[0], p[1], p3};
		point_t t1[3] = {p[1], p3, p[2]};
		triangle_top(t0, color);
		triangle_bottom(t1, color);
	}
}