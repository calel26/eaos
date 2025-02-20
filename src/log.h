#pragma once

#include "framebuffer.h"

// ascii escape code
#define T_ESC "\x1b"
#define T_COLOR(color) T_ESC "[c(" color ")"

struct log_level {
    char *name;
    u32 color;
};

void kmsg(struct log_level *level, char *message);
void kinfo(char *str);
void kwarn(char *str);
void kerr(char *str);

void kpanic(char *str);

void print_number(u64 value);

struct stringified_number {
    char data[17];
};
struct stringified_number number_to_string(u64 value);

void log_setterm(struct eaos_terminal *term);
struct eaos_terminal* log_getterm(void);

void kprintf(char *str, ...);