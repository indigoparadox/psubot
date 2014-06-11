
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
   char* pc_id_in, void (*thread_in)( int, int* ),
   void (*shutdown_in)( int, int* ), int i_argc_in, int* pi_argi_in
) {
   struct scheduler_task* ps_task_new,
      * ps_task_iter;

   /* Start the timer if a thread was added and none were present. */
   if( 0 == scheduler_count_threads() ) {
      CCTL0 = CCIE;
      TACCR0 = 10000;
      TACTL = TASSEL_2 + MC_1;
      __enable_interrupt();
   }

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct scheduler_task ) );
   memset( ps_task_new, 0, sizeof( struct scheduler_task ) );
   ps_task_new->task = thread_in;
   ps_task_new->shutdown = shutdown_in;
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
   struct scheduler_task* ps_task_iter = gps_timer_tasks,
      * ps_task_prev = NULL;

   /* Remove the thread from the threads list. */
   while( NULL != ps_task_iter ) {
      if( 0 != strcmp( ps_task_iter->id, pc_id_in ) ) {
         continue;
      }

      /* Are we at the start of the list? */
      if( NULL == ps_task_prev ) {
         gps_timer_tasks = ps_task_iter->next;
      } else {
         ps_task_prev->next = ps_task_iter->next;
      }

      /* Run the shutdown function. */
      (*ps_task_iter->shutdown)( ps_task_iter->argc, ps_task_iter->argi );

      /* Free the task. */
      free( ps_task_iter->id );
      free( ps_task_iter->argi );
      free( ps_task_iter );
      /* TODO: Do we free the task function pointer? */

      ps_task_prev = ps_task_iter;
      ps_task_iter = ps_task_iter->next;
   }

   /* TODO: Shut off the timer if no threads are present. */

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

