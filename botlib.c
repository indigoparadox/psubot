
#include "botlib.h"

#include <stdint.h>

//int gi_debounce_counter_button = 0;
BOOL gb_sleeping = 0;
int gi_eye_move_loops = 0;
DRIVING_DIR ge_driving_dir = DRIVING_STOPPED;

void psubot_init( void ) {

   /* Turn off all indicators.*/
   P1OUT = 0;
   P2OUT = 0;

   #if 0
   /* This should be used on button inputs to debounce or something,          *
    * probably. Our original intention was to use it to control wheel drivine *
    * before we switched to the new continuous driving model. The timer       *
    * itself conflicts with the low power modes.                              */

   /* Enable the maintenance timer. */
   WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL;
   /* WDTCTL = WDT_ADLY_1000; */
   IE1 |= WDTIFG;
   /* BCSCTL2 = DIVS_2; */
   _EINT();
   #else
   WDTCTL = WDTPW | WDTHOLD;
   #endif
}

int _psubot_pin_in( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         return P1IN & i_bit_in;

      case PORT2:
         return P2IN & i_bit_in;
   }

   return 0;
}

void _psubot_pin_dir_or( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         P1DIR |= i_bit_in;
         break;

      case PORT2:
         P2DIR |= i_bit_in;
         break;
   }
}

void _psubot_pin_out_or( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         P1OUT |= i_bit_in;
         break;

      case PORT2:
         P2OUT |= i_bit_in;
         break;
   }
}

void _psubot_pin_out_and( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         P1OUT &= i_bit_in;
         break;

      case PORT2:
         P2OUT &= i_bit_in;
         break;
   }
}

void psubot_eye_enable( void ) {

   /* Setup Motor I/O */
   P1DIR = 0;
   _psubot_pin_dir_or( EYE_R_PORT, EYE_R );
   _psubot_pin_dir_or( EYE_L_PORT, EYE_L );
   
   /* Setup LED I/O */
   P2DIR = 0;
   _psubot_pin_dir_or( LED_RED_PORT, LED_RED );
   _psubot_pin_dir_or( LED_GREEN_PORT, LED_GREEN );
   _psubot_pin_dir_or( LED_BLUE_PORT, LED_BLUE );
}

void psubot_wheels_enable( void ) {
   _psubot_pin_dir_or( WHEEL_R_F_PORT, WHEEL_R_F );
   _psubot_pin_dir_or( WHEEL_L_F_PORT, WHEEL_L_F );
   _psubot_pin_dir_or( WHEEL_L_R_PORT, WHEEL_L_R );
   _psubot_pin_dir_or( WHEEL_R_R_PORT, WHEEL_R_R );
}

/* Parameters:                                                                *
 *    i_pos_in - The percentage to the left to position the eye.              */
void psubot_eye_pos( int i_pos_in ) {
   uint8_t i_sense_count = 0;
   long l_target_pos,
      l_pos_in = i_pos_in,
      i;

   /* Figure out the position as a percentage of the eye's max range. */
   l_target_pos = l_pos_in * EYE_MAX_CYCLES_L / 100;

   /* Move the eye to position zero. */
   _psubot_pin_out_or( EYE_R_PORT, EYE_R );
   while( 
      !(_psubot_pin_in( EYE_SENSE_PORT, EYE_SENSE ) && i_sense_count >= 200) && 
      EYE_MAX_LOOPS_R > gi_eye_move_loops
   ) {
      __delay_cycles( 10 );
      gi_eye_move_loops++;
      
      /* Don't count the sensor as being held down unless it's being held     *
       * down!                                                                */
      if( _psubot_pin_in( EYE_SENSE_PORT, EYE_SENSE ) ) {
         i_sense_count++;
      } else {
         i_sense_count = 0;
      }
   }
   _psubot_pin_out_and( EYE_R_PORT, ~EYE_R );

   /* Make sure we didn't just time out. */
   if( EYE_MAX_LOOPS_R <= gi_eye_move_loops ) {
      psubot_halt();
   } else {
      gi_eye_move_loops = 0;
   }

   psubot_eye_move( EYE_LEFT );
   for( i = 0 ; i < l_target_pos ; i++ ) {}
   psubot_eye_move( EYE_STOPPED );
}

/* Purpose: Push the eye to the robot's left by the given increment.          */
/* TODO: Keep a global variable with the current position of the eye. Set it  *
 *       to 0 during the eye init phase and increment/decrement it whenever   *
 *       the eye is moved. Don't allow the eye to move out of its operational *
 *       range.                                                               */
void psubot_eye_move( EYE_DIR e_eye_dir_in ) {
   switch( e_eye_dir_in ) {
      case EYE_RIGHT:
         _psubot_pin_out_or( EYE_R_PORT, EYE_R );
         break;
      case EYE_LEFT:
         _psubot_pin_out_or( EYE_L_PORT, EYE_L );
         break;
      case EYE_STOPPED:
         _psubot_pin_out_and( EYE_L_PORT, ~EYE_L );
         _psubot_pin_out_and( EYE_R_PORT, ~EYE_R );
         break;
   }
}

void psubot_wheel_drive( DRIVING_DIR e_direction_in ) {
   switch( e_direction_in ) {
      case DRIVING_FORWARD:
         _psubot_pin_out_or( WHEEL_R_F_PORT, WHEEL_R_F );
         _psubot_pin_out_or( WHEEL_L_F_PORT, WHEEL_L_F );
         break;
      case DRIVING_REVERSE:
         _psubot_pin_out_or( WHEEL_L_R_PORT, WHEEL_L_R );
         _psubot_pin_out_or( WHEEL_R_R_PORT, WHEEL_R_R );
         break;
      case DRIVING_RIGHT:
         _psubot_pin_out_or( WHEEL_L_F_PORT, WHEEL_L_F );
         break;
      case DRIVING_RIGHT_PIVOT:
         _psubot_pin_out_or( WHEEL_L_F_PORT, WHEEL_L_F );
         _psubot_pin_out_or( WHEEL_R_R_PORT, WHEEL_R_R );
         break;
      case DRIVING_LEFT:
         _psubot_pin_out_or( WHEEL_R_F_PORT, WHEEL_R_F );
         break;
      case DRIVING_LEFT_PIVOT:
         _psubot_pin_out_or( WHEEL_R_F_PORT, WHEEL_R_F );
         _psubot_pin_out_or( WHEEL_L_R_PORT, WHEEL_L_R );
         break;
      case DRIVING_STOPPED:
         _psubot_pin_out_and( WHEEL_R_F_PORT, ~WHEEL_R_F );
         _psubot_pin_out_and( WHEEL_L_F_PORT, ~WHEEL_L_F );
         _psubot_pin_out_and( WHEEL_R_R_PORT, ~WHEEL_R_R );
         _psubot_pin_out_and( WHEEL_L_R_PORT, ~WHEEL_L_R );
      default:
         return;
   }
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
   _psubot_pin_dir_or( ILED_PORT, ILED );
   for(;;) {
      _psubot_pin_out_or( ILED_PORT, ILED );
      __delay_cycles( 250000 );
      _psubot_pin_out_and( ILED_PORT, ~ILED );
      __delay_cycles( 250000 );
   }
}

#if 0
/* See WDT notes above. */
#pragma vector=WDT_VECTOR
__interrupt void psubot_wdt_isr( void ) {
   P1OUT ^= BIT0;
   IFG1 &= ~WDTIFG;
}
#endif

