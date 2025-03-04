#include "element.h"

void ui_draw(struct ui_elem *elem, struct ui_state *state) {
    state->elem = elem;
    elem->render(state);
}

bool bbox_contains(const struct ui_bbox *bbox, u32 x, u32 y) {
    return x > bbox->x && y > bbox->y
        && x - bbox->x < bbox->w
        && y - bbox->y < bbox->h;
}