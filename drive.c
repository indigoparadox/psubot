
#include "drive.h"

DRIVING_DIR ge_driving_dir = DRIVING_STOPPED;

void drive_wheels_enable( void ) {
   #ifdef WHEEL_R_F
   /* Configured for two wheels forward/reverse. */
   WHEEL_R_F_PDIR |= WHEEL_R_F;
   WHEEL_L_F_PDIR |= WHEEL_L_F;
   WHEEL_L_R_PDIR |= WHEEL_L_R;
   WHEEL_R_R_PDIR |= WHEEL_R_R;
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
         WHEEL_R_F_POUT |= WHEEL_R_F;
         WHEEL_L_F_POUT |= WHEEL_L_F;
         break;
      case DRIVING_REVERSE:
         WHEEL_L_R_POUT |= WHEEL_L_R;
         WHEEL_R_R_POUT |= WHEEL_R_R;
         break;
      case DRIVING_RIGHT:
         WHEEL_L_F_POUT |= WHEEL_L_F;
         break;
      case DRIVING_RIGHT_PIVOT:
         WHEEL_L_F_POUT |= WHEEL_L_F;
         WHEEL_R_R_POUT |= WHEEL_R_R;
         break;
      case DRIVING_LEFT:
         WHEEL_R_F_POUT |= WHEEL_R_F;
         break;
      case DRIVING_LEFT_PIVOT:
         WHEEL_R_F_POUT |= WHEEL_R_F;
         WHEEL_L_R_POUT |= WHEEL_L_R;
         break;
      case DRIVING_STOPPED:
         WHEEL_R_F_POUT &= ~WHEEL_R_F;
         WHEEL_L_F_POUT &= ~WHEEL_L_F;
         WHEEL_R_R_POUT &= ~WHEEL_R_R;
         WHEEL_L_R_POUT &= ~WHEEL_L_R;
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

