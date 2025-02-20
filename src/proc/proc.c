#include "proc.h"
#include "config.h"
#include "log.h"
#include "phys.h"

extern void save_ctx(struct context* ctx);
extern void restore_ctx(struct context* ctx);

struct proc proc_table[NPROC];

usize next_proc_n = 1;

usize running_proc_id = 0;

// creates a process and returns the process number.
usize mkproc(char* name, char* image) {
    struct context* ctx = kalloc(1);
    // save_ctx(ctx); // this causes a page fault

    usize id = next_proc_n ++;
    if (id >= NPROC) {
        kpanic("process limit reached!");
    }

    struct proc proc = {
        .ctx = ctx,
        .id = id,
        .name = name,
        .image = image
    };

    proc_table[id] = proc;

    return id;
}

void yield(void) {
    
}

usize get_proc_id(void) {
    return running_proc_id;
}