#pragma once

#include "eaos.h"

// defers to the scheduler
void schedule();

void wake_eventually(usize pid);

void scheduler_step(void);

void schedule_spin(void);