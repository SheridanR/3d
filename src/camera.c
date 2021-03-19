#include "vec4.h"
#include "matrix.h"
#include "camera.h"
#include "main.h"
#include <math.h>

static vec4_t project(
	const vec4_t* world,
	const mat4x4_t* model,
	const mat4x4_t* projview,
	const vec4_t* window
	) {
	vec4_t result = *world; result.w = 1.f;
	mul_mat_vec4(&result, model, &vec4_copy(result));
	mul_mat_vec4(&result, projview, &vec4_copy(result));
	div_vec4(&result, &result, &vec4(result.w));
	mul_vec4(&result, &result, &vec4(0.5f));
	add_vec4(&result, &result, &vec4(0.5f));
	result.x = result.x * window->z + window->x;
	result.y = result.y * window->w + window->y;
	return result;
}

vec4_t world_to_screen_coords(const vec4_t* world_coords, const camera_t* camera) {
	vec4_t window = {0.f, 0.f, (float)XRES, (float)YRES};
	mat4x4_t model = mat4x4(1.f);
	mat4x4_t projview;
	mul_mat(&projview, &camera->proj, &camera->view);
	return project(world_coords, &model, &projview, &window);
}

static mat4x4_t perspective(float fov, float aspect, float clip_near) {
	const float range = tanf(fov / 2.f) * clip_near;
	const float left = -range * aspect;
	const float right = range * aspect;
	const float bottom = -range;
	const float top = range;
	mat4x4_t result = mat4x4(0.f);
	result.x.x = (2.f * clip_near) / (right - left);
	result.y.y = (2.f * clip_near) / (top - bottom);
	result.z.z = -1.f;
	result.z.w = -1.f;
	result.w.z = -2.f * clip_near;
	return result;
}

void camera_update(camera_t* camera) {
	camera->proj = perspective(camera->fov, (float)XRES / (float)YRES, (float)CAMERA_CLIP_NEAR);
	mat4x4_t rotate = quat_to_mat(&camera->ang);
	mat4x4_t translate = mat4x4(1.f);
	translate.w = camera->pos;
	translate.w.w = 1.f;
	mul_mat(&camera->view, &rotate, &translate);
}