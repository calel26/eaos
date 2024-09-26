#include "limine.h"
#include "eaos.h"

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void make_it_purple(void) {
    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    int width = framebuffer->width;
    int height = framebuffer->height;

    for (usize x = 0; x < width; x++) {
        for (usize y = 0; y < height; y++) {
            volatile u32 *fb_ptr = framebuffer->address;
            // fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xc64dff;
            fb_ptr[y * width + x] = 0xc64dff;
        }
    }
}
