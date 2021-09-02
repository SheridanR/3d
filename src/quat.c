#include "quat.h"
#include <math.h>

quat_t euler_to_quat(const float pitch, const float yaw, const float roll) {
    quat_t result;
    vec4_t c = {cosf(pitch * 0.5f), cosf(yaw * 0.5f), cosf(roll * 0.5f), 0.f};
    vec4_t s = {sinf(pitch * 0.5f), sinf(yaw * 0.5f), sinf(roll * 0.5f), 0.f};
    result.x = s.x * c.y * c.z - c.x * s.y * s.z;
    result.y = c.x * s.y * c.z + s.x * c.y * s.z;
    result.z = c.x * c.y * s.z - s.x * s.y * c.z;
    result.w = c.x * c.y * c.z + s.x * s.y * s.z;
    return result;
}

mat4x4_t quat_to_mat(const quat_t* restrict q) {
    mat4x4_t result;
    result.x.x = 1.f - 2.f * (q->y * q->y + q->z * q->z);
    result.x.y = 2.f * (q->x * q->y + q->w * q->z);
    result.x.z = 2.f * (q->x * q->z - q->w * q->y);
    result.x.w = 0.f;
    result.y.x = 2.f * (q->x * q->y - q->w * q->z);
    result.y.y = 1.f - 2.f * (q->x * q->x + q->z * q->z);
    result.y.z = 2.f * (q->y * q->z + q->w * q->x);
    result.y.w = 0.f;
    result.z.x = 2.f * (q->x * q->z + q->w * q->y);
    result.z.y = 2.f * (q->y * q->z - q->w * q->x);
    result.z.z = 1.f - 2.f * (q->x * q->x + q->y * q->y);
    result.z.w = 0.f;
    result.w.x = 0.f;
    result.w.y = 0.f;
    result.w.z = 0.f;
    result.w.w = 1.f;
    return result;
}

quat_t* mul_quat(quat_t* restrict result, const quat_t* restrict a, const quat_t* restrict b) {
    result->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    result->y = a->w * b->y + a->y * b->w + a->z * b->x - a->x * b->z;
    result->z = a->w * b->z + a->z * b->w + a->x * b->y - a->y * b->x;
    result->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    return result;
}

vec4_t quat_to_vec3(const quat_t* restrict q) {
    vec4_t v = {0.f, 0.f, -1.f, 0.f};
    vec4_t qv = {-q->x, -q->y, -q->z, -q->w};
    vec4_t t;
    cross_vec3(&t, &qv, &v);
    t.x *= 2.f; t.y *= 2.f; t.z *= 2.f; t.w = 0.f;
    vec4_t result;
    add_vec4(&result,
        add_vec4(&vec4(0.f), &v, pow_vec4(&vec4(0.f), &t, qv.w)),
        cross_vec3(&vec4(0.f), &qv, &t)
    );
    return result;
}