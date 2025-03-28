#pragma once

#include "eaos.h"

void memcpy(void *dest, const void *src, usize n);
void vmemcpy(volatile void *dest, const void *src, usize n);

void memset(void *dest, u8 val, usize length);