#ifndef MTL_H
#define MTL_H

#include "vec4.h"
#include <stdint.h>

#define FILENAME_SIZE 32

typedef struct texture {
    char filename[FILENAME_SIZE];
    void* surface;
    uint32_t* pixels;
    int width;
    int height;
} texture_t;

typedef struct mtl {
    char filename[FILENAME_SIZE];
    vec4_t diffuse;
    texture_t diffuse_texture;
} mtl_t;

#define MATERIAL_LIMIT 4

typedef struct mtllib {
    mtl_t materials[MATERIAL_LIMIT];
} mtllib_t;

mtllib_t* load_mtllib(mtllib_t* result, const char* filename);
void free_mtllib(mtllib_t* mtllib);

#endif