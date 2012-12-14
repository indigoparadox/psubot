
#ifndef PSUBOT_H
#define PSUBOT_H

#include <msp430g2553.h>

#define LED_RED   BIT0
#define LED_GREEN BIT1
#define LED_BLUE  BIT2

#define BUTTON    BIT3

/* Eye directions are from the robot's point of view. */

#define EYE_R     BIT4
#define EYE_L     BIT3
#define EYE_ON    BIT5

/* 5 = 1 second */
#define DEBOUNCE_TIMEOUT   2

/* = Function Prototypes = */

void psubot_init( void );
void psubot_button_enable( void );
void psubot_wait( void );

#endif /* PSUBOT_H */

