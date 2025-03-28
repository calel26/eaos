#include "event.h"
#include "config.h"
#include "log.h"
#include "proc.h"
#include "scheduler.h"

// FIXME: this is another big (~12KB) in-binary data structure that should
// ideally be runtime-allocated.
static struct ev_subscription subscriptions[event_count][NPROC];

void subscribe(struct ev_subscription subscription) {
    // this is kind of a hack. every process gets exactly one subscription to each event
    usize next_id = subscription.trigger.proc;

    // check if this event slot is occupied
    if (subscriptions[subscription.event][next_id].type != ev_st_empty) {
        kpanic("process tried to double-subscribe to event");
    }

    subscriptions[subscription.event][next_id] = subscription;
}

void handle(enum ev_type event, bool (*predicate)(union ev_subscription_params*)) {
    for (usize i = 0; i < sizeof(subscriptions[0]) / sizeof(subscriptions[0][0]); i++) {
        struct ev_subscription *s = &subscriptions[event][i];
        if (s->type == ev_st_empty) continue;
        if (predicate(&s->params)) {
            // clear subscription
            // is it really a subscription if it only happens once? oh well....
            // save a copy.
            enum ev_subscription_type ty = subscriptions[event][i].type;
            subscriptions[event][i].type = ev_st_empty;

            switch (ty) {
                case ev_st_empty:
                    kpanic("unreachable");
                    break;
                case ev_st_process:
                    wake_eventually(s->trigger.proc);
                    break;
            }
        }
    }
}