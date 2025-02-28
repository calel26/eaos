#include "ui/button.h"
#include "ui/element.h"

static void render_button(struct ui_state *state) {
    struct ui_button_config *conf = state->elem->conf;
    struct ui_bbox bbox = state->elem->bbox;
    if (state->mouse_l && state->mouse_over) {
        conf->on_click();
    }

    bool hovering = state->mouse_over;
    u32 color = conf->color;
    if (hovering) {
        color = conf->hover_color;
    }

    // render background
    for (u32 x = bbox.x; x < bbox.x + bbox.w; x++) {
        for (u32 y = bbox.y; y < bbox.y + bbox.h; y++) {
            fb_set_px(state->term, x, y, color);
        }
    }

    // render text
}

struct ui_elem ui_mk_button(struct ui_button_config *conf) {
    struct ui_elem elem = {
        .bbox = conf->bbox,
        .render = render_button,
        .conf = conf
    };

    return elem;
};