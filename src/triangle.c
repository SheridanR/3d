#include "main.h"
#include "triangle.h"
#include "draw.h"
#include "crap.h"
#include <stdlib.h>
#include <math.h>

static inline void triangle_span(
	int y, int x0, int x1, float z0, float z1,
	float r0, float r1, float g0, float g1, float b0, float b1
) {
	int startx = x0 < 0 ? 0 : (x0 > XRES ? XRES : x0);
	int endx = x1 <= 0 ? -1 : (x1 >= XRES ? XRES - 1 : x1);
	if (startx == XRES || endx == -1) {
		return;
	}
	const float x_diff = 1.f / (float)(x1 - x0);
	const float z_inc = (z1 - z0) * x_diff;
	const float r_inc = (r1 - r0) * x_diff;
	const float g_inc = (g1 - g0) * x_diff;
	const float b_inc = (b1 - b0) * x_diff;
	const int xoff = abs(startx - x0);
	float z = z0 + z_inc * xoff;
	float r = r0 + r_inc * xoff;
	float g = g0 + g_inc * xoff;
	float b = b0 + b_inc * xoff;
	for (int x = startx; x <= endx; ++x) {
		if (check_depth(x, y, z)) {
			pixel(x, y, color(r * 255.f, g * 255.f, b * 255.f, 255));
			write_depth(x, y, z);
		}
		z += z_inc;
		r += r_inc;
		g += g_inc;
		b += b_inc;
	}
}

static inline void triangle_half(
	const point_t p[3], int mode,
	int a, const int b[4], int c
	) {
	int starty = (int)p[0].y < 0 ? 0 : ((int)p[0].y > YRES ? YRES : (int)p[0].y);
	int endy = (int)p[a].y <= 0 ? -1 : ((int)p[a].y >= YRES ? YRES - 1 : (int)p[a].y);
	if (starty == YRES || endy == -1) {
		return;
	}
	const float x_min = fmin(p[0].x, fmin(p[1].x, p[2].x));
	const float x_max = fmax(p[0].x, fmax(p[1].x, p[2].x));
	const float z_min = fmin(p[0].z, fmin(p[1].z, p[2].z));
	const float z_max = fmax(p[0].z, fmax(p[1].z, p[2].z));
	const float r_min = fmin(p[0].r, fmin(p[1].r, p[2].r));
	const float r_max = fmax(p[0].r, fmax(p[1].r, p[2].r));
	const float g_min = fmin(p[0].g, fmin(p[1].g, p[2].g));
	const float g_max = fmax(p[0].g, fmax(p[1].g, p[2].g));
	const float b_min = fmin(p[0].b, fmin(p[1].b, p[2].b));
	const float b_max = fmax(p[0].b, fmax(p[1].b, p[2].b));
	const float y_diff0 = 1.f / (p[b[0]].y - p[b[1]].y) * mode;
	const float y_diff1 = 1.f / (p[b[2]].y - p[b[3]].y) * mode;
	const float x_inc0 = (p[b[0]].x - p[b[1]].x) * y_diff0;
	const float x_inc1 = (p[b[2]].x - p[b[3]].x) * y_diff1;
	const float z_inc0 = (p[b[0]].z - p[b[1]].z) * y_diff0;
	const float z_inc1 = (p[b[2]].z - p[b[3]].z) * y_diff1;
	const float r_inc0 = (p[b[0]].r - p[b[1]].r) * y_diff0;
	const float r_inc1 = (p[b[2]].r - p[b[3]].r) * y_diff1;
	const float g_inc0 = (p[b[0]].g - p[b[1]].g) * y_diff0;
	const float g_inc1 = (p[b[2]].g - p[b[3]].g) * y_diff1;
	const float b_inc0 = (p[b[0]].b - p[b[1]].b) * y_diff0;
	const float b_inc1 = (p[b[2]].b - p[b[3]].b) * y_diff1;
	const int yoff = mode > 0 ? abs(starty - (int)p[0].y) : abs(endy - (int)p[2].y);
	float x0 = p[c].x + x_inc0 * yoff; float x1 = p[c].x + x_inc1 * yoff;
	float z0 = p[c].z + z_inc0 * yoff; float z1 = p[c].z + z_inc1 * yoff;
	float r0 = p[c].r + r_inc0 * yoff; float r1 = p[c].r + r_inc1 * yoff;
	float g0 = p[c].g + g_inc0 * yoff; float g1 = p[c].g + g_inc1 * yoff;
	float b0 = p[c].b + b_inc0 * yoff; float b1 = p[c].b + b_inc1 * yoff;
	int begin = mode > 0 ? starty : endy;
	int end = mode > 0 ? endy : starty;
	if (x_inc0 < x_inc1) {
		for (int y = begin; mode > 0 ? (y <= end) : (y >= end); y += mode) {
			int bx0 = (int)fmax(x0, x_min);
			int bx1 = (int)fmin(x1, x_max);
			float bz0 = fmax(z0, z_min);
			float bz1 = fmin(z1, z_max);
			float br0 = fmax(r0, r_min);
			float br1 = fmin(r1, r_max);
			float bg0 = fmax(g0, g_min);
			float bg1 = fmin(g1, g_max);
			float bb0 = fmax(b0, b_min);
			float bb1 = fmin(b1, b_max);
			triangle_span(y, bx0, bx1, bz0, bz1,
				br0, br1, bg0, bg1, bb0, bb1);
			x0 += x_inc0; x1 += x_inc1;
			z0 += z_inc0; z1 += z_inc1;
			r0 += r_inc0; r1 += r_inc1;
			g0 += g_inc0; g1 += g_inc1;
			b0 += b_inc0; b1 += b_inc1;
		}
	} else {
		for (int y = begin; mode > 0 ? (y <= end) : (y >= end); y += mode) {
			int bx0 = (int)fmax(x1, x_min);
			int bx1 = (int)fmin(x0, x_max);
			float bz0 = fmax(z1, z_min);
			float bz1 = fmin(z0, z_max);
			float br0 = fmax(r1, r_min);
			float br1 = fmin(r0, r_max);
			float bg0 = fmax(g1, g_min);
			float bg1 = fmin(g0, g_max);
			float bb0 = fmax(b1, b_min);
			float bb1 = fmin(b0, b_max);
			triangle_span(y, bx0, bx1, bz0, bz1,
				br0, br1, bg0, bg1, bb0, bb1);
			x0 += x_inc0; x1 += x_inc1;
			z0 += z_inc0; z1 += z_inc1;
			r0 += r_inc0; r1 += r_inc1;
			g0 += g_inc0; g1 += g_inc1;
			b0 += b_inc0; b1 += b_inc1;
		}
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

#define TRIANGLE_TOP(P) triangle_half(P, 1, 1, (int[4]){1, 0, 2, 0}, 0)
#define TRIANGLE_BOT(P) triangle_half(P, -1, 2, (int[4]){2, 0, 2, 1}, 2)

void draw_triangle(point_t p0, point_t p1, point_t p2) {
	point_t p[3] = {p0, p1, p2};
	sort_points(p);
	if (p[1].y == p[2].y) {
		TRIANGLE_TOP(p);
	} else if (p[0].y == p[1].y) {
		TRIANGLE_BOT(p);
	} else {
		point_t p3;
		const float factor = (p[1].y - p[0].y) / (p[2].y - p[0].y);
		p3.x = p[0].x + factor * (p[2].x - p[0].x);
		p3.y = p[1].y;
		p3.z = p[0].z + factor * (p[2].z - p[0].z);
		p3.r = p[0].r + factor * (p[2].r - p[0].r);
		p3.g = p[0].g + factor * (p[2].g - p[0].g);
		p3.b = p[0].b + factor * (p[2].b - p[0].b);
		point_t t0[3] = {p[0], p[1], p3};
		point_t t1[3] = {p[1], p3, p[2]};
		TRIANGLE_TOP(t0);
		TRIANGLE_BOT(t1);
	}
}