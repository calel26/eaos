#include "mouse.h"
#include "framebuffer.h"
#include "log.h"

static u16 bitmap[16] = {
    0b1100000000000000,
    0b1111000000000000,
    0b1111110000000000,
    0b1111111100000000,
    0b1111111111000000,
    0b1111111111110000,
    0b1111111111111100,
    0b1111111111111111,
    0b1111111111111111,
    0b0000000010000000,
    0b0000000010000000,
    0b0000000001000000,
    0b0000000001000000,
    0b0000000000100000,
    0b0000000000100000,
    0b0000000000010000,
};

u32 last_x, last_y;

void draw_mouse(u32 x, u32 y) {
    struct eaos_terminal *term = log_getterm();
    if (last_x != 0 && last_y != 0) {
        for (u32 bx = 0; bx < 16; bx++) {
            for (u32 by = 0; by < 16; by++) {
                bool off = ((bitmap[by] >> (16-1-bx)) & 0b1) == 0;
                if (off) continue;
                fb_set_px(term, bx + last_x, by + last_y, 0);
            }
        }
    }
    for (u32 bx = 0; bx < 16; bx++) {
        for (u32 by = 0; by < 16; by++) {
            bool off = ((bitmap[by] >> (16-1-bx)) & 0b1) == 0;
            u32 color = 0xFFFFFF;
            if (off) {
                color = 0;
            };
            fb_set_px(term, bx + x, by + y, color);
        }
    }
    last_x = x;
    last_y = y;
}