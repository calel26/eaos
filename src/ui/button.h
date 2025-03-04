#pragma once

#include "ui/element.h"

struct ui_button_config {
    struct ui_bbox bbox;
    char *text;
    u32 color;
    u32 hover_color;
    void (*on_click)(struct ui_state *state);
};

struct ui_elem ui_mk_button(struct ui_button_config *conf);