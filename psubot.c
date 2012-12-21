
#include "psubot.h"

#include <stdint.h>

//int gi_debounce_counter_button = 0;
BOOL gb_sleeping = 0;
int gi_eye_move_loops = 0;

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
   uint8_t i_sense_count = 0;

   /* Move the eye to position zero. */
   P1OUT |= EYE_R;
   while( 
      !(P1IN & EYE_SENSE && i_sense_count >= 200) && 
      EYE_MAX_LOOPS_R > gi_eye_move_loops
   ) {
      __delay_cycles( 10 );
      gi_eye_move_loops++;
      
      /* Don't count the sensor as being held down unless it's being held     *
       * down!                                                                */
      if( P1IN && EYE_SENSE ) {
         i_sense_count++;
      } else {
         i_sense_count = 0;
      }
   }
   P1OUT &= ~EYE_R;

   /* Make sure we didn't just time out. */
   if( EYE_MAX_LOOPS_R <= gi_eye_move_loops ) {
      psubot_halt();
   } else {
      gi_eye_move_loops = 0;
   }

   psubot_eye_left( i_pos_in );
}

/* Purpose: Push the eye to the robot's left by the given increment.          */
void psubot_eye_left( int i_pos_in ) {
   long l_target_pos,
      l_pos_in = i_pos_in,
      i;

   l_target_pos = l_pos_in * EYE_MAX_CYCLES_L / 100;

   #if 0
   char ac_pos_out[5] = { '\0' };
   uart_echo( "\n\rOUT: " );
   uart_echo( ltoa( l_target_pos, ac_pos_out, 10 ) );
   uart_echo( "\n\r" );
   #endif

   /* Move the eye to the selected position. */
   P1OUT |= EYE_L;
   /* TODO: Use a timer for this? */
   for( i = 0 ; i < l_target_pos ; i++ ) {}
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

/* Purpose: Halt all activity and blink the internal LED.                     */
void psubot_halt( void ) {
   
   /* Disable all interrupts. */
   P1IE = 0;
   P2IE = 0;
   IE2 = 0;

   /* Turn off all outputs. */
   P1OUT = 0;
   P2OUT = 0;

   /* Blink indefinitely. */
   P1DIR |= ILED;
   for(;;) {
      P1OUT |= ILED;
      __delay_cycles( 250000 );
      P1OUT &= ~ILED;
      __delay_cycles( 250000 );
   }
}

