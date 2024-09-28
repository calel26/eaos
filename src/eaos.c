#include "eaos.h"

void spin(void) {
    for(;;) __asm__("hlt");
}