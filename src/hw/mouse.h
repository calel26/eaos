#pragma once
#include "eaos.h"

struct mouse {
    usize x, y;
    bool l, r, m;
};

void mouse_init(void);
bool is_mouse_configured(void);
struct mouse *get_mouse();