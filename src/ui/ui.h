#pragma once

#include "ui/element.h"

// redraws all UI elements that need to be redrawn.
void redraw(void);

// redraws all UI elements regardless of if they need to be redrawn.
void redraw_all(void);

// adds an element to the screen
void ui_add_elem(struct ui_elem *elem);