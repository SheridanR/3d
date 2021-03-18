#ifndef HASHMAP_H
#define HASHMAP_H

#include "vector.h"

#define MAX_BUCKET_SIZE 4

typedef struct pair {
    void* key;
    void* value;
} pair_t;

typedef struct hashmap {
    vector_t buckets;
    size_t size;
    size_t keySize;
    size_t valueSize;

    void* (*alloc)(size_t);
    void (*free)(void*);
    size_t (*hash)(void*, size_t);
} hashmap_t;

typedef struct iterator {
    hashmap_t* hashmap;
    size_t position;
    size_t bucket;
    size_t numBuckets;
} iterator_t;

hashmap_t hashmapNew(void* (*alloc)(size_t), void (*free)(void*), size_t keySize, size_t valueSize, size_t (*hash)(void*, size_t));
void hashmapDelete(hashmap_t* hashmap);
void hashmapClear(hashmap_t* hashmap);
size_t hashmapSize(hashmap_t* hashmap);
int hashmapEmpty(hashmap_t* hashmap);
vector_t* hashmapGetBucket(hashmap_t* hashmap, size_t hash);
int hashmapKeyCompare(hashmap_t* hashmap, void* keyA, void* keyB);
int hashmapAdd(hashmap_t* hashmap, void* key, void* value);
void hashmapRehash(hashmap_t* hashmap, size_t newBucketCount);
int hashmapRemove(hashmap_t* hashmap, void* key);
void* hashmapFind(hashmap_t* hashmap, void* key);
iterator_t hashmapFindIterator(hashmap_t* hashmap, void* key);
iterator_t hashmapIteratorStart(hashmap_t* hashmap);
int hashmapIteratorEnd(iterator_t it);
iterator_t hashmapIteratorNext(iterator_t it);
pair_t hashmapIteratorGet(iterator_t it);
int hashmapIteratorRemove(iterator_t it);
size_t hashmapDjb2Hash(void* key, size_t size);

#endif