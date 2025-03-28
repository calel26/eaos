#include "ui/button.h"
#include "framebuffer.h"
#include "ui/element.h"

static void render_button(struct ui_state *state) {
    struct ui_button_config *conf = state->elem->conf;
    struct ui_bbox bbox = state->elem->bbox;
    bool disabled = conf->disabled;
    if (state->mouse->l && state->mouse_over && !disabled) {
        conf->on_click(state);
    }

    bool hovering = state->mouse_over;
    u32 color = conf->color;
    if (hovering) {
        color = conf->hover_color;
    }
    if (disabled) {
        color = 0xAFAFAF;
    }

    // render background
    for (u32 x = bbox.x; x < bbox.x + bbox.w; x++) {
        for (u32 y = bbox.y; y < bbox.y + bbox.h; y++) {
            fb_set_px(state->term, x, y, color);
        }
    }

    // fb_print(state->term, "done one");

    // render text
    u32 len = 0;
    while (conf->text[len++] != '\0');
    u32 y = bbox.y + (bbox.h / 2) - (16/2);
    u32 x = bbox.x + (bbox.w / 2) - ((len*8)/2);
    struct eaos_terminal t = *state->term;
    t.line = (y - 5) / 16;
    t.cursor_x = x - 5;
    fb_print(&t, conf->text);
}

struct ui_elem ui_mk_button(struct ui_button_config *conf) {
    struct ui_elem elem = {
        .bbox = conf->bbox,
        .render = render_button,
        .conf = conf
    };

    return elem;
};