
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
   void (*task)( int, int* );
   void (*shutdown)( int, int* );
   int argc;
   int* argi;
   char* id;
   struct scheduler_task* next;
};

/* = Function Prototypes = */

void* scheduler_realloc( void*, size_t );
int scheduler_count_tasks( void );
void scheduler_add_task(
   char*, void (*task_in)( int, int* ), void (*shutdown_in)( int, int* ), int,
   int*
);
void scheduler_del_task( const char* );
void scheduler_halt( void );

#endif /* SCHEDULER_H */

