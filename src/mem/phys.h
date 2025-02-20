#pragma once

#include "eaos.h"

#define MEM_PAGE_SIZE 4096

void init_pmm(void);
void *kalloc(u32 pages);
void *kcalloc(u32 pages);
void kfree(void *ptr);

usize get_total_memory(void);