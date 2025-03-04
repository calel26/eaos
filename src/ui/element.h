#pragma once

#include "eaos.h"
#include "framebuffer.h"
#include "hw/mouse.h"

struct ui_bbox {
    u32 x, y;
    u32 w, h;
};

struct ui_state {
    struct mouse *mouse;
    bool mouse_over;
    struct ui_elem *elem;
    struct eaos_terminal* term;
};

struct ui_elem {
    void (*render)(struct ui_state*);

    struct ui_bbox bbox;
    void* conf;
};

void ui_draw(struct ui_elem *elem, struct ui_state *state);

bool bbox_contains(const struct ui_bbox *bbox, u32 x, u32 y);