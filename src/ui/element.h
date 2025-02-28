#pragma once

#include "eaos.h"
#include "framebuffer.h"

struct ui_bbox {
    u32 x, y;
    u32 w, h;
};

struct ui_state {
    bool mouse_over, mouse_l, mouse_r, mouse_mid;
    u32 mouse_x, mouse_y;
    struct ui_elem *elem;
    struct eaos_terminal* term;
};

struct ui_elem {
    void (*render)(struct ui_state*);

    struct ui_bbox bbox;
    void* conf;
};