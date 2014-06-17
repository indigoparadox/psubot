
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <msp430.h>

#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "pins.h"

/* = TI Header Fixes = */

#if ! defined(TACTL_) && ! defined(TACTL)
#define TACTL TA0CTL
#define TAR TA0R
#define TACCR0 TA0CCR0
#define TACCTL0 TA0CCTL0
#endif /* TACTL */
#ifndef TIMERA0_VECTOR
#define TIMERA0_VECTOR TIMER0_A0_VECTOR
#endif /* TIMERA0_VECTOR */

/* = Constants = */

#define BUZZER_MODE_CONTINUOUS MC_2
#define BUZZER_MODE_UP MC_1

/* = Structs = */

struct scheduler_task;
struct scheduler_task {
   void (*task)( uint8_t, int* );
   uint8_t (*shutdown)( uint8_t, int* );
   uint8_t argc;
   int* argi;
   uint8_t id;
   struct scheduler_task* next;
};

struct scheduler_buzz;
struct scheduler_buzz {
   /* Return TRUE until done. */
   PORT port;
   int pin;
   int period;
   int duty;
   int duration;
   int mode;
   /* TODO: Callback should be the ISR, shutdown should be shutdown. */
   void (*callback)( uint8_t, int* );
   BOOL (*finished)( uint8_t, int* );
   uint8_t argc;
   int* argi;
   struct scheduler_buzz* next;
};

/* = Function Prototypes = */

void scheduler_init( void );
uint8_t scheduler_add_task(
   void (*)( uint8_t, int* ), uint8_t (*)( uint8_t, int* ), uint8_t, int*
);
void scheduler_del_task( uint8_t );
void scheduler_buzz(
   PORT, int, int, int, int, int, void (*)( uint8_t, int* ),
   BOOL (*)( uint8_t, int* ), uint8_t, int*
);
void scheduler_buzzer_task( uint8_t, int* );
void scheduler_halt( void );

#endif /* SCHEDULER_H */

