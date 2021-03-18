#ifndef MATRIX_H
#define MATRIX_H

#include "vec4.h"

typedef struct mat4x4 {
	vec4_t x;
	vec4_t y;
	vec4_t z;
	vec4_t w;
} mat4x4_t;

#define mat4x4(F) ((mat4x4_t) {\
	F, 0.f, 0.f, 0.f,\
	0.f, F, 0.f, 0.f,\
	0.f, 0.f, F, 0.f,\
	0.f, 0.f, 0.f, F,\
})
extern inline mat4x4_t* mul_mat(mat4x4_t* result, const mat4x4_t* m1, const mat4x4_t* m2);
extern inline vec4_t* mul_mat_vec4(vec4_t* result, const mat4x4_t* m, const vec4_t* v);

#endif