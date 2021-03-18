#include "vec4.h"

vec4_t* add_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b) {
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	result->z = a->z + b->z;
	result->w = a->w + b->w;
	return result;
}

vec4_t* sub_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b) {
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	result->z = a->z - b->z;
	result->w = a->w - b->w;
	return result;
}

vec4_t* mul_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b) {
	result->x = a->x * b->x;
	result->y = a->y * b->y;
	result->z = a->z * b->z;
	result->w = a->w * b->w;
	return result;
}

vec4_t* div_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b) {
	result->x = a->x / b->x;
	result->y = a->y / b->y;
	result->z = a->z / b->z;
	result->w = a->w / b->w;
	return result;
}

vec4_t* pow_vec4(vec4_t* result, const vec4_t* v, float f) {
	result->x = v->x * f;
	result->y = v->y * f;
	result->z = v->z * f;
	result->w = v->w * f;
	return result;
}

float dot_vec4(const vec4_t* a, const vec4_t* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}