#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdlib.h>

typedef struct vector {
    void* data;
    size_t numElements;
    size_t maxElements;
    size_t elementSize;

    void* (*alloc)(size_t);
    void (*free)(void*);
} vector_t;

vector_t vectorNew(void* (*alloc)(size_t), void (*free)(void*), size_t elementSize);
void vectorDelete(vector_t* vector);
void vectorClear(vector_t* vector);
size_t vectorSize(const vector_t* vector);
int vectorEmpty(const vector_t* vector);
void vectorAdd(vector_t* vector, const void* const data);
int vectorResize(vector_t* vector, size_t size);
int vectorRemove(vector_t* vector, size_t index);
int vectorRemoveSwap(vector_t* vector, size_t index);
void* vectorGet(vector_t* vector, size_t index);
int vectorCopy(vector_t* src, vector_t* dest);

#endif