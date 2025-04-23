#include "scheduler.h"
#include "config.h"
#include "ctx.h"
#include "eaos.h"
#include "log.h"
#include "proc.h"

// the S.T.U.P.I.D. scheduler!
// - Spinning
// - Troubled
// - Unapologetic
// - Perfectly InDecisive

extern void schedule(void);

void schedule_inner(u64 caller_rbp, u64 caller_rsp, u64 original_ret_addr) {
   struct proc *thisproc = &get_proc_table()[get_proc_id()]; 

   save_ctx(thisproc->ctx);
   thisproc->status = proc_status_waiting;

   thisproc->ctx->rbp = caller_rbp;
   thisproc->ctx->rip = original_ret_addr;
   thisproc->ctx->rsp = caller_rsp;

   schedule_spin();
}

void wake_eventually(usize pid) {
   get_proc_table()[pid].status = proc_status_ready;
}

void scheduler_step(void) {
   // check for any processes that need to run!
   for (usize i = 0; i < NPROC; i++) {
      struct proc *p = &get_proc_table()[i];
      if (p->status == proc_status_ready) {
         wake(p->id);
      } 
   }
}

__attribute__((noreturn))
void schedule_spin(void) {
   while (true) {
      scheduler_step();
      __asm__ volatile ("hlt");
   }
}