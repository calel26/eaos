#pragma once

#include "eaos.h"

void init_pmm(void);
void *kalloc(u32 pages);
void kfree(void *ptr);