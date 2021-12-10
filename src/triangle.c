#include "main.h"
#include "triangle.h"
#include "draw.h"
#include "crap.h"
#include "mtl.h"
#include <stdlib.h>
#include <math.h>

static inline int dmin(int a, int b) { return a < b ? a : b; }

static inline int dmax(int a, int b) { return a > b ? a : b; }

static inline void triangle_span(
    int y, int x0, int x1, float z0, float z1,
    float r0, float r1, float g0, float g1, float b0, float b1,
    float u0, float u1, float v0, float v1, mtl_t* mtl
) {
    int startx = x0 < 0 ? 0 : (x0 > XRES ? XRES : x0);
    int endx = x1 <= 0 ? -1 : (x1 >= XRES ? XRES - 1 : x1);
    if (startx == XRES || endx == -1) {
        return;
    }
    const float x_diff = 1.f / (float)(x1 - x0);
    const float z_inc = (z1 - z0) * x_diff; const float r_inc = (r1 - r0) * x_diff;
    const float g_inc = (g1 - g0) * x_diff; const float b_inc = (b1 - b0) * x_diff;
    const float u_inc = (u1 - u0) * x_diff; const float v_inc = (v1 - v0) * x_diff;
    const int xoff = abs(startx - x0);
    float z = z0 + z_inc * xoff; float r = r0 + r_inc * xoff;
    float g = g0 + g_inc * xoff; float b = b0 + b_inc * xoff;
    float u = u0 + u_inc * xoff; float v = v0 + v_inc * xoff;
    if (mtl && mtl->diffuse_texture.pixels) {
        for (int x = startx; x <= endx; ++x) {
            const float recip = 1.f / z;
            if (check_depth(x, y, recip)) {
                const texture_t* t = &mtl->diffuse_texture;
                const int ui = dmin(dmax(0, (u / z) * t->width), t->width);
                const int vi = dmin(dmax(0, (1.f - (v / z)) * t->height), t->height);
                uint32_t c = t->pixels[vi * t->width + ui];
                pixel(x, y, color(((c & t->mask[0]) >> t->shift[0]) * r, ((c & t->mask[1]) >> t->shift[1]) * g, ((c & t->mask[2]) >> t->shift[2]) * b, 255));
                write_depth(x, y, recip);
            }
            z += z_inc; r += r_inc; g += g_inc;
            b += b_inc; u += u_inc; v += v_inc;
        }
    } else {
        for (int x = startx; x <= endx; ++x) {
            if (check_depth(x, y, z)) {
                pixel(x, y, color(r * 255.f, g * 255.f, b * 255.f, 255));
                write_depth(x, y, z);
            }
            z += z_inc; r += r_inc; g += g_inc; b += b_inc;
        }
    }
}

static inline void triangle_half(
    const vertex_t v[3], int mode,
    const int b[4], mtl_t* mtl
    ) {
    const int last_scan = 1 - mode;
    int topy = (int)v[0].y < 0 ? 0 : ((int)v[0].y > YRES ? YRES : (int)v[0].y);
    int boty = (int)v[2].y <= 0 ? -1 : ((int)v[2].y >= YRES ? YRES - 1 : (int)v[2].y);
    if (topy == YRES || boty == -1) {
        return;
    }
    const float x_min = fmin(v[0].x, fmin(v[1].x, v[2].x)); const float x_max = fmax(v[0].x, fmax(v[1].x, v[2].x));
    const float z_min = fmin(v[0].z, fmin(v[1].z, v[2].z)); const float z_max = fmax(v[0].z, fmax(v[1].z, v[2].z));
    const float r_min = fmin(v[0].r, fmin(v[1].r, v[2].r)); const float r_max = fmax(v[0].r, fmax(v[1].r, v[2].r));
    const float g_min = fmin(v[0].g, fmin(v[1].g, v[2].g)); const float g_max = fmax(v[0].g, fmax(v[1].g, v[2].g));
    const float b_min = fmin(v[0].b, fmin(v[1].b, v[2].b)); const float b_max = fmax(v[0].b, fmax(v[1].b, v[2].b));
    const float u_min = fmin(v[0].u, fmin(v[1].u, v[2].u)); const float u_max = fmax(v[0].u, fmax(v[1].u, v[2].u));
    const float v_min = fmin(v[0].v, fmin(v[1].v, v[2].v)); const float v_max = fmax(v[0].v, fmax(v[1].v, v[2].v));
    const float y_diff0 = 1.f / (v[b[0]].y - v[b[1]].y) * mode;
    const float y_diff1 = 1.f / (v[b[2]].y - v[b[3]].y) * mode;
    const float x_inc0 = (v[b[0]].x - v[b[1]].x) * y_diff0; const float x_inc1 = (v[b[2]].x - v[b[3]].x) * y_diff1;
    const float z_inc0 = (v[b[0]].z - v[b[1]].z) * y_diff0; const float z_inc1 = (v[b[2]].z - v[b[3]].z) * y_diff1;
    const float r_inc0 = (v[b[0]].r - v[b[1]].r) * y_diff0; const float r_inc1 = (v[b[2]].r - v[b[3]].r) * y_diff1;
    const float g_inc0 = (v[b[0]].g - v[b[1]].g) * y_diff0; const float g_inc1 = (v[b[2]].g - v[b[3]].g) * y_diff1;
    const float b_inc0 = (v[b[0]].b - v[b[1]].b) * y_diff0; const float b_inc1 = (v[b[2]].b - v[b[3]].b) * y_diff1;
    const float u_inc0 = (v[b[0]].u - v[b[1]].u) * y_diff0; const float u_inc1 = (v[b[2]].u - v[b[3]].u) * y_diff1;
    const float v_inc0 = (v[b[0]].v - v[b[1]].v) * y_diff0; const float v_inc1 = (v[b[2]].v - v[b[3]].v) * y_diff1;
    const int yoff = mode > 0 ? abs(topy - (int)v[0].y) : abs(boty - (int)v[2].y);
    float x0 = v[last_scan].x + x_inc0 * yoff; float x1 = v[last_scan].x + x_inc1 * yoff;
    float z0 = v[last_scan].z + z_inc0 * yoff; float z1 = v[last_scan].z + z_inc1 * yoff;
    float r0 = v[last_scan].r + r_inc0 * yoff; float r1 = v[last_scan].r + r_inc1 * yoff;
    float g0 = v[last_scan].g + g_inc0 * yoff; float g1 = v[last_scan].g + g_inc1 * yoff;
    float b0 = v[last_scan].b + b_inc0 * yoff; float b1 = v[last_scan].b + b_inc1 * yoff;
    float u0 = v[last_scan].u + u_inc0 * yoff; float u1 = v[last_scan].u + u_inc1 * yoff;
    float v0 = v[last_scan].v + v_inc0 * yoff; float v1 = v[last_scan].v + v_inc1 * yoff;
    const int starty = mode > 0 ? topy : boty;
    const int endy = mode > 0 ? boty : topy;
#define BOUND(x0, x1, z0, z1, r0, r1, g0, g1, b0, b1, u0, u1, v0, v1)\
        int bx0 = (int)fmax(x0, x_min); int bx1 = (int)fmin(x1, x_max);\
        float bz0 = fmax(z0, z_min); float bz1 = fmin(z1, z_max);\
        float br0 = fmax(r0, r_min); float br1 = fmin(r1, r_max);\
        float bg0 = fmax(g0, g_min); float bg1 = fmin(g1, g_max);\
        float bb0 = fmax(b0, b_min); float bb1 = fmin(b1, b_max);\
        float bu0 = fmax(u0, u_min); float bu1 = fmin(u1, u_max);\
        float bv0 = fmax(v0, v_min); float bv1 = fmin(v1, v_max);
#define SPAN()\
        triangle_span(y, bx0, bx1, bz0, bz1, br0, br1, bg0, bg1, bb0, bb1, bu0, bu1, bv0, bv1, mtl);\
        x0 += x_inc0; x1 += x_inc1; z0 += z_inc0; z1 += z_inc1;\
        r0 += r_inc0; r1 += r_inc1; g0 += g_inc0; g1 += g_inc1;\
        b0 += b_inc0; b1 += b_inc1; u0 += u_inc0; u1 += u_inc1;\
        v0 += v_inc0; v1 += v_inc1;
    if (x_inc0 < x_inc1) {
        for (int y = starty; mode > 0 ? (y <= endy) : (y >= endy); y += mode) {
            BOUND(x0, x1, z0, z1, r0, r1, g0, g1, b0, b1, u0, u1, v0, v1)
            SPAN()
        }
    } else {
        for (int y = starty; mode > 0 ? (y <= endy) : (y >= endy); y += mode) {
            BOUND(x1, x0, z1, z0, r1, r0, g1, g0, b1, b0, u1, u0, v1, v0)
            SPAN()
        }
    }
}

static inline void swap(vertex_t* v0, vertex_t* v1) {
    vertex_t t = *v0;
    *v0 = *v1;
    *v1 = t;
}

#define TRIANGLE_TOP(P) triangle_half(P, 1, (int[4]){1, 0, 2, 0}, mtl)
#define TRIANGLE_BOT(P) triangle_half(P, -1, (int[4]){2, 0, 2, 1}, mtl)

void perspective_correct(vertex_t* v) {
    for (int c = 0; c < 3; ++c) {
        const float z = v[c].z;
        v[c].u = v[c].u / z;
        v[c].v = v[c].v / z;
        v[c].z = 1.f / z;
    }
}

void draw_triangle(vertex_t v[3], mtl_t* mtl) {
    perspective_correct(v);
    if (v[0].y > v[1].y) { swap(&v[0], &v[1]); }
    if (v[1].y > v[2].y) { swap(&v[1], &v[2]); }
    if (v[0].y > v[1].y) { swap(&v[0], &v[1]); }
    if (v[1].y == v[2].y) {
        TRIANGLE_TOP(v);
    } else if (v[0].y == v[1].y) {
        TRIANGLE_BOT(v);
    } else {
        vertex_t v3;
        const float factor = (v[1].y - v[0].y) / (v[2].y - v[0].y);
        v3.x = v[0].x + factor * (v[2].x - v[0].x);
        v3.y = v[1].y;
        v3.z = v[0].z + factor * (v[2].z - v[0].z);
        v3.r = v[0].r + factor * (v[2].r - v[0].r);
        v3.g = v[0].g + factor * (v[2].g - v[0].g);
        v3.b = v[0].b + factor * (v[2].b - v[0].b);
        v3.u = v[0].u + factor * (v[2].u - v[0].u);
        v3.v = v[0].v + factor * (v[2].v - v[0].v);
        vertex_t t0[3] = {v[0], v[1], v3};
        vertex_t t1[3] = {v[1], v3, v[2]};
        TRIANGLE_TOP(t0);
        TRIANGLE_BOT(t1);
    }
}
