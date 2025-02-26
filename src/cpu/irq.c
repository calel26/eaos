#include "irq.h"
#include "idt.h"
#include "io.h"
#include "log.h"

// PIC Constants
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

#define PIC_EOI 0x20 // end of interrupt

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF1 0x0C
#define ICW4_BUF2 0x08
#define ICW4_SFNM 0x10

#define FULL_INIT ICW1_INIT | ICW1_ICW4

void pic_cmd(u8 main, u8 secondary) {
    outb(PIC1_COMMAND, main);
    outb(PIC2_COMMAND, secondary);
    io_wait();
}

void pic_data(u8 main, u8 secondary) {
    outb(PIC1_DATA, main);
    outb(PIC2_DATA, secondary);
    io_wait();
}

void init_pic() {
    kinfo("initializing PIC...");

    // start up the PICs
    pic_cmd(FULL_INIT, FULL_INIT);

    // ICW2 is the base interrupt
    // IRQ 2 will then trigger interrupt 2 + 0x20
    pic_data(0x20, 0x28);

    // ICW3 tells the secondary PIC about the primary and vice versa
    pic_data(4, 2);

    pic_data(ICW4_8086, ICW4_8086);

    // now we the PIC what interrupts to actually forward.
    // Only interested in 0x00, the timer, 0x01, the keyboard, and 0x02, the secondary PIC
    // On the second PIC, we want 0x0C (the fourth relatively), the mouse
    pic_data(0b11111000, 0b11101111);

    // enables RTC (int 0x08) for testing
    // __asm__ volatile ("cli");
    // outb(0x70, 0x8B);
    // u8 prev = inb(0x71);
    // outb(0x70, 0x8B);
    // outb(0x71, prev | 0x40);
    // __asm__ volatile ("sti");

    // the PIC should be done!
    kinfo("PIC setup complete");
}

// end of interrupt
void pic_eoi(u8 irq) {
    if (irq >= 8) {
        // this IRQ is handled by the secondary PIC
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

void irq_handle(u8 relative_vector, void *handler) {
    idt_set_interrupt_handler(0x20 + relative_vector, handler, 0);
}

void div_by_zero() {
    kpanic("division by zero");
};
void single_step_interrupt() {};
void nmi_error() {};
void breakpoint() {};
void overflow() {};
void bound_range_exceeded() {};
void invalid_opcode() {};
void coprocessor_not_available() {};
void double_fault() {
    kpanic("there was a double fault");
}
void invalid_tss() {};
void segment_invalid() {};
void stack_segment_fault() {};
void general_protection_fault() {};
void page_fault() {
    kpanic("page fault!");
};
void x87_float_exception() {};
void alignment_check() {};
void machine_check() {};
void SIMD_float_exception() {};
void virtualization_exception() {};
void control_protection_exception() {};