#include "log.h"
#include "framebuffer.h"

struct eaos_terminal *active_terminal;

void log_setterm(struct eaos_terminal *term) {
    active_terminal = term;
}

void kmsg(struct log_level *level, char *message) {
    u32 original_color = active_terminal->active_color;

    active_terminal->active_color = level->color;
    fb_printc(active_terminal, '[');
    fb_print(active_terminal, level->name);
    fb_print(active_terminal, "] ");

    active_terminal->active_color = original_color;
    fb_print(active_terminal, message);
    fb_printc(active_terminal, '\n');
}

struct log_level ll_info = {
    .name = "info",
    .color = 0x63a2ff
};

void kinfo(char *str) {
    kmsg(&ll_info, str);
}

struct log_level ll_warn = {
    .name = "warn",
    .color = 0xffd063
};

void kwarn(char *str) {
    kmsg(&ll_warn, str);
}

struct log_level ll_error = {
    .name = "error",
    .color = 0xfc3a3a
};

void kerr(char *str) {
    kmsg(&ll_error, str);
}

void kpanic(char *str) {

}