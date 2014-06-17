
#ifndef BEEP_H
#define BEEP_H

#include <stdlib.h>

#include "scheduler.h"

/* = Constants = */

typedef enum {
   BEEP_NULL = 0,
   BEEP_C = 1900,
   BEEP_D = 1700,
   BEEP_E = 1500,
   BEEP_F = 1300,
   BEEP_G = 1250,
   BEEP_A = 1200,
   BEEP_B = 1050
} BEEP_NOTE;

/* = Function Prototypes = */

void beep_init( void );
void beep( int, int );
void beep_string( BEEP_NOTE*, int );

#endif /* BEEP_H */

