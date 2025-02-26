#include "event.h"
#include "config.h"

static struct ev_subscription subscriptions[event_count][MAX_EVENT_SUBSCRIBERS];