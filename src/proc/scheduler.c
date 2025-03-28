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

void schedule() {
   struct proc *thisproc = &get_proc_table()[get_proc_id()]; 

   save_ctx(thisproc->ctx);
   thisproc->status = proc_status_waiting;

   // warning: taking the address of a label like this is nonstandard C!
   thisproc->ctx->rip = (u64) &&return_here;

   schedule_spin();

   // this should be unreachable by normal execution flow
   return_here:
   kinfo("h");
}

void wake_eventually(usize pid) {
   get_proc_table()[pid].status = proc_status_ready;
}

void scheduler_step(void) {
   // check for any processes that need to run!
   for (usize i = 0; i < NPROC; i++) {
      struct proc *p = &get_proc_table()[i];
      if (p->status == proc_status_ready) {
         kinfo("WAKING");
         wake(p->id);
      } 
   }
}

void schedule_spin(void) {
   while (true) {
      scheduler_step();
      __asm__ volatile ("hlt");
   }
}