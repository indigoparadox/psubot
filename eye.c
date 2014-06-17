
#include "eye.h"

int gi_eye_move_loops = 0;

void eye_enable( void ) {

   /* Setup Motor I/O */
   EYE_R_PDIR |= EYE_R;
   EYE_L_PDIR |= EYE_L;
   
   /* Setup LED I/O */
   EYE_LED_RED_PDIR |= EYE_LED_RED;
   EYE_LED_GREEN_PDIR |= EYE_LED_GREEN;
   EYE_LED_BLUE_PDIR |= EYE_LED_BLUE;
}

/* Parameters:                                                                *
 *    i_pos_in - The percentage to the left to position the eye.              */
void eye_pos( int i_pos_in ) {
   uint8_t i_sense_count = 0;
   long l_target_pos,
      l_pos_in = i_pos_in,
      i;

   /* Figure out the position as a percentage of the eye's max range. */
   l_target_pos = l_pos_in * EYE_MAX_CYCLES_L / 100;

   /* Move the eye to position zero. */
   EYE_R_POUT |= EYE_R;
   while( 
      !(EYE_SENSE_PIN & EYE_SENSE && i_sense_count >= 200) && 
      EYE_MAX_LOOPS_R > gi_eye_move_loops
   ) {
      __delay_cycles( 10 );
      gi_eye_move_loops++;
      
      /* Don't count the sensor as being held down unless it's being held     *
       * down!                                                                */
      if( EYE_SENSE_PIN & EYE_SENSE ) {
         i_sense_count++;
      } else {
         i_sense_count = 0;
      }
   }
   EYE_R_POUT &= ~EYE_R;

   /* Make sure we didn't just time out. */
   if( EYE_MAX_LOOPS_R <= gi_eye_move_loops ) {
      scheduler_halt();
   } else {
      gi_eye_move_loops = 0;
   }

   eye_move( EYE_LEFT );
   for( i = 0 ; i < l_target_pos ; i++ ) {}
   eye_move( EYE_STOPPED );
}

/* Purpose: Push the eye to the robot's left by the given increment.          */
/* TODO: Keep a global variable with the current position of the eye. Set it  *
 *       to 0 during the eye init phase and increment/decrement it whenever   *
 *       the eye is moved. Don't allow the eye to move out of its operational *
 *       range.                                                               */
void eye_move( EYE_DIR e_eye_dir_in ) {
   switch( e_eye_dir_in ) {
      case EYE_RIGHT:
         EYE_R_POUT |= EYE_R;
         break;
      case EYE_LEFT:
         EYE_L_POUT |= EYE_L;
         break;
      case EYE_STOPPED:
         EYE_L_POUT &= ~EYE_L;
         EYE_R_POUT &= ~EYE_R;
         break;
   }
}

void eye_glow( EYE_COLOR i_color_in, int i_brightness_in ) {
   static int16_t i_last_id = -1;
   int* pi_args_out;
   
   pi_args_out = malloc( 3 * sizeof( int ) );

   switch( i_color_in ) {
      case EYE_RED:
         pi_args_out[0] = (int)&EYE_LED_RED_POUT;
         pi_args_out[1] = EYE_LED_RED;
         break;

      case EYE_GREEN:
         pi_args_out[0] = (int)&EYE_LED_GREEN_POUT;
         pi_args_out[1] = EYE_LED_GREEN;
         break;

      case EYE_BLUE:
         pi_args_out[0] = (int)&EYE_LED_BLUE_POUT;
         pi_args_out[1] = EYE_LED_BLUE;
         break;
   }
   pi_args_out[2] = i_brightness_in;

   /* Shutdown previous eye task if it exists. */
   if( 0 <= i_last_id ) {
      scheduler_del_task( i_last_id );
   }
   
   i_last_id = scheduler_add_task(
      eye_glow_task, eye_glow_shutdown, 3, pi_args_out
   );
}

void eye_glow_task( uint8_t i_argc_in, int* pi_argi_in ) {
   static uint16_t i_led_duty_counter = 0;
   PORT_REF* pi_led_port = (PORT_REF*)pi_argi_in[0];

   /* Use PWM to vary the brightness of the LED as best we can. */

   if( i_led_duty_counter < pi_argi_in[2] ) {
      *pi_led_port |= pi_argi_in[1];
   } else {
      *pi_led_port &= ~pi_argi_in[1];
   }

   i_led_duty_counter++;
   if( EYE_DUTY_MAX <= i_led_duty_counter ) {
      i_led_duty_counter = 0;
   }
}

uint8_t eye_glow_shutdown( uint8_t i_argc_in, int* pi_argi_in ) {
   pi_argi_in[0] &= ~pi_argi_in[1];
   return 0;
}

