#pragma once

#include "eaos.h"

enum proc_event {
    ev_timer,
    ev_keyboard,
    ev_mouse,
    ev_custom,

    event_count
};

struct proc_event_subscription {
    // 0 if empty
    usize proc;
};