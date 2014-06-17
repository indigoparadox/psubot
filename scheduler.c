
#include "scheduler.h"

struct scheduler_task* gps_timer_tasks = NULL;
uint8_t gi_timer_tasks_count = 0;
struct scheduler_buzz* gps_buzzes = NULL;
uint8_t gi_buzzes_count = 0;

void scheduler_init( void ) {
   #ifdef CALBC1_8MHZ
   /* Try to go as fast as possible. */
   if( 0xff != CALBC1_8MHZ ) {
      DCOCTL = 0;
      BCSCTL1 = CALBC1_8MHZ;
      DCOCTL = CALDCO_8MHZ;

   } else {
      scheduler_halt();
   }
   #endif /* CALBC1_8MHZ */

   /* The buzz queue executor operates as a task with the other callback      *
    * timer tasks.                                                            */
   scheduler_add_task( scheduler_buzzer_task, NULL, 0, NULL );
}

uint8_t scheduler_add_task(
   void (*task_in)( uint8_t, int* ),
   uint8_t (*shutdown_in)( uint8_t, int* ), uint8_t i_argc_in, int* pi_argi_in
) {
   struct scheduler_task* ps_task_new,
      * ps_task_iter;

   /* Start the timer if a task was added and none were present. */
   if( 0 == gi_timer_tasks_count ) {
      WDTCTL = WDTPW | WDT_MDLY_0_5;
      IE1 |= WDTIE;
   }

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct scheduler_task ) );
   ps_task_new->task = task_in;
   ps_task_new->shutdown = shutdown_in;
   ps_task_new->argc = i_argc_in;
   ps_task_new->argi = pi_argi_in;
   ps_task_new->id = gi_timer_tasks_count;
   ps_task_new->next = NULL;

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
         /* Iterate. */
         ps_task_prev = ps_task_iter;
         ps_task_iter = ps_task_iter->next;
         continue;
      }

      /* Are we at the start of the list? */
      if( NULL == ps_task_prev ) {
         gps_timer_tasks = ps_task_iter->next;
      } else {
         ps_task_prev->next = ps_task_iter->next;
      }

      /* Run the shutdown function if there is one. */
      if( NULL != (*ps_task_iter->shutdown) ) {
         (*ps_task_iter->shutdown)( ps_task_iter->argc, ps_task_iter->argi );
      }

      /* Free the task. */
      _scheduler_free_task( ps_task_iter );

      /* Iterate. */
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

/*
 * i_finished_in - An optional callback to check if we should stop buzzing.   *
 *                 Useful for more complicated buzzes. Only called if the     *
 *                 duration is > 0.                                           */
void scheduler_buzz(
   PORT i_port_in, int i_pin_in, int i_period_in, int i_duty_in,
   int i_duration_in, int i_mode_in, void (*callback_in)( uint8_t, int* ),
   BOOL (*finished_in)( uint8_t, int* ), uint8_t i_argc_in, int* pi_argi_in
) {
   struct scheduler_buzz* ps_buzz_new,
      * ps_buzz_iter;

   /* Create buzz in queue. */
   ps_buzz_new = malloc( sizeof( struct scheduler_buzz ) );
   ps_buzz_new->port = i_port_in;
   ps_buzz_new->pin = i_pin_in;
   ps_buzz_new->period = i_period_in;
   ps_buzz_new->duty = i_duty_in;
   ps_buzz_new->duration = i_duration_in;
   ps_buzz_new->mode = i_mode_in;
   ps_buzz_new->callback = callback_in;
   ps_buzz_new->finished = finished_in;
   ps_buzz_new->argc = i_argc_in;
   ps_buzz_new->argi = pi_argi_in;
   ps_buzz_new->next = NULL;

   /* Add the buzz to the end of the buzz list. */
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
}

static void _scheduler_free_buzz( struct scheduler_buzz* ps_buzz_in ) {
   free( ps_buzz_in->argi );
   free( ps_buzz_in );
   /* TODO: Do we free the task function pointer? */
}

/* Purpose: Start/stop buzzes as indicated by the queue.                      */
void scheduler_buzzer_task( uint8_t i_argc_in, int* pi_argi_in ) {
   static int i_buzzer_locked = FALSE;
   struct scheduler_buzz* ps_buzz_iter = gps_buzzes;

   if( NULL != ps_buzz_iter ) {
      /* Operate on the first buzz in line. */

      if(
         (
            /* If buzzer duration is negative, and there is a callback, call  *
             * the callback to see if we're done buzzing.                     */
            0 > ps_buzz_iter->duration && 
            NULL != ps_buzz_iter->finished &&
            FALSE == (*ps_buzz_iter->finished)( 
               ps_buzz_iter->argc,
               ps_buzz_iter->argi
            )
         ) || (
            /* If buzzer duration is out or negative and there is no          *
             * callback, we must be done buzzing.                             */
            NULL == ps_buzz_iter->finished &&
            0 >= ps_buzz_iter->duration
         )
      ) {
         /* If we're at the end of a duration, stop buzzing. */
         switch( ps_buzz_iter->port ) {
            case PORT1:
               P1SEL &= ~(ps_buzz_iter->pin);
               break;
            case PORT2:
               P2SEL &= ~(ps_buzz_iter->pin);
               break;
         }

         /* Remove the task from the buzz queue. */
         gps_buzzes = ps_buzz_iter->next;
         _scheduler_free_buzz( ps_buzz_iter );
         ps_buzz_iter = NULL;
         i_buzzer_locked = FALSE;

      } else if( !i_buzzer_locked ) {
         /* Start the next buzz. */
         i_buzzer_locked = TRUE;
         switch( ps_buzz_iter->port ) {
            case PORT1:
               P1SEL |= ps_buzz_iter->pin;
               break;
            case PORT2:
               P2SEL |= ps_buzz_iter->pin;
               break;
         }
         CCR0 = ps_buzz_iter->period;
         if( 0 < ps_buzz_iter->duty ) {
            CCR1 = ps_buzz_iter->duty;
         }
         CCTL0 = OUT;
         CCTL1 |= OUTMOD_7;
         TACTL = TASSEL_2 + ps_buzz_iter->mode;
      }

      /* Start or continue counting down. */
      /* TODO: Adjust time consumed for items in task queue? */
      if( NULL != ps_buzz_iter && 0 < ps_buzz_iter->duration ) {
         ps_buzz_iter->duration--;
      }
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

   /* TODO: Remove all scheduled buzzes. */

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
__interrupt void scheduler_timera0_isr( void ) {
   if( NULL != gps_buzzes && NULL != gps_buzzes->callback ) {
      (*gps_buzzes->callback)( gps_buzzes->argc, gps_buzzes->argi );
   }
}

