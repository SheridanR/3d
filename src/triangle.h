#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

typedef struct vertex_t {
    float x; float y; float z;
    float r; float g; float b;
    float u; float v;
} vertex_t;

typedef struct mtl mtl_t;

void draw_triangle(vertex_t v[3], mtl_t* mtl);

#endif
