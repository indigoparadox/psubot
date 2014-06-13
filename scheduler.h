
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
   BOOL (*buzz)( uint8_t, int* );
   uint8_t argc;
   int* argi;
   uint8_t id;
   struct scheduler_buzz* next;
};

/* = Function Prototypes = */

void scheduler_init( void );
uint8_t scheduler_add_task(
   void (*)( uint8_t, int* ), uint8_t (*)( uint8_t, int* ), uint8_t, int*
);
void scheduler_del_task( uint8_t );
uint8_t scheduler_buzz( BOOL (*)( uint8_t, int* ), uint8_t, int*, BOOL );
void scheduler_halt( void );

#endif /* SCHEDULER_H */

