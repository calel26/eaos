#include "eaos.h"

__attribute__((noreturn))
void spin(void) {
    for(;;) __asm__("hlt");
}