#pragma once

#include "proc.h"

// implemented in ctx.S
extern void save_ctx(struct context *ctx);
extern void restore_ctx(struct context *ctx);