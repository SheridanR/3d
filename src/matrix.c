#include "matrix.h"
#include "vec4.h"

mat4x4_t* mul_mat(mat4x4_t* result, const mat4x4_t* m1, const mat4x4_t* m2) {
    (void)add_vec4(
        &result->x,
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->x, m2->x.x), pow_vec4(&vec4(0.f), &m1->y, m2->x.y)),
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->z, m2->x.z), pow_vec4(&vec4(0.f), &m1->w, m2->x.w))
    );
    (void)add_vec4(
        &result->y,
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->x, m2->y.x), pow_vec4(&vec4(0.f), &m1->y, m2->y.y)),
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->z, m2->y.z), pow_vec4(&vec4(0.f), &m1->w, m2->y.w))
    );
    (void)add_vec4(
        &result->z,
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->x, m2->z.x), pow_vec4(&vec4(0.f), &m1->y, m2->z.y)),
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->z, m2->z.z), pow_vec4(&vec4(0.f), &m1->w, m2->z.w))
    );
    (void)add_vec4(
        &result->w,
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->x, m2->w.x), pow_vec4(&vec4(0.f), &m1->y, m2->w.y)),
        add_vec4(&vec4(0.f), pow_vec4(&vec4(0.f), &m1->z, m2->w.z), pow_vec4(&vec4(0.f), &m1->w, m2->w.w))
    );
    return result;
}

vec4_t* mul_mat_vec4(vec4_t* result, const mat4x4_t* m, const vec4_t* v) {
    result->x = m->x.x * v->x + m->y.x * v->y + m->z.x * v->z + m->w.x * v->w;
    result->y = m->x.y * v->x + m->y.y * v->y + m->z.y * v->z + m->w.y * v->w;
    result->z = m->x.z * v->x + m->y.z * v->y + m->z.z * v->z + m->w.z * v->w;
    result->w = m->x.w * v->x + m->y.w * v->y + m->z.w * v->z + m->w.w * v->w;
    return result;
}