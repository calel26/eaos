#include "idt.h"
#include "framebuffer.h"
#include "log.h"
#include "eaos.h"
#include "mem/util.h"
#include "mem/phys.h"

struct interrupt_frame {
    u16 ip;
    u16 cs;
    u16 flags;
    u16 sp;
    u16 ss;
};

struct idt_entry {
    u16 isr_low; // ISR's lower address bits
    u16 kernel_cs; // Segment loaded into CS before calling ISR
    u8 ist;
    u8 attributes;
    u16 isr_mid; // Middle 16 bits of ISR's addr
    u32 isr_high; // upper 32 bits of the ISR's addr
    u32 reserved;
} __attribute__((packed));

struct idtr {
    u16 limit;
    u64 base;
} __attribute__((packed));


void set_idt_entry(u8 vector, void *isr, u8 flags, struct idt_entry *idt) {
    u64 a = (u64) isr;
    idt[vector].isr_low = (a & 0xFFFF);
    idt[vector].isr_mid = ((a & 0xFFFF0000) >> 16);
    idt[vector].isr_high = ((a & 0xFFFFFFFF00000000) >> 32);
    // idt[vector].attributes = flags;
    idt[vector].attributes = 0x8E;
    idt[vector].kernel_cs = 0x01;
    idt[vector].reserved = 0;
}

#define intr __attribute__((interrupt))

intr void div_by_zero(struct interrupt_frame* stack_frame) {
    for(;;) {}
};
intr void single_step_interrupt(struct interrupt_frame* stack_frame) {};
intr void nmi_error(struct interrupt_frame *stack_frame) {};
intr void breakpoint(struct interrupt_frame *stack_frame) {};
intr void overflow(struct interrupt_frame *stack_frame) {};
intr void bound_range_exceeded(struct interrupt_frame *stack_frame) {};
intr void invalid_opcode(struct interrupt_frame *stack_frame) {};
intr void coprocessor_not_available(struct interrupt_frame *stack_frame) {};
intr void double_fault(struct interrupt_frame *stack_frame) {
    for(;;) {}
    // kpanic("double fault");
}
intr void invalid_tss(struct interrupt_frame *stack_frame) {};
intr void segment_invalid(struct interrupt_frame *stack_frame) {};
intr void stack_segment_fault(struct interrupt_frame *stack_frame) {};
intr void general_protection_fault(struct interrupt_frame *stack_frame) {};
intr void page_fault(struct interrupt_frame *stack_frame) {};
intr void x87_float_exception(struct interrupt_frame *stack_frame) {};
intr void alignment_check(struct interrupt_frame *stack_frame) {};
intr void machine_check(struct interrupt_frame *stack_frame) {};
intr void SIMD_float_exception(struct interrupt_frame *stack_frame) {};
intr void virtualization_exception(struct interrupt_frame *stack_frame) {};
intr void control_protection_exception(struct interrupt_frame *stack_frame) {};

__attribute__((section("text")))
struct idt_entry idt[256];

struct idtr idtr_register = {
    .limit = sizeof(idt) - 1,
    .base = (u64) idt
};

void setup_idt(void) {
    kinfo("setting up IDT");
    print_number(*(u64*) &idtr_register);
    spin();

    void* handlers[32] = {
        div_by_zero,
        single_step_interrupt,
        nmi_error,
        breakpoint,
        overflow,
        bound_range_exceeded,
        invalid_opcode,
        coprocessor_not_available,
        double_fault,
        null, // older thing
        invalid_tss,
        segment_invalid,
        stack_segment_fault,
        general_protection_fault,
        page_fault,
        null, // RESERVED
        x87_float_exception,
        alignment_check,
        machine_check,
        SIMD_float_exception,
        virtualization_exception,
        control_protection_exception,
        // all else = NULL
    };

    // the interrupt handler will run in ring 0. this value is ignored by hardware interrupts which happen in ring 0 regardless
    u8 ring_level = 0;
    u8 gate_type = 0b1111; // trap gate, not interrupt

    // 1 = present bit.
    u8 flags = (1<<7) + (ring_level<<4) + gate_type;
    for(int i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
        void *handler = handlers[i];
        if(handler == null) continue;
        set_idt_entry(i, handler, flags, idt);
    }

    __asm__ volatile ("lidt %0" : : "m" (idtr_register));
    __asm__ volatile ("sti"); // enable interrupts

    kinfo("printing idtr:\n");
    print_number(idtr_register.base);
    print_number((u64) idtr_register.limit);
    struct idt_entry *first = (struct idt_entry*)idtr_register.base;
    u64 isr = (u64) first->isr_low | ((u64) first->isr_mid << 16) | ((u64) first->isr_high << 32);

    print_number(isr);
    kinfo("as opposed to");
    print_number((u64) div_by_zero);

    kinfo("IDT loaded");
}