#include "kui.h"
#include "log.h"
#include "phys.h"
#include "ui/button.h"
#include "ui/element.h"
#include "ui/ui.h"

static void on_click(struct ui_state *state) {
    kinfo("click!");
    struct ui_button_config *bconf = state->elem->conf;
    bconf->text = "clicked!";
}

static struct ui_button_config conf = {
    .bbox = {
        .x = 400,
        .y = 400,
        .h = 100,
        .w = 200,
    },
    .color = 0xFF0000,
    .hover_color = 0x00FF00,
    .on_click = on_click,
    .text = "hugh mungus"
};

void start_kui() {
    kinfo("starting kernel UI");

    struct ui_elem* test = kalloc(1);
    *test = ui_mk_button(&conf);
    ui_add_elem(test);
    
    redraw_all();
}