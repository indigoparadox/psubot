
#ifndef EYE_H
#define EYE_H

#include <stdint.h>

#include "core.h"
#include "pins.h"
#include "scheduler.h"

/* = Constants = */

typedef enum {
   EYE_RIGHT,
   EYE_LEFT,
   EYE_STOPPED
} EYE_DIR;

typedef enum {
   EYE_RED,
   EYE_GREEN,
   EYE_BLUE
} EYE_COLOR;

/* = Function Prototypes = */

void eye_enable( void );
void eye_pos( int );
void eye_move( EYE_DIR );
void eye_glow( EYE_COLOR, int );
void eye_glow_task( int, int* );

#endif /* EYE_H */
