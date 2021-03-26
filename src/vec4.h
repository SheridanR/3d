#ifndef VEC4_H
#define VEC4_H

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

#define vec4(F) ((vec4_t){F, F, F, F})
#define vec4_copy(V) ((vec4_t){V.x, V.y, V.z, V.w})
extern inline vec4_t* add_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* sub_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* mul_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* div_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* pow_vec4(vec4_t* result, const vec4_t* v, float f);
extern inline vec4_t* cross_vec3(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline vec4_t* cross_vec4(vec4_t* result, const vec4_t* a, const vec4_t* b);
extern inline float dot_vec4(const vec4_t* a, const vec4_t* b);
extern inline float length_vec4(const vec4_t* v);
extern inline vec4_t* normal_vec4(vec4_t* result, const vec4_t* v);

#endif