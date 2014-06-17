
#ifndef BEEP_H
#define BEEP_H

#include <stdlib.h>

#include "scheduler.h"

/* = Constants = */

typedef enum {
   BEEP_NULL = 0,
   BEEP_C = 76,
   BEEP_D = 67,
   BEEP_E = 60,
   BEEP_F = 56,
   BEEP_G = 50,
   BEEP_A = 44,
   BEEP_B = 32
} BEEP_NOTE;

/* = Function Prototypes = */

void beep_init( void );
void beep( int, int );
void beep_string( BEEP_NOTE*, int );

#endif /* BEEP_H */

