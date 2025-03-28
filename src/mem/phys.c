#include "phys.h"
#include "log.h"
#include "limine.h"
#include "eaos.h"
#include "util.h"

__attribute__((used, section(".requests")))
struct limine_memmap_request memmap = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests")))
// HHDM = higher half direct memory
struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

struct pmm {
    void* end;
    void* head;
    void* v_end;
    void* v_head;
};

struct pmm pmm_status;

usize total_memory = 0;

void init_pmm(void) {
    kinfo("Initializing kernel physical memory allocator...");
    if(memmap.response == null) {
        kpanic("didn't get memmap from limine");
    }

    struct limine_memmap_response *mm = memmap.response;

    struct limine_memmap_entry *largest_block = null;

    for (usize e = 0; e < mm->entry_count; e++) {
        struct limine_memmap_entry *en = mm->entries[e];
        if(en->type != LIMINE_MEMMAP_USABLE) continue;
        if(largest_block == null || en->length > largest_block->length)
            largest_block = en;
        total_memory += en->length;
    }

    if (largest_block == null) {
        kpanic("couldn't find any available memory?");
    }

    kinfo(ksprintf("Detected available memory: %d", total_memory));
    kinfo(ksprintf("Detected largest block of: %d", largest_block->length));

    pmm_status.end = (void *) (largest_block->base + largest_block->length);
    pmm_status.head = (void *) largest_block->base;
    // pmm_status.head = (void *) hhdm_request.response->offset;
    // pmm_status.end = (void *) hhdm_request.response->offset + total_memory;
}

void* kalloc(u32 pages) {
    void* head = pmm_status.head;
    pmm_status.head += MEM_PAGE_SIZE * pages;
    // if (pmm_status.head > pmm_status.end) {
    //     kpanic("out of memory");
    // }

    return head + hhdm_request.response->offset;
};

void* kcalloc(u32 pages) {
    void* ptr = kalloc(pages);
    // zero the memory
    memset(ptr, 0, MEM_PAGE_SIZE * pages);
    return ptr;
}

void kfree(void *ptr) {
    kwarn("free not implemented yet!");
};

usize get_total_memory(void) {
    return total_memory;
}