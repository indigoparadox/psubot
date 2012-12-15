
#include "psubot.h"

int gi_debounce_counter_button = 0;

void psubot_init( void ) {

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

void psubot_eye_enable( void ) {

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
}

/* Parameters:                                                                *
 *    i_pos_in - The percentage to the left to position the eye.              */
void psubot_eye_pos( int i_pos_in ) {
   //int i_pos_target = 0;
   int i;

   /* Turn on the motor. */
   P1OUT |= EYE_ON;

   /* Move the eye to position zero. */
   P1OUT |= EYE_R;
   while( !(P2IN & EYE_SENSE) ) {
      SLEEP( 10 );
   }
   P1OUT &= ~EYE_R;

   /* Turn off the motor. */
   P1OUT &= ~EYE_ON;

   psubot_eye_left( i_pos_in );
}

/* Purpose: Push the eye to the robot's left by the given increment.          */
void psubot_eye_left( int i_pos_in ) {
   int i;

   /* Turn on the motor. */
   P1OUT |= EYE_ON;

   /* Move the eye to the selected position. */
   P1OUT |= EYE_L;
   for( i = 0 ; i < i_pos_in ; i++ ) {}
   P1OUT &= ~EYE_L;

   /* Turn off the motor. */
   P1OUT &= ~EYE_ON;
}

/* Purpose: Push the eye to the robot's right by the given increment.         */
void psubot_eye_right( int i_pos_in ) {
   int i;

   /* Turn on the motor. */
   P1OUT |= EYE_ON;

   /* Move the eye to the selected position. */
   P1OUT |= EYE_R;
   for( i = 0 ; i < i_pos_in ; i++ ) {}
   P1OUT &= ~EYE_R;

   /* Turn off the motor. */
   P1OUT &= ~EYE_ON;
}

/* Purpose: Wait for stimuli.                                                 */
void psubot_wait( void ) {

   for( ; ; ) {
      /* Go to sleep. */
      __bis_SR_register( LPM3_bits + GIE );
   }
}

