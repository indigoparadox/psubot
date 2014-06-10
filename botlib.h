
#ifndef PSUBOT_H
#define PSUBOT_H

#include <msp430.h>

#include "core.h"
#include "pins.h"

/* = Function Prototypes = */

void psubot_init( void );
void psubot_button_enable( void );
void psubot_wait( void );
void psubot_halt( void );

#endif /* PSUBOT_H */

