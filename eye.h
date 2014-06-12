
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

#define EYE_DUTY_MAX 10000
#define EYE_DUTY_HALF EYE_DUTY_MAX / 2

/* = Function Prototypes = */

void eye_enable( void );
void eye_pos( int );
void eye_move( EYE_DIR );
void eye_glow( EYE_COLOR, int );
void eye_glow_task( uint8_t, int* );
uint8_t eye_glow_shutdown( uint8_t, int* );

#endif /* EYE_H */

