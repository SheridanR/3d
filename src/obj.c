#include "obj.h"
#include "triangle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

obj_t* load_obj(obj_t* result, const char* filename) {
	char buf[1024];
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] != '#' && buf[0] != '\n') {
			char* type = strtok(buf, " ");
			if (strcmp(type, "v") == 0) {
				++result->num_positions;
			}
			else if (strcmp(type, "f") == 0) {
				result->num_indices += 3;
			}
		}
	}
	rewind(fp);
	result->positions = (vec4_t*)malloc(result->num_positions * sizeof(vec4_t));
	result->indices = (uint32_t*)malloc(result->num_indices * sizeof(uint32_t));
	size_t indices_count = 0, positions_count = 0;
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] != '#' && buf[0] != '\n') {
			char* type = strtok(buf, " ");
			if (strcmp(type, "v") == 0) {
				char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
				char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
				char* c3 = strtok(NULL, " "); float f3 = strtof(c3, NULL);
				result->positions[positions_count++] = (vec4_t){f1, f2, f3, 0.f};
			}
			else if (strcmp(type, "f") == 0) {
				char* token;
				while (token = strtok(NULL, " ")) {
					uint32_t index = (uint32_t)strtol(token, NULL, 10) - 1;
					result->indices[indices_count++] = index;
				}
			}
		}
	}
	fclose(fp);
	return NULL;
}

void free_obj(obj_t* obj) {
	free(obj->positions);
	free(obj->indices);
}

void draw_obj(const obj_t* obj, const camera_t* camera) {
	vec4_t camera_dir = quat_to_vec3(&camera->ang);
	for (int c = 0; c < obj->num_indices; c += 3) {
		const vec4_t* p[3] = {
			&obj->positions[obj->indices[c]],
			&obj->positions[obj->indices[c + 1]],
			&obj->positions[obj->indices[c + 2]]
		};
		vec4_t normal = vec4(0.f);
		cross_vec3(&normal, sub_vec4(&vec4(0.f), p[1], p[0]), sub_vec4(&vec4(0.f), p[2], p[0]));
		normal_vec4(&normal, &vec4_copy(normal));
		float r[3], g[3], b[3];
		for (int i = 0; i < 3; ++i) {
			vec4_t diff = vec4(0.f);
			sub_vec4(&diff, p[i], &camera->pos);
			normal_vec4(&diff, &vec4_copy(diff));
			if (dot_vec4(&diff, &camera_dir) <= 0.f) {
				goto next; // cull triangles that would be projected backwards
			}
			const float dir = dot_vec4(&diff, &normal);
			if (dir > 0.f) {
				goto next; // backface culling
			}
			r[i] = -dir;
			g[i] = -dir;
			b[i] = -dir;
		}
		vec4_t sp[3];
		sp[0] = world_to_screen_coords(p[0], camera);
		sp[1] = world_to_screen_coords(p[1], camera);
		sp[2] = world_to_screen_coords(p[2], camera);
		point_t t[3];
		t[0] = (point_t){sp[0].x, sp[0].y, sp[0].z, r[0], g[0], b[0]};
		t[1] = (point_t){sp[1].x, sp[1].y, sp[1].z, r[1], g[1], b[1]};
		t[2] = (point_t){sp[2].x, sp[2].y, sp[2].z, r[2], g[2], b[2]};
		draw_triangle(t[0], t[1], t[2]);
next:
		continue;
	}
}