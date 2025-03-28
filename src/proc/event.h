#pragma once

#include "eaos.h"

enum ev_type {
    // this event will never fire
    ev_never,
    // this represents a custom event
    ev_custom,
    // fires at the specified interval
    ev_timer,
    // fires whenever new keyboard data comes in
    ev_keyboard,
    // fires whenever the mouse moves
    ev_mouse,

    // not an event, just used internally for knowing how many events there are.
    event_count
};

struct ev_never_config {};
struct ev_custom_config {
    usize file;
};
struct ev_timer_config {
    usize miliseconds;
};
struct ev_keyboard_config {
    bool keydown, keyup;
};
struct ev_mouse_config {
    // TODO: flesh this out more
    bool click;
};

union ev_subscription_params {
    struct ev_never_config never;
    struct ev_custom_config custom;
    struct ev_timer_config timer;
    struct ev_keyboard_config keyboard;
    struct ev_mouse_config mouse;
};

enum ev_subscription_type {
    // empty
    ev_st_empty,
    // wakes a process
    ev_st_process
};

union ev_subscription_trigger {
    usize proc;
};

struct ev_subscription {
    enum ev_type event;
    union ev_subscription_trigger trigger;
    enum ev_subscription_type type;
    union ev_subscription_params params;
};

void subscribe(struct ev_subscription subscription);
void handle(enum ev_type event, bool(predicate)(union ev_subscription_params*));