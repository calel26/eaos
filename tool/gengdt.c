#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

// https://wiki.osdev.org/GDT_Tutorial#Basics
// ported to tool so that I can encode values directly in the bootloader's assembly
int main(void) {
    uint8_t *target = malloc(8);

    for(;;) {
        memset(target, 0, 8);

        uint32_t limit = 0;
        printf("enter limit (lowercase hex) = ");
        fflush(stdout);
        scanf("%x", &limit);

        // Check the limit to make sure that it can be encoded
        if (limit > 0xFFFFF) {
            printf("GDT cannot encode limits larger than 0xFFFFF, try again \n");
            continue;
        }

        uint32_t base = 69420;
        printf("enter base (lowercase hex) = ");
        fflush(stdout);
        scanf("%x", &limit);
        uint8_t access_byte = 0;
        printf("enter access byte (lowercase hex) = ");
        fflush(stdout);
        scanf("%" PRIx8 "", &access_byte);
        uint8_t flags = 0;
        printf("enter flags (lowercase hex) = ");
        fflush(stdout);
        scanf("%" PRIx8 "", &flags);

        
        // Encode the limit
        target[0] = limit & 0xFF;
        target[1] = (limit >> 8) & 0xFF;
        target[6] = (limit >> 16) & 0x0F;
        
        // Encode the base
        target[2] = base & 0xFF;
        target[3] = (base >> 8) & 0xFF;
        target[4] = (base >> 16) & 0xFF;
        target[7] = (base >> 24) & 0xFF;
        
        // Encode the access byte
        target[5] = access_byte;
        
        // Encode the flags
        target[6] |= (flags << 4);

        printf("output = 0x%" PRIx64 "\n", *((uint64_t*) target));
    }

    free(target);
}