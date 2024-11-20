#pragma once

#include "eaos.h"

void setup_idt(void);

void idt_set_interrupt_handler(u8 vector, void *isr, u8 ring_level);