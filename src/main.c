#include "eaos.h"
#include "limine.h"
#include "framebuffer.h"

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

void start(void) {
    bool a = false;
    i32 b = 5;

    make_it_purple();

    // loop forever
    while (1) {
        // int j = b / 1-1;
        __asm__( "hlt" );
    }
}
