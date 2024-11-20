#pragma once

#include "eaos.h"

void irq_handle(u8 relative_vector, void *handler);
void init_pic(void);
void pic_eoi(u8 irq);

#define INTR(name) \
    extern void name##_entry(void); \
    void name(void);

INTR(div_by_zero);
INTR(single_step_interrupt);
INTR(nmi_error);
INTR(breakpoint);
INTR(overflow);
INTR(bound_range_exceeded);
INTR(invalid_opcode);
INTR(coprocessor_not_available);
INTR(double_fault);
INTR(invalid_tss);
INTR(segment_invalid);
INTR(stack_segment_fault);
INTR(general_protection_fault);
INTR(page_fault);
INTR(x87_float_exception);
INTR(alignment_check);
INTR(machine_check);
INTR(SIMD_float_exception);
INTR(virtualization_exception);
INTR(control_protection_exception);