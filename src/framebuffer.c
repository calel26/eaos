#include "limine.h"
#include "eaos.h"
#include "framebuffer.h"
#include "mem/util.h"
#include "phys.h"

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct psf1 {
    u16 magic;
    u8 modes;
    u8 glyph_size;
} __attribute__((packed));

static const u8 font[] = {
#embed "assets/zap-light16.psf"
};

const u32 fb_margin = 5;

// this is an abomination because instead of dynamically
// allocating memory at runtime, it makes a huge hole
// in the kernel binary. this drastically inflates its size for no good reason.
static u32 abomination[2000*1000*4] = {0};

// void load_font(struct psf1* font) {
//     bool has_table = (font->modes & 0x02) != 0 || (font->modes & 0x04) != 0;
// }

void fb_print(struct eaos_terminal *terminal, char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
       fb_printc(terminal, str[i]);
    }
}

void fb_printc(struct eaos_terminal *terminal, char c) {
    if (c == '\n') {
        terminal->line++;
        terminal->cursor_x = 0;
        return;
    }

    u8 glyph_size = 16;
    u8 glyph_width = 8;

    struct psf1* fnt = (struct psf1*)(&font);
    u8* glyphs = (u8*)(((u8*)fnt) + 4); // 4 = size of header
    u8* glyph = &glyphs[c * glyph_size];

    for (int y = 0; y < glyph_size; y++) {
        u8 row = glyph[y];
        for (int x = 0; x < glyph_width; x++) {
            bool off = ((row >> (glyph_width-1-x)) & 0b1) == 0;
            u32 color = terminal->active_color;
            if (off) {
                color = 0x000000;
            }
            fb_set_px(
                terminal,
                x + terminal->cursor_x + fb_margin,
                y + fb_margin + terminal->line * glyph_size + 3,
                color
            );
            // fb[
            //     ((y + fb_margin + terminal->line * glyph_size + 3) * width)
            //     + x + terminal->cursor_x + fb_margin
            // ] = color;
        }
    }

    terminal->cursor_x += glyph_width;
}

inline struct limine_framebuffer* fb_get_framebuffer() {
    return framebuffer_request.response->framebuffers[0];
}

inline void fb_set_px(struct eaos_terminal *term, u32 x, u32 y, u32 color) {
    u64 width = term->framebuffer->width;
    u32* fb = term->fbmem;
    fb[width * y + x] = color;
}

// buffering
void fb_done(struct eaos_terminal *term) {
    return;
    struct limine_framebuffer *fb = term->framebuffer;
    vmemcpy(fb->address, term->fbmem, fb->width * fb->height * sizeof(u32));
}

void *fb_allocate_buffer(struct limine_framebuffer *fb) {
    // u32 size = fb->width * fb->height * sizeof(u32); 
    // void *addr = kcalloc(size / MEM_PAGE_SIZE);
    // return addr;
    return abomination;
}