#include "eaos.h"
#include "limine.h"

struct eaos_terminal {
    struct limine_framebuffer *framebuffer;
    u32 cursor_x;
    u32 line;
    u32 active_color;
};

void fb_printc(struct eaos_terminal* terminal, char c);
void fb_print(struct eaos_terminal* terminal, char* str);

struct limine_framebuffer* fb_get_framebuffer();