#include "obj.h"
#include "triangle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

obj_t* load_obj(obj_t* result, const char* filename) {
	char buf[1024];
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		result->valid = false;
		return NULL;
	}
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] != '#' && buf[0] != '\n') {
			char* type = strtok(buf, " ");
			if (strcmp(type, "v") == 0) {
				++result->num_positions;
			}
			else if (strcmp(type, "vt") == 0) {
				++result->num_texture_coords;
			}
			else if (strcmp(type, "vn") == 0) {
				++result->num_normals;
			}
			else if (strcmp(type, "f") == 0) {
				result->num_position_indices += 3;
				result->num_texture_coord_indices += 3;
				result->num_normal_indices += 3;
			}
		}
	}
	rewind(fp);
	result->positions = (vec4_t*)malloc(result->num_positions * sizeof(vec4_t));
	result->position_indices = (uint32_t*)malloc(result->num_position_indices * sizeof(uint32_t));
	result->texture_coords = (vec4_t*)malloc(result->num_texture_coords * sizeof(vec4_t));
	result->texture_coord_indices = (uint32_t*)malloc(result->num_texture_coord_indices * sizeof(uint32_t));
	result->normals = (vec4_t*)malloc(result->num_normals * sizeof(vec4_t));
	result->normal_indices = (uint32_t*)malloc(result->num_normal_indices * sizeof(uint32_t));
	size_t position_indices_count = 0, position_count = 0;
	size_t texture_coord_indices_count = 0, texture_coord_count = 0;
	size_t normal_indices_count = 0, normal_count = 0;
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] != '#' && buf[0] != '\n') {
			char* type = strtok(buf, " ");
			if (strcmp(type, "v") == 0) {
				char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
				char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
				char* c3 = strtok(NULL, " "); float f3 = strtof(c3, NULL);
				result->positions[position_count++] = (vec4_t){f1, f2, f3, 0.f};
			}
			else if (strcmp(type, "vt") == 0) {
				char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
				char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
				result->texture_coords[texture_coord_count++] = (vec4_t){f1, f2, 0.f, 0.f};
			}
			else if (strcmp(type, "vn") == 0) {
				char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
				char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
				char* c3 = strtok(NULL, " "); float f3 = strtof(c3, NULL);
				result->normals[normal_count++] = (vec4_t){f1, f2, f3, 0.f};
			}
			else if (strcmp(type, "f") == 0) {
				char *token, *token2, *token3;
				while ((token = strtok(NULL, " "))) {
					uint32_t position_index = (uint32_t)strtol(token, NULL, 10) - 1;
					result->position_indices[position_indices_count++] = position_index;
					if ((token2 = strchr(token, '/'))) {
						++token2;
						uint32_t texture_coord_index = (uint32_t)strtol(token2, NULL, 10) - 1;
						result->texture_coord_indices[texture_coord_indices_count++] = texture_coord_index;
						if ((token3 = strchr(token2, '/'))) {
							++token3;
							uint32_t normal_index = (uint32_t)strtol(token3, NULL, 10) - 1;
							result->normal_indices[normal_indices_count++] = normal_index;
						}
					}
				}
			}
		}
	}
	fclose(fp);
	result->valid = true;
	return result;
}

void free_obj(obj_t* obj) {
	free(obj->positions);
	free(obj->position_indices);
	free(obj->texture_coords);
	free(obj->texture_coord_indices);
	free(obj->normals);
	free(obj->normal_indices);
}

void draw_obj(const obj_t* obj, const camera_t* camera) {
	vec4_t camera_dir = quat_to_vec3(&camera->ang);
	for (int c = 0; c < obj->num_position_indices - 2; c += 3) {
		const vec4_t* p[3] = {
			&obj->positions[obj->position_indices[c]],
			&obj->positions[obj->position_indices[c + 1]],
			&obj->positions[obj->position_indices[c + 2]]
		};
		const vec4_t* t[3] = {
			&obj->texture_coords[obj->texture_coord_indices[c]],
			&obj->texture_coords[obj->texture_coord_indices[c + 1]],
			&obj->texture_coords[obj->texture_coord_indices[c + 2]]
		};
		const vec4_t* n[3] = {
			&obj->normals[obj->normal_indices[c]],
			&obj->normals[obj->normal_indices[c + 1]],
			&obj->normals[obj->normal_indices[c + 2]]
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
			if (dot_vec4(&diff, &normal) > 0.f) {
				goto next; // backface culling
			}
			const float dir = dot_vec4(&diff, n[i]);
			r[i] = fmin(fmax(0.f, -dir), 1.f);
			g[i] = fmin(fmax(0.f, -dir), 1.f);
			b[i] = fmin(fmax(0.f, -dir), 1.f);
		}
		vec4_t sp[3];
		sp[0] = world_to_screen_coords(p[0], camera);
		sp[1] = world_to_screen_coords(p[1], camera);
		sp[2] = world_to_screen_coords(p[2], camera);
		point_t triangle[3];
		triangle[0] = (point_t){sp[0].x, sp[0].y, sp[0].z, r[0], g[0], b[0]};
		triangle[1] = (point_t){sp[1].x, sp[1].y, sp[1].z, r[1], g[1], b[1]};
		triangle[2] = (point_t){sp[2].x, sp[2].y, sp[2].z, r[2], g[2], b[2]};
		draw_triangle(triangle[0], triangle[1], triangle[2]);
next:
		continue;
	}
}
