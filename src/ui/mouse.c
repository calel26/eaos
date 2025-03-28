#include "mouse.h"
#include "framebuffer.h"
#include "log.h"

// yes I drew this by hand in 1's and 0's
static u16 bitmap_black[16] = {
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

static u16 bitmap_white[16] = {
    0b1100000000000000,
    0b1011000000000000,
    0b1000110000000000,
    0b1000001100000000,
    0b1000000011000000,
    0b1000000000110000,
    0b1000000000001100,
    0b1000000000000011,
    0b1111111101111111,
    0b0000000101000000,
    0b0000000101000000,
    0b0000000010100000,
    0b0000000010100000,
    0b0000000001010000,
    0b0000000001010000,
    0b0000000000111000,
};

u32 last_x, last_y;

void draw_mouse(u32 x, u32 y) {
    struct eaos_terminal *term = log_getterm();
    // clear previous cursor position
    if (last_x != 0 && last_y != 0) {
        for (u32 bx = 0; bx < 16; bx++) {
            for (u32 by = 0; by < 16; by++) {
                bool off = (((bitmap_black[by] >> (16-1-bx)) & 0b1) == 0) && 
                    (((bitmap_white[by] >> (16-1-bx)) & 0b1) == 0);
                if (off) continue;
                fb_set_px(term, bx + last_x, by + last_y, 0);
            }
        }
    }

    // draw new cursor
    for (u32 bx = 0; bx < 16; bx++) {
        for (u32 by = 0; by < 16; by++) {
            bool black = ((bitmap_black[by] >> (16-1-bx)) & 0b1) == 1;
            bool white = ((bitmap_white[by] >> (16-1-bx)) & 0b1) == 1;
            if (!(black || white)) continue;
            u32 color = 0;
            if (white) {
                color = 0xFFFFFF;
            }
            fb_set_px(term, bx + x, by + y, color);
        }
    }
    last_x = x;
    last_y = y;
}