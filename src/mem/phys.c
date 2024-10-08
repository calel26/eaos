#include "phys.h"
#include "log.h"
#include "limine.h"
#include "eaos.h"

#define PAGE_SIZE 4096

__attribute__((used, section(".requests")))
struct limine_memmap_request memmap = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

struct pmm {
    void* end;
    void* head;
};

struct pmm pmm_status;

void init_pmm(void) {
    kinfo("Initializing kernel physical memory allocator...");
    if (true) {
        kwarn("^ not yet");
        return;
    }
    if(memmap.response == null) {
        kpanic("didn't get memmap from limine");
    }

    struct limine_memmap_response *mm = memmap.response;

    struct limine_memmap_entry *largest_block = null;
    for(usize e = 0; e < mm->entry_count; e++) {
        struct limine_memmap_entry *en = mm->entries[e];
        if(en->type != LIMINE_MEMMAP_USABLE) continue;
        if(largest_block == null || en->length > largest_block->length)
            largest_block = en;
    }
    if (largest_block == null) {
        kpanic("couldn't find any available memory?");
    }

    pmm_status.end = (void *) (largest_block->base + largest_block->length);
    pmm_status.head = (void *) largest_block->base;
}

void* kalloc(u32 pages) {
    pmm_status.head += PAGE_SIZE;
    if (pmm_status.head > pmm_status.end) {
        kpanic("out of memory");
    }

    return pmm_status.head;
};

void kfree(void *ptr) {
    kwarn("free not implemented yet!");
};