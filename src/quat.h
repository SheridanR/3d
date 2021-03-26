#ifndef QUAT_H
#define QUAT_H

#include "matrix.h"

typedef struct quat {
    float x;
    float y;
    float z;
    float w;
} quat_t;

#define quat() ((quat_t){0.f, 0.f, 0.f, 1.f})
#define quat_copy(q) ((quat_t){q.x, q.y, q.z, q.w})
extern inline quat_t euler_to_quat(float pitch, float yaw, float roll);
extern inline mat4x4_t quat_to_mat(const quat_t* q);
extern inline quat_t* mul_quat(quat_t* result, const quat_t* a, const quat_t* b);
extern inline vec4_t quat_to_vec3(const quat_t* q);

#endif