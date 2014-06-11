
#include "drive.h"

DRIVING_DIR ge_driving_dir = DRIVING_STOPPED;

void drive_wheels_enable( void ) {
   #ifdef WHEEL_R_F
   /* Configured for two wheels forward/reverse. */
   pins_dir_or( WHEEL_R_F_PORT, WHEEL_R_F );
   pins_dir_or( WHEEL_L_F_PORT, WHEEL_L_F );
   pins_dir_or( WHEEL_L_R_PORT, WHEEL_L_R );
   pins_dir_or( WHEEL_R_R_PORT, WHEEL_R_R );
   #elif defined WHEEL_D_F
   /* Configured for a single drive wheel and a turning wheel. */
   /* TODO */
   #endif
}

void drive_wheel_drive( DRIVING_DIR e_direction_in ) {
   #ifdef WHEEL_R_F
   /* Configured for two wheels forward/reverse. */
   switch( e_direction_in ) {
      case DRIVING_FORWARD:
         pins_out_or( WHEEL_R_F_PORT, WHEEL_R_F );
         pins_out_or( WHEEL_L_F_PORT, WHEEL_L_F );
         break;
      case DRIVING_REVERSE:
         pins_out_or( WHEEL_L_R_PORT, WHEEL_L_R );
         pins_out_or( WHEEL_R_R_PORT, WHEEL_R_R );
         break;
      case DRIVING_RIGHT:
         pins_out_or( WHEEL_L_F_PORT, WHEEL_L_F );
         break;
      case DRIVING_RIGHT_PIVOT:
         pins_out_or( WHEEL_L_F_PORT, WHEEL_L_F );
         pins_out_or( WHEEL_R_R_PORT, WHEEL_R_R );
         break;
      case DRIVING_LEFT:
         pins_out_or( WHEEL_R_F_PORT, WHEEL_R_F );
         break;
      case DRIVING_LEFT_PIVOT:
         pins_out_or( WHEEL_R_F_PORT, WHEEL_R_F );
         pins_out_or( WHEEL_L_R_PORT, WHEEL_L_R );
         break;
      case DRIVING_STOPPED:
         pins_out_and( WHEEL_R_F_PORT, ~WHEEL_R_F );
         pins_out_and( WHEEL_L_F_PORT, ~WHEEL_L_F );
         pins_out_and( WHEEL_R_R_PORT, ~WHEEL_R_R );
         pins_out_and( WHEEL_L_R_PORT, ~WHEEL_L_R );
      default:
         return;
   }
   #elif defined WHEEL_D_F
   /* Configured for a single drive wheel and a turning wheel. */
   /* TODO */
   #endif
}

void drive_wheel_task( int i_argc_in, int* pi_argi_in ) {

}

void drive_wheel_shutdown( int i_argc_in, int* pi_argi_in ) {

}

