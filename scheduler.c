
#include "scheduler.h"

struct scheduler_task* gps_timer_tasks = NULL;

int scheduler_count_tasks( void ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks;
   int i_task_count = 0;

   while( NULL != ps_task_iter ) {
      i_task_count++;
      ps_task_iter = ps_task_iter->next;
   }

   return i_task_count;
}

void scheduler_add_task(
   char* pc_id_in, void (*task_in)( int, int* ),
   void (*shutdown_in)( int, int* ), int i_argc_in, int* pi_argi_in
) {
   struct scheduler_task* ps_task_new,
      * ps_task_iter;

   /* Start the timer if a task was added and none were present. */
   if( 0 == scheduler_count_tasks() ) {
      WDTCTL = WDT_MDLY_0_064;
      IE1 |= WDTIE;
   }

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct scheduler_task ) );
   memset( ps_task_new, 0, sizeof( struct scheduler_task ) );
   ps_task_new->task = task_in;
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

void _scheduler_free_task( struct scheduler_task* ps_task_in ) {
   free( ps_task_in->id );
   free( ps_task_in->argi );
   free( ps_task_in );
   /* TODO: Do we free the task function pointer? */
}

void scheduler_del_task( const char* pc_id_in ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks,
      * ps_task_prev = NULL;

   /* Remove the task from the tasks list. */
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
      _scheduler_free_task( ps_task_iter );

      ps_task_prev = ps_task_iter;
      ps_task_iter = ps_task_iter->next;
   }

   /* Shut off the timer if no tasks are present. */
   if( 0 == scheduler_count_tasks() ) {
      /* TODO */
   }

}

/* Purpose: Halt all activity and blink the internal LED.                     */
void scheduler_halt( void ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks,
      * ps_task_remove = NULL;

   /* Remove all scheduled tasks. */
   while( NULL != ps_task_iter ) {
      ps_task_remove = ps_task_iter;
      ps_task_iter = ps_task_iter->next;
      _scheduler_free_task( ps_task_remove );
   }
   gps_timer_tasks = NULL;

   /* Disable all interrupts. */
   P1IE = 0;
   P2IE = 0;
   #ifdef IE2
   IE2 = 0;
   #endif /* IE2 */
   /*
   TACTL = 0;
   CCTL0 = 0;
   */

   /* Turn off all outputs. */
   P1OUT = 0;
   P2OUT = 0;

   /* Blink indefinitely. */
   pins_dir_or( ILED_PORT, ILED );
   for(;;) {
      pins_out_or( ILED_PORT, ILED );
      __delay_cycles( 250000 );
      pins_out_and( ILED_PORT, ~ILED );
      __delay_cycles( 250000 );
   }
}

#pragma vector=WDT_VECTOR
__interrupt void scheduler_wdt_isr( void ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks;

   while( NULL != ps_task_iter ) {
      (*ps_task_iter->task)( ps_task_iter->argc, ps_task_iter->argi );
      
      ps_task_iter = ps_task_iter->next;
   }
}

