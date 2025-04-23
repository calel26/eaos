#include "proc.h"
#include "config.h"
#include "eaos.h"
#include "log.h"

extern void save_ctx(struct context *ctx);

// this doesn't return because it jumps far away and starts a new life.
__attribute__((noreturn))
extern void restore_ctx(struct context *ctx);

struct proc proc_table[NPROC];

usize next_proc_n = 1;

// init is the first thing running.
usize running_proc_id = 1;

// creates a process and returns the process number.
usize mkproc(char *name, char *image) {
    usize id = next_proc_n++;
    if (id >= NPROC) {
        kpanic("process limit reached!");
    }

    struct proc proc = {
        .id = id,
        .name = name,
        .image = image,
        .status = proc_status_waiting
    };

    proc_table[id] = proc;

    return id;
}

usize get_proc_id(void) {
    return running_proc_id;
}

inline struct proc *get_proc_table(void) {
    return proc_table;
}

void wake(usize id) {
    running_proc_id = id;
    struct proc *p = &proc_table[id];
    p->status = proc_status_running;

    restore_ctx(p->ctx);
}