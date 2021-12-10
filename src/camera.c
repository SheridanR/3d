#include "vec4.h"
#include "matrix.h"
#include "camera.h"
#include "main.h"
#include <math.h>

static vec4_t project(
    const vec4_t* coords,
    const vec4_t* window
    ) {
    vec4_t result = *coords;
    div_vec4(&result, &result, &vec4(result.w));
    mul_vec4(&result, &result, &vec4(0.5f));
    add_vec4(&result, &result, &vec4(0.5f));
    result.x = result.x * window->z + window->x;
    result.y = result.y * window->w + window->y;
    return result;
}

vec4_t project_to_screen(const vec4_t* coords, const camera_t* camera) {
    vec4_t window = {0.f, 0.f, (float)XRES, (float)YRES};
    return project(coords, &window);
}

static mat4x4_t perspective(float fov, float aspect, float clip_near) {
    const float range = tanf(fov / 2.f);
    const float left = -range * aspect;
    const float right = range * aspect;
    const float bottom = -range;
    const float top = range;
    mat4x4_t result = mat4x4(0.f);
    result.x.x = 2.f / (right - left);
    result.y.y = 2.f / (top - bottom);
    result.z.z = -1.f;
    result.z.w = -1.f;
    result.w.z = -2.f * clip_near;
    return result;
}

static frustum_t setup_frustum(float fov, float aspect, float clip_near) {
    frustum_t frustum;
    float sh = sinf((fov / 2.f) * aspect);
    float sv = sinf((fov / 2.f) * aspect);
    float ch = cosf(fov / 2.f);
    float cv = cosf(fov / 2.f);
    frustum.planes[0] = (vec4_t){ch,  0.f,  sh, 0.f}; // left
    frustum.planes[1] = (vec4_t){-ch, 0.f,  sh, 0.f}; // right
    frustum.planes[2] = (vec4_t){0.f,  cv,  sv, 0.f}; // top
    frustum.planes[3] = (vec4_t){0.f, -cv,  sv, 0.f}; // bottom
    frustum.planes[4] = (vec4_t){0.f, 0.f, 1.f, -clip_near}; // near
    return frustum;
}

void camera_update(camera_t* camera) {
    const float fov = camera->fov;
    const float aspect = (float)XRES / (float)YRES;
    const float clip_near = (float)CAMERA_CLIP_NEAR;
    camera->proj = perspective(fov, aspect, clip_near);
    camera->frustum = setup_frustum(fov, aspect, clip_near);
    quat_t* q = &camera->ang;
    mat4x4_t rotate = quat_to_mat(&(quat_t){q->x, q->y, q->z, -q->w});
    mat4x4_t translate = mat4x4(1.f);
    pow_vec4(&translate.w, &camera->pos, -1.f);
    translate.w.w = 1.f;
    mul_mat(&camera->view, &rotate, &translate);
}
