#include "log.h"
#include "eaos.h"
#include "framebuffer.h"
#include <stdarg.h>

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

// Panics with a message and then spins.
void kpanic(char *str) {
    active_terminal->active_color = ll_error.color;
    fb_print(active_terminal, "\n !! KERNEL PANIC !!\n");

    active_terminal->active_color = 0xffffff;
    fb_print(active_terminal, "The kernel encountered a major issue and\nhad to stop. Please reboot your computer!\n\n");

    active_terminal->active_color = 0x9e9e9e;
    fb_print(active_terminal, "Error message: ");
    active_terminal->active_color = 0xffffff;
    fb_print(active_terminal, str);

    // clear interrupts before spinning to prevent continuing from a panic
    __asm__ volatile ( "cli" );

    spin();
}

static char hexalphabet[16] = "01234567890abcef";

struct stringified_number number_to_string(u64 value) {
    struct stringified_number n = {};
    
    for (int i = 15; i >= 0; i--) {
        n.data[i] = hexalphabet[value & 0xF];
        value >>= 4;
    }

    n.data[16] = '\0';
    return n;
}

void print_number(u64 value) {
    fb_print(log_getterm(), "0x");
    fb_print(log_getterm(), number_to_string(value).data);
    fb_printc(log_getterm(), '\n');
}

struct eaos_terminal* log_getterm(void) {
    return active_terminal;
}

void kprintf(char *str, ...) {
}