#include "obj.h"
#include "triangle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

obj_t* load_obj(obj_t* result, const char* filename) {
    char buf[1024];
    memset(result, 0, sizeof(obj_t));
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        result->valid = false;
        return NULL;
    }
    uint32_t material_index = 0;
    while (fgets(buf, sizeof(buf), fp)) {
        if (buf[0] != '#' && buf[0] != '\n') {
            char* type = strtok(buf, " ");
            if (strcmp(type, "v") == 0) {
                ++result->num_coords[VB_POSITION];
            }
            else if (strcmp(type, "vt") == 0) {
                ++result->num_coords[VB_TEXTURE];
            }
            else if (strcmp(type, "vn") == 0) {
                ++result->num_coords[VB_NORMAL];
            }
            else if (strcmp(type, "f") == 0) {
                char *token;
                while ((token = strtok(NULL, " "))) {
                    for (int c = 0; c < VB_NUM; ++c) {
                        result->num_indices[c] += 3;
                    }
                }
                for (int c = 0; c < VB_NUM; ++c) {
                    result->num_indices[c] -= 6;
                }
            }
            else if (strcmp(type, "mtllib") == 0) {
                char buf[128] = {'\0'};
                char* prefix = strrchr(filename, '/') ? strrchr(filename, '/') : strrchr(filename, '\\');
                strncpy(buf, filename, (size_t)(prefix - filename + 1));
                char* name = strtok(NULL, " \r\n");
                strcat(buf, name);
                (void)load_mtllib(&result->mtllib, buf);
            }
            else if (strcmp(type, "usemtl") == 0) {
                char buf[128] = {'\0'};
                char* name = type + strlen(type) + 1;
                char* newline = strrchr(name, '\r') ? strrchr(name, '\r') : strrchr(name, '\n');
                strncpy(buf, name, (size_t)(newline - name));
                for (int c = 0; c < MATERIAL_LIMIT; ++c) {
                    mtl_t* mtl = &result->mtllib.materials[c];
                    if (strcmp(mtl->filename, buf) == 0) {
                        result->mtl_indices[material_index] = result->num_indices[VB_POSITION];
                        result->mtl[material_index++] = mtl;
                        break;
                    }
                }
            }
        }
    }
    rewind(fp);
    for (int c = 0; c < VB_NUM; ++c) {
        result->coords[c] = (vec4_t*)malloc(result->num_coords[c] * sizeof(vec4_t));
        result->indices[c] = (uint32_t*)malloc(result->num_indices[c] * sizeof(uint32_t));
    }
    uint32_t indices_count[VB_NUM] = {0}, coords_count[VB_NUM] = {0};
    while (fgets(buf, sizeof(buf), fp)) {
        if (buf[0] != '#' && buf[0] != '\n') {
            char* type = strtok(buf, " ");
            if (strcmp(type, "v") == 0) {
                char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
                char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
                char* c3 = strtok(NULL, " "); float f3 = strtof(c3, NULL);
                result->coords[VB_POSITION][coords_count[VB_POSITION]++] = (vec4_t){f1, f2, f3, 1.f};
            }
            else if (strcmp(type, "vt") == 0) {
                char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
                char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
                result->coords[VB_TEXTURE][coords_count[VB_TEXTURE]++] = (vec4_t){f1, f2, 0.f, 0.f};
            }
            else if (strcmp(type, "vn") == 0) {
                char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
                char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
                char* c3 = strtok(NULL, " "); float f3 = strtof(c3, NULL);
                result->coords[VB_NORMAL][coords_count[VB_NORMAL]++] = (vec4_t){f1, f2, f3, 0.f};
            }
            else if (strcmp(type, "f") == 0) {
                int index = 0;
                uint32_t first_index[VB_NUM];
                uint32_t last_index[VB_NUM];
                for (int c = 0; c < VB_NUM; ++c) {
                    first_index[c] = indices_count[c];
                }
                char *token;
                while ((token = strtok(NULL, " "))) {
                    if (++index > 3) {
                        for (int c = 0; c < VB_NUM; ++c) {
                            result->indices[c][indices_count[c]++] = result->indices[c][first_index[c]];
                            result->indices[c][indices_count[c]++] = result->indices[c][last_index[c]];
                        }
                    }
                    char* slice = token;
                    for (int c = 0; c < VB_NUM; ++c) {
                        uint32_t index = (uint32_t)strtol(slice, NULL, 10) - 1;
                        last_index[c] = indices_count[c];
                        result->indices[c][indices_count[c]++] = index;
                        if ((slice = strchr(slice, '/'))) {
                            ++slice;
                        } else {
                            break;
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
    if (obj->valid) {
        free_mtllib(&obj->mtllib);
        for (int c = 0; c < VB_NUM; ++c) {
            free(obj->coords[c]);
            free(obj->indices[c]);
        }
    }
}

void draw_obj(const obj_t* obj, const mat4x4_t* model, const camera_t* camera) {
    mat4x4_t projview;
    mul_mat(&projview, &camera->proj, &camera->view);
    vec4_t camera_dir = quat_to_vec3(&camera->ang);
    int material_index = -1; mtl_t* mtl = NULL;
    for (int c = 0; c < obj->num_indices[VB_POSITION] - 2; c += 3) {
        if (material_index < MATERIAL_LIMIT - 1 &&
            c >= obj->mtl_indices[material_index + 1] &&
            obj->mtl[material_index + 1]) {
            mtl = obj->mtl[++material_index];
        }
        const vec4_t* p[3] = {
            &obj->coords[VB_POSITION][obj->indices[VB_POSITION][c]],
            &obj->coords[VB_POSITION][obj->indices[VB_POSITION][c + 1]],
            &obj->coords[VB_POSITION][obj->indices[VB_POSITION][c + 2]],
        };
        const vec4_t* t[3] = {
            &obj->coords[VB_TEXTURE][obj->indices[VB_TEXTURE][c]],
            &obj->coords[VB_TEXTURE][obj->indices[VB_TEXTURE][c + 1]],
            &obj->coords[VB_TEXTURE][obj->indices[VB_TEXTURE][c + 2]],
        };
        const vec4_t* n[3] = {
            &obj->coords[VB_NORMAL][obj->indices[VB_NORMAL][c]],
            &obj->coords[VB_NORMAL][obj->indices[VB_NORMAL][c + 1]],
            &obj->coords[VB_NORMAL][obj->indices[VB_NORMAL][c + 2]],
        };
        vec4_t triangle_normal = vec4(0.f);
        cross_vec3(&triangle_normal, sub_vec4(&vec4(0.f), p[1], p[0]), sub_vec4(&vec4(0.f), p[2], p[0]));
        normal_vec4(&triangle_normal, &vec4_copy(triangle_normal));
        vertex_t v[3];
        for (int c = 0; c < 3; ++c) {
            vec4_t diff = vec4(0.f);
            sub_vec4(&diff, p[c], &camera->pos);
            normal_vec4(&diff, &vec4_copy(diff));
        	if (dot_vec4(&diff, &triangle_normal) > 0.f) {
        	    goto next; // backface culling
        	}
            
        	// projection
			v[c].pos = *p[c];
			mul_mat_vec4(&v[c].pos, model, &vec4_copy(v[c].pos));
			mul_mat_vec4(&v[c].pos, &projview, &vec4_copy(v[c].pos));
			v[c].pos = project_to_screen(&v[c].pos, camera);
        	
        	// lighting
            const float attenuation = fminf(fmaxf(0.f, -dot_vec4(&diff, n[c])), 1.f);
            pow_vec4(&v[c].color, mtl ? &mtl->diffuse : &vec4(1.f), attenuation);
			
			// texture
			v[c].uv = *t[c];
        }
		draw_triangle(v, mtl);
next:
        continue;
    }
}
