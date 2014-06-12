
#include "scheduler.h"

struct scheduler_task* gps_timer_tasks = NULL;
uint8_t gi_timer_tasks_count = 0;
struct scheduler_buzz* gps_buzzes = NULL;
uint8_t gi_buzzes_count = 0;

/* XXX: Change this to a global int. */
#if 0
int scheduler_count_tasks( void ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks;
   int i_task_count = 0;

   while( NULL != ps_task_iter ) {
      i_task_count++;
      ps_task_iter = ps_task_iter->next;
   }

   return i_task_count;
}
#endif

uint8_t scheduler_add_task(
   void (*task_in)( uint8_t, int* ),
   uint8_t (*shutdown_in)( uint8_t, int* ), uint8_t i_argc_in, int* pi_argi_in
) {
   struct scheduler_task* ps_task_new,
      * ps_task_iter;

   /* Start the timer if a task was added and none were present. */
   if( 0 == gi_timer_tasks_count ) {
      WDTCTL = WDTPW | WDT_MDLY_0_064;
      IE1 |= WDTIE;
      __enable_interrupt();
   }

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct scheduler_task ) );
   memset( ps_task_new, 0, sizeof( struct scheduler_task ) );
   ps_task_new->task = task_in;
   ps_task_new->shutdown = shutdown_in;
   ps_task_new->argc = i_argc_in;
   ps_task_new->argi = pi_argi_in;
   ps_task_new->id = gi_timer_tasks_count;

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

   gi_timer_tasks_count++;
   return ps_task_new->id;
}

void _scheduler_free_task( struct scheduler_task* ps_task_in ) {
   free( ps_task_in->argi );
   free( ps_task_in );
   /* TODO: Do we free the task function pointer? */
}

void scheduler_del_task( uint8_t i_id_in ) {
   struct scheduler_task* ps_task_iter = gps_timer_tasks,
      * ps_task_prev = NULL;

   /* Remove the task from the tasks list. */
   while( NULL != ps_task_iter ) {
      if( ps_task_iter->id != i_id_in ) {
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

   gi_timer_tasks_count--;

   /* Shut off the timer if no tasks are present. */
   if( 0 == gi_timer_tasks_count ) {
      IE1 &= ~WDTIE;
      WDTCTL = WDTPW | WDTHOLD;
   }
}

uint8_t scheduler_add_buzz(
   void (*buzz_in)( uint8_t, int* ), uint8_t i_argc_in, int* pi_argi_in
) {
   struct scheduler_buzz* ps_buzz_new,
      * ps_buzz_iter;

   /* Start the timer if a task was added and none were present. */
   if( 0 == gi_buzzes_count ) {
      CCTL0 = CCIE;
      TACCR0 = 10000;
      TACTL = TASSEL_2 + MC_1;
      __enable_interrupt();
   }

   /* Create the new task. */
   ps_buzz_new = malloc( sizeof( struct scheduler_buzz ) );
   memset( ps_buzz_new, 0, sizeof( struct scheduler_buzz ) );
   ps_buzz_new->task = buzz_in;
   ps_buzz_new->argc = i_argc_in;
   ps_buzz_new->argi = pi_argi_in;
   ps_buzz_new->id = gi_buzzes_count;

   /* Add the task to the tasks list. */
   if( NULL == gps_buzzes ) {
      gps_buzzes = ps_buzz_new;
   } else {
      ps_buzz_iter = gps_buzzes;
      while( NULL != ps_buzz_iter->next ) {
         ps_buzz_iter = ps_buzz_iter->next;
      }
      ps_buzz_iter->next = ps_buzz_new;
   }
   
   gi_buzzes_count++;
   return ps_buzz_new->id;
}

void scheduler_del_buzz( uint8_t i_id_in ) {

   gi_buzzes_count--;
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
   IE1 &= ~WDTIE;
   WDTCTL = WDTPW | WDTHOLD;

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

#pragma vector=TIMERA0_VECTOR
__interrupt void scheduler_timer0_isr( void ) {

}

