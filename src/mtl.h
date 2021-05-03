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
    uint32_t shift[4];
    uint32_t mask[4];
} texture_t;

typedef struct mtl {
    char filename[FILENAME_SIZE];
    vec4_t diffuse;
    texture_t diffuse_texture;
} mtl_t;

#define MATERIAL_LIMIT 16

typedef struct mtllib {
    mtl_t materials[MATERIAL_LIMIT];
} mtllib_t;

mtllib_t* load_mtllib(mtllib_t* result, const char* filename);
void free_mtllib(mtllib_t* mtllib);

#endif
