
#include "drive.h"

DRIVING_DIR ge_driving_dir = DRIVING_STOPPED;

void drive_wheels_enable( void ) {
   pins_dir_or( WHEEL_R_F_PORT, WHEEL_R_F );
   pins_dir_or( WHEEL_L_F_PORT, WHEEL_L_F );
   pins_dir_or( WHEEL_L_R_PORT, WHEEL_L_R );
   pins_dir_or( WHEEL_R_R_PORT, WHEEL_R_R );
}

void drive_wheel_drive( DRIVING_DIR e_direction_in ) {
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
}

