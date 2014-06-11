
#ifndef DRIVE_H
#define DRIVE_H

#include "pins.h"
#include "scheduler.h"

/* = Constants = */

typedef enum {
   DRIVING_STOPPED,
   DRIVING_FORWARD,
   DRIVING_REVERSE,
   DRIVING_RIGHT,
   DRIVING_LEFT,
   DRIVING_RIGHT_PIVOT,
   DRIVING_LEFT_PIVOT
} DRIVING_DIR;

/* = Function Prototypes = */

void drive_wheels_enable( void );
void drive_wheel_drive( DRIVING_DIR );

#endif /* DRIVE_H */

