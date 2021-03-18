#ifndef VEC4_H
#define VEC4_H

typedef struct vec4 {
	float x;
	float y;
	float z;
	float w;
} vec4_t;

#define vec4(F) ((vec4_t){F, F, F, F})
extern inline vec4_t* add_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* mul_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* div_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* pow_vec4(vec4_t* result, const vec4_t* v, float f);

#endif