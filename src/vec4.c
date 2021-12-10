#include "vec4.h"
#include <math.h>

vec4_t* add_vec4(vec4_t* restrict result, const vec4_t* restrict a, const vec4_t* restrict b) {
    result->x = a->x + b->x; result->y = a->y + b->y;
    result->z = a->z + b->z; result->w = a->w + b->w;
    return result;
}

vec4_t* sub_vec4(vec4_t* restrict result, const vec4_t* restrict a, const vec4_t* restrict b) {
    result->x = a->x - b->x; result->y = a->y - b->y;
    result->z = a->z - b->z; result->w = a->w - b->w;
    return result;
}

vec4_t* mul_vec4(vec4_t* restrict result, const vec4_t* restrict a, const vec4_t* restrict b) {
    result->x = a->x * b->x; result->y = a->y * b->y;
    result->z = a->z * b->z; result->w = a->w * b->w;
    return result;
}

vec4_t* div_vec4(vec4_t* restrict result, const vec4_t* restrict a, const vec4_t* restrict b) {
    result->x = a->x / b->x; result->y = a->y / b->y;
    result->z = a->z / b->z; result->w = a->w / b->w;
    return result;
}

vec4_t* pow_vec4(vec4_t* restrict result, const vec4_t* restrict v, const float f) {
    result->x = v->x * f; result->y = v->y * f;
    result->z = v->z * f; result->w = v->w * f;
    return result;
}

float dot_vec4(const vec4_t* restrict a, const vec4_t* restrict b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

vec4_t* cross_vec3(vec4_t* restrict result, const vec4_t* restrict a, const vec4_t* restrict b) {
    result->x = a->y * b->z - a->z * b->y; result->y = a->z * b->x - a->x * b->z; result->z = a->x * b->y - a->y * b->x;
    return result;
}

vec4_t* cross_vec4(vec4_t* restrict result, const vec4_t* restrict a, const vec4_t* restrict b) {
    result->x = a->y * b->z - a->z * b->y; result->y = a->z * b->w - a->w * b->z;
    result->z = a->w * b->x - a->x * b->w; result->w = a->x * b->y - a->y * b->x;
    return result;
}

float length_vec4(const vec4_t* restrict v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

vec4_t* normal_vec4(vec4_t* restrict result, const vec4_t* restrict v) {
    float length = length_vec4(v);
    result->x = v->x / length; result->y = v->y / length;
    result->z = v->z / length; result->w = v->w / length;
    return result;
}
