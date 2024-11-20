#pragma once

#include "eaos.h"

u8 inb(u16 port);
void outb(u16 port, u8 byte);
void io_wait(void);