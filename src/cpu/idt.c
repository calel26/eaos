#include "idt.h"
#include "framebuffer.h"
#include "irq.h"
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
    idt[vector].ist = 0;
    idt[vector].kernel_cs = 0x28;
    idt[vector].reserved = 0;
}

struct idt_entry idt[256];

void setup_idt(void) {
    struct idtr idtr_register = {
        .base = (u64) &idt,
        .limit = sizeof(idt) - 1
    };

    kinfo("setting up IDT");

    void* handlers[32] = {
        div_by_zero_entry,
        single_step_interrupt_entry,
        nmi_error_entry,
        breakpoint_entry,
        overflow_entry,
        bound_range_exceeded_entry,
        invalid_opcode_entry,
        coprocessor_not_available_entry,
        double_fault_entry,
        null, // older thing
        invalid_tss_entry,
        segment_invalid_entry,
        stack_segment_fault_entry,
        general_protection_fault_entry,
        page_fault_entry,
        null, // RESERVED
        x87_float_exception_entry,
        alignment_check_entry,
        machine_check_entry,
        SIMD_float_exception_entry,
        virtualization_exception_entry,
        control_protection_exception_entry,
        // all else = null
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

    kinfo("IDT loaded");
}

// more public interface
void idt_set_interrupt_handler(u8 vector, void *isr, u8 ring_level) {
    u8 flags = (1<<7) + (ring_level<<4) + 0b1110; // 1110 = interrupt gate
    set_idt_entry(vector, isr, flags, idt);
}