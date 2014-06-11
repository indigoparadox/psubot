
#include "scheduler.h"

void (*gpf_timer_threads[])( int ) = { NULL };
int gpi_thread_args[] = { -1 };
char* gpc_timer_thread_ids[] = { NULL };

void* scheduler_realloc( void* old_ptr_in, size_t size_in ) {
   
}

int scheduler_count_threads( void ) {
   char* pc_id_iter = gpc_timer_thread_ids[0];
   int i_thread_count = 0;

   while( NULL != *pc_id_iter ) {
      i_thread_count++;
      pc_id_iter++;
   }

   return i_thread_count;
}

void scheduler_add_thread( const char* pc_id_in, void (*thread_in)( int ) ) {
   int i_old_thread_count;


   i_old_thread_count = scheduler_count_threads();

   /* TODO: Add the thread to the threads list. */
   gpf_timer_threads = scheduler_realloc( gpf_timer_threads, 30 );

   /* TODO: Add the arg to the args list. */

   if( 0 == i_old_thread_count ) {
      /* Start the timer if a thread was added and none were present. */
      CCTL0 = CCIE;
      TACTL = TASSEL_2 + MC_2;
   }
}

void scheduler_del_thread( const char* pc_id_in ) {

   /* TODO: Shut off the timer if no threads are present. */

   /* TODO: Remove the thread from the threads list. */

   /* TODO: Remove the arg from the args list. */
}

#pragma vector=TIMERA0_VECTOR
__interrupt void scheduler_timer0_isr( void ) {
   void (*pf_thread_iter)( int ) = gpf_timer_threads[0];
   int* pi_arg_iter = &gpi_thread_args[0];

   while( NULL != *pf_thread_iter ) {
      (*pf_thread_iter)( *pi_arg_iter );
      
      pf_thread_iter++;
      pi_arg_iter++;
   }
}

