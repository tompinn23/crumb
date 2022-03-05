#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef _MSC_VER
#include <intrin.h>
#ifndef __clang__

static inline int __builtin_ctzll(unsigned long long input) {
    unsigned long index;
#ifdef _WIN64
    _BitScanForward64(&index, input);
#else
    if((uint32_t)input != 0) {
        _BitScanForward(&index, (uint32_t)input);
    } else {
        _BitScanForward(&index, (uint32_t)(input >> 32));
        index += 32;
    }
#endif
    return index;
}

static inline int __builtin_clzll(unsigned long long input) {
    unsigned long index;
#ifdef _WIN64
    _BitScanReverse64(&index, input);
#else
    if(input > 0xFFFFFFFF) {
        _BitScanReverse(&index, (uint32_t)(input >> 32));
    } else {
        _BitScanReverse(&index, (uint32_t)(input));
        index += 32;
    }
#endif
    return 63 - index;
}

static inline int __builtin_clz(int input) {
    unsigned long index;
    _BitScanReverse(&index, input);
    return 31 - index;
}

static inline int __builtin_popcountll(unsigned long long input_num) {
#if defined(_M_ARM64) || defined(_M_ARM)
  input_num = input_num - ((input_num >> 1) & 0x5555555555555555);
  input_num = (input_num & 0x3333333333333333) + ((input_num >> 2) & 0x3333333333333333);
  input_num = ((input_num + (input_num >> 4)) & 0x0F0F0F0F0F0F0F0F);
  return (uint32_t)((input_num * (0x0101010101010101)) >> 56);
#elif defined(_WIN64) // highly recommended!!!
    return (int)__popcnt64(input_num);
#else // if we must support 32-bit Windows
    return (int)(__popcnt((uint32_t)input_num) + __popcnt((uint32_t)(input_num >> 32)));
#endif
}

#endif
#endif

typedef struct bitset {
    uint64_t* set;
    size_t sz;
    size_t cap;
} bitset_t;

bitset_t* bitset_create();
void bitset_free(bitset_t* set);

void bitset_clear(bitset_t* set);
void bitset_set(bitset_t* set, size_t i);
void bitset_clearbit(bitset_t* set, size_t i);

size_t bitset_count(const bitset_t* set);

static inline bool bitset_next(const bitset_t* set, size_t* i) {
    size_t x = *i >> 6;
    if(x >= set->sz) {
        return false;
    }
    uint64_t w = set->set[x];
    w >>= (*i & 63);
    if(w != 0) {
        *i += __builtin_ctzll(w);
        return true;
    }
    x++;
    while(x < set->sz) {
        w = set->set[x];
        if(w != 0) {
            *i = x * 64 + __builtin_ctzll(w);
            return true;
        }
        x++;
    }
    return false;
}
