#include "ui.h"
#include "log.h"
#include "mouse.h"
#include "hw/mouse.h"
#include "ui/element.h"

static struct ui_elem *elems[32] = {0};
static bool finished_drawing = true;
static u8 elems_i = 0;

inline void redraw() { redraw_all(); }

void redraw_all() {
    if (!finished_drawing) return;
    finished_drawing = false;
    struct mouse *m = get_mouse();

    struct ui_state state = {
        .term = log_getterm(),
        .mouse = m,
        .mouse_over = false
    };

    for (u8 i = 0; i < elems_i; i++) {
        struct ui_elem *elem = elems[i];
        if (elem->render != null) {
            state.mouse_over = bbox_contains(&elem->bbox, m->x, m->y);
            ui_draw(elem, &state);
        } else {
            break;
        }
    }

    draw_mouse(m->x, m->y);
    finished_drawing = true;

    fb_done(state.term);
}

void ui_add_elem(struct ui_elem *elem) {
    elems[elems_i++] = elem;
}