#pragma once

#include "eaos.h"

// defers to the scheduler
extern void schedule();

void schedule_inner(u64 caller_rbp, u64 caller_rsp, u64 original_ret_addr);

void wake_eventually(usize pid);

void scheduler_step(void);

void schedule_spin(void);