#include "bitset.h"

#include "z-mem.h"

#include <string.h>

bitset_t* bitset_create() {
    bitset_t* set = mem_alloc(sizeof(bitset_t));
    set->set = NULL;
    set->sz = 0;
    set->cap = 0;
    return set;
}

void bitset_free(bitset_t* set) {
    mem_free(set->set);
    mem_free(set);
}

void bitset_clear(bitset_t* set) {
    memset(set->set, 0, sizeof(uint64_t) * set->sz);
}

static inline bool bitset_grow(bitset_t* set, size_t new_sz) {
    if(set->cap < new_sz) {
        uint64_t* new_array;
        set->cap = new_sz * 2;
        new_array = mem_realloc(set->set, sizeof(uint64_t) * set->cap);
        set->set = new_array;
    }
    memset(set->set + set->sz, 0, sizeof(uint64_t) * (new_sz - set->sz));
    set->sz = new_sz;
    return true;
}

void bitset_set(bitset_t* set, size_t i) {
    size_t shiftedi = i >> 6;
    if(shiftedi >= set->sz) {
        if(!bitset_grow(set, shiftedi + 1)) {
            return;
        }
    }
    set->set[shiftedi] |= ((uint64_t)1) << (i % 64);
}

void bitset_clearbit(bitset_t* set, size_t i) {
    size_t shiftedi = i >> 6;
    if(shiftedi >= set->sz) {
        if(!bitset_grow(set, shiftedi + 1)) {
            return;
        }
    }
    set->set[shiftedi] |= ((uint64_t)0) << (i % 64);
}

size_t bitset_count(const bitset_t* set) {
    size_t card = 0;
    size_t k = 0;
    // assumes that long long is 8 bytes
    for(; k + 7 < set->sz; k+=8) {
        card += __builtin_popcountll(set->set[k]);
        card += __builtin_popcountll(set->set[k+1]);
        card += __builtin_popcountll(set->set[k+2]);
        card += __builtin_popcountll(set->set[k+3]);
        card += __builtin_popcountll(set->set[k+4]);
        card += __builtin_popcountll(set->set[k+5]);
        card += __builtin_popcountll(set->set[k+6]);
        card += __builtin_popcountll(set->set[k+7]);
    }
    for(; k + 3 < set->sz; k+=4) {
        card += __builtin_popcountll(set->set[k]);
        card += __builtin_popcountll(set->set[k+1]);
        card += __builtin_popcountll(set->set[k+2]);
        card += __builtin_popcountll(set->set[k+3]);
    }
    for(; k < set->sz; k++) {
        card += __builtin_popcountll(set->set[k]);
    }
    return card;
}
