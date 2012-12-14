
#include "psubot.h"

void psubot_init( void ) {

   /* Setup Motor I/O */
   P1DIR = 0;
   P1DIR |= EYE_R;
   P1DIR |= EYE_L;
   P1DIR |= EYE_ON;
   
   /* Setup LED I/O */
   P2DIR = 0;
   P2DIR |= LED_RED;
   P2DIR |= LED_GREEN;
   P2DIR |= LED_BLUE;

   /* Turn off all indicators.*/
   P1OUT = 0;
   P2OUT = 0;
}

void psubot_button_enable( void ) {

   /* Enable button interrupt. */
   P2IFG &= ~BUTTON;
   P2REN |= BUTTON;
   P2IE |= BUTTON;

   /* Listen for button release. */
   P2IES &= ~BUTTON;
}

/* Purpose: Wait for stimuli.                                                 */
void psubot_wait( void ) {

   for( ; ; ) {
      /* Go to sleep. */
      __bis_SR_register( LPM3_bits + GIE );
   }
}

