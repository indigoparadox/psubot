
#include "psubot.h"

int gi_debounce_counter_button = 0;
BOOL gb_sleeping = 0;

void psubot_init( void ) {

   /* Turn off all indicators.*/
   P1OUT = 0;
   P2OUT = 0;
}

void psubot_eye_enable( void ) {

   /* Setup Motor I/O */
   P1DIR = 0;
   P1DIR |= EYE_R;
   P1DIR |= EYE_L;
   
   /* Setup LED I/O */
   P2DIR = 0;
   P2DIR |= LED_RED;
   P2DIR |= LED_GREEN;
   P2DIR |= LED_BLUE;
}

/* Parameters:                                                                *
 *    i_pos_in - The percentage to the left to position the eye.              */
void psubot_eye_pos( int i_pos_in ) {

   /* Move the eye to position zero. */
   P1OUT |= EYE_R;
   while( !(P1IN & EYE_SENSE) ) {
      __delay_cycles( 10 );
   }
   P1OUT &= ~EYE_R;

   psubot_eye_left( i_pos_in );
}

/* Purpose: Push the eye to the robot's left by the given increment.          */
void psubot_eye_left( int i_pos_in ) {
   int i;

   /* Move the eye to the selected position. */
   P1OUT |= EYE_L;
   /* TODO: Use a timer for this? */
   for( i = 0 ; i < i_pos_in ; i++ ) {}
   P1OUT &= ~EYE_L;
}

/* Purpose: Push the eye to the robot's right by the given increment.         */
void psubot_eye_right( int i_pos_in ) {
   int i;

   /* Move the eye to the selected position. */
   P1OUT |= EYE_R;
   /* TODO: Use a timer for this? */
   for( i = 0 ; i < i_pos_in ; i++ ) {}
   P1OUT &= ~EYE_R;
}

