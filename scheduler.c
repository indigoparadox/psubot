
#include "scheduler.h"

struct scheduler_task* gps_timer_tasks = NULL;

int scheduler_count_threads( void ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks;
   int i_thread_count = 0;

   while( NULL != ps_task_iter ) {
      i_thread_count++;
      ps_task_iter = ps_task_iter->next;
   }

   return i_thread_count;
}

void scheduler_add_thread(
   char* pc_id_in, void (*thread_in)( int, int* ), int i_argc_in,
   int* pi_argi_in
) {
   struct scheduler_task* ps_task_new,
      * ps_task_iter;

   /* Start the timer if a thread was added and none were present. */
   if( 0 == scheduler_count_threads() ) {
      CCTL0 = CCIE;
      TACTL = TASSEL_2 + MC_2;
   }

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct scheduler_task ) );
   memset( ps_task_new, 0, sizeof( struct scheduler_task ) );
   ps_task_new->task = thread_in;
   ps_task_new->argc = i_argc_in;
   ps_task_new->argi = pi_argi_in;
   ps_task_new->id = pc_id_in;

   /* Add the task to the tasks list. */
   if( NULL == gps_timer_tasks ) {
      gps_timer_tasks = ps_task_new;
   } else {
      ps_task_iter = gps_timer_tasks;
      while( NULL != ps_task_iter->next ) {
         ps_task_iter = ps_task_iter->next;
      }
      ps_task_iter->next = ps_task_new;
   }
}

void scheduler_del_thread( const char* pc_id_in ) {

   /* TODO: Shut off the timer if no threads are present. */

   /* TODO: Remove the thread from the threads list. */

}

void scheduler_halt( void ) {

}

#pragma vector=TIMERA0_VECTOR
__interrupt void scheduler_timer0_isr( void ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks;

   while( NULL != ps_task_iter ) {
      (*ps_task_iter->task)( ps_task_iter->argc, ps_task_iter->argi );
      
      ps_task_iter = ps_task_iter->next;
   }
}

