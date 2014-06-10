
#ifndef EYE_H
#define EYE_H

#include <stdint.h>

#include "core.h"
#include "pins.h"

/* = Constants = */

typedef enum {
   EYE_RIGHT,
   EYE_LEFT,
   EYE_STOPPED
} EYE_DIR;

/* = Function Prototypes = */

void eye_enable( void );
void eye_pos( int );
void eye_move( EYE_DIR );

#endif /* EYE_H */

