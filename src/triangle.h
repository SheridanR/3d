#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec4.h"
#include <stdint.h>

typedef struct vertex_t {
    vec4_t pos;
    vec4_t color;
    vec4_t uv;
} vertex_t;

typedef struct mtl mtl_t;

void draw_triangle(vertex_t v[3], mtl_t* mtl);

#endif
