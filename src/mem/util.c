#include "eaos.h"
#include "util.h"

void memcpy(void *dest, const void *src, usize n) {
    for (usize i = 0; i < n; i++) {
        ((u8*)dest)[i] = ((u8*)src)[i];
    }
}

void memset(void *dest, u8 val, usize length) {
    for (usize i = 0; i < length; i++) {
        ((u8*)dest)[i] = val;
    }
}