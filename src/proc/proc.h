#pragma once

#include "eaos.h"
#include "proc/event.h"

struct context {
    u64 rdi; u64 rsi; u64 rbp; u64 rsp;
    u64 rax; u64 rbx; u64 rcx; u64 rdx;
    u64 r8; u64 r9; u64 r10; u64 r11; u64 r12; u64 r13; u64 r14; u64 r15;
    u64 rip;
    u64 rflags;
    u64 cs;
};

struct proc {
    usize id;
    char* name;
    char* image;
    struct context* ctx;
};

usize mkproc(char* name, char* image);

void wake(usize id);
void yield(void);

void wait(enum proc_event event);

usize get_proc_id();