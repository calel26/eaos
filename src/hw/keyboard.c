#include "hw/keyboard.h"
#include "framebuffer.h"
#include "io.h"
#include "irq.h"
#include "log.h"

INTR(kbd_handle);

static const char keymap[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

static const char shift_keymap[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

struct keyboard_demo {
    bool enabled;
    u8 last_event;
    usize line, x;
};

static struct keyboard_demo demo;

void kbd_init(void) {
    kinfo("setting up keyboard...");
    
    irq_handle(0x01, kbd_handle_entry);
    // set up keyboard demo:

    kinfo("keyboard demo: ");
    demo.line = log_getterm()->line;
    fb_print(log_getterm(), "> ");
    demo.x = log_getterm()->cursor_x;
    fb_print(log_getterm(), "* press a key *\n");

    demo.enabled = true;
}

static char entry_space[32];
static u8 i = 0;
static bool shift = false;

void render_entry() {
    struct eaos_terminal* term = log_getterm();
    usize x = term->cursor_x;
    fb_print(term, "                                                       ");
    term->cursor_x = x;
    for (u8 j = 0; j < i; j++) {
        fb_printc(term, entry_space[j]);
    }
}

void kbd_handle(void) {
    if (!demo.enabled) {
        (void) inb(0x60);
        goto skip_render;
    }
    struct eaos_terminal* term = log_getterm();

    u8 ev = inb(0x60);
    demo.last_event = ev;

    if (ev == 0x2A || ev == 0x36) {
        // shift
        shift = true;
        goto skip_render;
    } else if (ev == 0xAA || ev == 0xB6) {
        shift = false;
        goto skip_render;
    }

    usize old_line = term->line;
    usize old_x = term->cursor_x;
    usize old_color = term->active_color;

    term->active_color = 0xFFFFFF;
    term->cursor_x = demo.x;
    term->line = demo.line;

    if (i >= sizeof(entry_space)) {
        i = 0;
    }

    if (ev & 0x80) {
        // key release
        // u8 index = ev & 0x7F;
    } else {
        u8 index = ev;
        char key = keymap[index];
        if (key == '\n') {
            // reset
            i = 0;
        } else if (key == '\b') {
            // delete
            if (i > 0) { --i; }
        } else {
            if (shift) key = shift_keymap[index];
            entry_space[i] = key;
            ++i;
        }
    }

    render_entry();

    term->line = old_line;
    term->cursor_x = old_x;
    term->active_color = old_color;

skip_render:
    pic_eoi(0x01);
}

char kbd_getchar(void) {
    kwarn("todo: kbd_getchar()");
    return '?';
}