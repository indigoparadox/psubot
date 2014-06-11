
#include "eye.h"

int gi_eye_move_loops = 0;

void eye_enable( void ) {

   /* Setup Motor I/O */
   P1DIR = 0;
   pins_dir_or( EYE_R_PORT, EYE_R );
   pins_dir_or( EYE_L_PORT, EYE_L );
   
   /* Setup LED I/O */
   P2DIR = 0;
   pins_dir_or( EYE_LED_RED_PORT, EYE_LED_RED );
   pins_dir_or( EYE_LED_GREEN_PORT, EYE_LED_GREEN );
   pins_dir_or( EYE_LED_BLUE_PORT, EYE_LED_BLUE );
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
   pins_out_or( EYE_R_PORT, EYE_R );
   while( 
      !(pins_in( EYE_SENSE_PORT, EYE_SENSE ) && i_sense_count >= 200) && 
      EYE_MAX_LOOPS_R > gi_eye_move_loops
   ) {
      __delay_cycles( 10 );
      gi_eye_move_loops++;
      
      /* Don't count the sensor as being held down unless it's being held     *
       * down!                                                                */
      if( pins_in( EYE_SENSE_PORT, EYE_SENSE ) ) {
         i_sense_count++;
      } else {
         i_sense_count = 0;
      }
   }
   pins_out_and( EYE_R_PORT, ~EYE_R );

   /* Make sure we didn't just time out. */
   if( EYE_MAX_LOOPS_R <= gi_eye_move_loops ) {
      psubot_halt();
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
         pins_out_or( EYE_R_PORT, EYE_R );
         break;
      case EYE_LEFT:
         pins_out_or( EYE_L_PORT, EYE_L );
         break;
      case EYE_STOPPED:
         pins_out_and( EYE_L_PORT, ~EYE_L );
         pins_out_and( EYE_R_PORT, ~EYE_R );
         break;
   }
}

void eye_glow( EYE_COLOR i_color_in, int i_brightness_in ) {
   int* pi_args_out = malloc( 3 * sizeof( int ) );
   switch( i_color_in ) {
      case EYE_RED:
         pi_args_out[0] = EYE_LED_RED_PORT;
         pi_args_out[1] = EYE_LED_RED;
         break;

      case EYE_GREEN:
         pi_args_out[0] = EYE_LED_GREEN_PORT;
         pi_args_out[1] = EYE_LED_GREEN;
         break;

      case EYE_BLUE:
         pi_args_out[0] = EYE_LED_BLUE_PORT;
         pi_args_out[1] = EYE_LED_BLUE;
         break;
   }
   pi_args_out[2] = i_brightness_in;
   scheduler_del_thread( "eye_led" );
   scheduler_add_thread(
      "eye_led", eye_glow_task, eye_glow_shutdown, 3, pi_args_out
   );
}

void eye_glow_task( int i_argc_in, int* pi_argi_in ) {
   static int i_led_duty_counter = 1;

   /* If we're at max duty then just keep the eye solid. */
   if( EYE_DUTY_MAX == pi_argi_in[2] ) {
      pins_out_or( pi_argi_in[0], pi_argi_in[1] );
      return;
   }

   /* TODO: Vary how many times we turn on/off based on [2], brightness. */
   if( 0 == (EYE_DUTY_MAX - i_led_duty_counter) / pi_argi_in[2] ) {
      i_led_duty_counter = 1;
      /* EYE_*_PORT, EYE_* */
      pins_out_toggle( pi_argi_in[0], pi_argi_in[1] );
   } else {
      i_led_duty_counter++;
   }
}

void eye_glow_shutdown( int i_argc_in, int* pi_argi_in ) {
   pins_out_and( pi_argi_in[0], ~pi_argi_in[1] );
}

