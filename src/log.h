#pragma once

#include "framebuffer.h"

struct log_level {
    char *name;
    u32 color;
};

void kmsg(struct log_level *level, char *message);
void kinfo(char *str);
void kwarn(char *str);
void kerr(char *str);

void kpanic(char *str);

void log_setterm(struct eaos_terminal *term);