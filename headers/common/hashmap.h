#ifndef HASHMAP_H
#define HASHMAP_H

#ifdef __HASHMAP_C
    #include <stdlib.h> // size_t 
    #include <string.h> // memcmp()
    #include <stdio.h> // perror(), printf()
    #include <common/base_types.h>
    #include <common/common.h>
#endif

typedef struct ChainData {
    void *key;
    size_t key_len;
    void *data;
} ChainData;


typedef struct HashData {
    void *data;
    void *key;
    size_t key_len;
} HashData;

typedef struct Hashmap {
    HashData *map_data;
    size_t map_size;
    size_t used_size;
} Hashmap;

#ifdef __HASHMAP
    size_t hashfunc (
        void *key, 
        size_t n_key, 
        size_t map_size
    ); 

    void reallocateHashmap(Hashmap *p_hm);
#endif

void newHashmap (
    Hashmap *p_hashmap, 
    size_t n_len
);

void pushToHashmap (
    Hashmap *p_hm,
    void *key,
    size_t key_size,
    void *val
);

void *findValue (
    Hashmap *p_hm, 
    void *key, 
    size_t key_len
);

#endif
