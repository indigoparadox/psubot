
#include <msp430.h>

#include "core.h"
#ifdef ENABLE_SHELL
#include "shell.h"
#include "uart.h"
#endif /* ENABLE_SHELL */
#ifdef ENABLE_BEEP
#include "beep.h"
#endif /* ENABLE_BEEP */
#ifdef ENABLE_EYE
#include "eye.h"
#endif /* ENABLE_EYE */
#include "drive.h"
#include "scheduler.h"

#include <stdlib.h>

SHELL_ENABLE();

int main( void ) {
   #ifdef ENABLE_BEEP
   BEEP_NOTE gae_startup[] = { STARTUP_TUNE };
   #endif /* ENABLE_BEEP */

   WDTCTL = WDTPW | WDTHOLD;
   P1OUT = 0;
   P2OUT = 0;

   #ifdef ENABLE_EYE
   eye_enable();
   /* eye_pos( 50 ); */
   #endif /* ENABLE_EYE */

   drive_wheels_enable();

   #ifdef ENABLE_SHELL
   uart_serial_init();
   #endif /* ENABLE_SHELL */

   #ifdef ENABLE_EYE
   eye_glow( EYE_RED, 1 );
   #endif /* ENABLE_EYE */

   #ifdef ENABLE_BEEP
   beep_init();
   #endif /* ENABLE_BEEP */

   #ifdef ENABLE_SERIAL_BT
   // TODO: Check to see if we're connected.
   uart_echo( "\r\n+STWMOD=0\r\n" );
   uart_echo( "\r\n+STNA=" SERIAL_BT_ID "\r\n" );
   uart_echo( "\r\n+STAAUTO=1\r\n" );
   uart_echo( "\r\n+STOAUT=1\r\n" );
   uart_echo( "\r\n+STPIN=0000\r\n" );
   __delay_cycles( 1000000 );
   uart_echo( "\r\n+INQ=1\r\n" );
   __delay_cycles( 1000000 );
   /* uart_echo( "\r\n+CONN=00:10:C6:C3:E6:4D\r\n" ); */
   #endif /* ENABLE_SERIAL_BT */

   #ifdef ENABLE_SHELL
   shell_init();
   #endif /* ENABLE_SHELL */

   #ifdef ENABLE_BEEP
   /* Play a little tune to signal we're ready. */
   beep_string( gae_startup, 100 );
   #endif /* ENABLE_BEEP */

   #ifdef ENABLE_EYE
   eye_glow( EYE_BLUE, EYE_DUTY_MAX );
   #endif /* ENABLE_EYE */

   /* Go to sleep. */
   /* TODO: Find a way to sleep without disabling the maintenance timer. */
   __bis_SR_register( LPM0_bits + GIE );

   return 0;
}

#ifdef ENABLE_SHELL

#ifdef ENABLE_EYE

void command_led( void ) {
   if( shell_strcmp( "RED", gac_args[1] ) ) {
      eye_glow( EYE_RED, 100 );
   } else if( shell_strcmp( "GREEN", gac_args[1] ) ) {
      eye_glow( EYE_GREEN, 100 );
   } else if( shell_strcmp( "BLUE", gac_args[1] ) ) {
      eye_glow( EYE_BLUE, 100 );
   }
}

void command_eye( void ) {
   if( shell_strcmp( "POS", gac_args[1] ) ) {
      eye_pos( atoi( gac_args[2] ) );
   } else if( shell_strcmp( "R", gac_args[1] ) ) {
      eye_move( EYE_RIGHT );
   } else if( shell_strcmp( "L", gac_args[1] ) ) {
      eye_move( EYE_LEFT );
   } else if( shell_strcmp( "S", gac_args[1] ) ) {
      eye_move( EYE_STOPPED );
   }
}

#endif /* ENABLE_EYE */

#ifdef ENABLE_BEEP

void command_beep( void ) {
   beep( atoi( gac_args[1] ), atoi( gac_args[2] ) );
}

#endif /* ENABLE_BEEP */

void command_drive( void ) {
   if( shell_strcmp( "R", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_RIGHT );
   } else if( shell_strcmp( "L", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_LEFT );
   } else if( shell_strcmp( "PR", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_RIGHT_PIVOT );
   } else if( shell_strcmp( "PL", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_LEFT_PIVOT );
   } else if( shell_strcmp( "F", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_FORWARD );
   } else if( shell_strcmp( "B", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_REVERSE );
   } else if( shell_strcmp( "S", gac_args[1] ) ) {
      drive_wheel_drive( DRIVING_STOPPED );
   }
}

SHELL_COMMANDS_BLOCK_START( 4 )
#ifdef ENABLE_EYE
SHELL_COMMANDS_BLOCK_ITEM( "LED", "[EYE COLOR]", command_led ),
SHELL_COMMANDS_BLOCK_ITEM( "EYE", "[POS] [INCR] OR [R/L] [INCR]", command_eye ),
#endif /* ENABLE_EYE */
#ifdef ENABLE_BEEP
SHELL_COMMANDS_BLOCK_ITEM( "BEEP", "[FREQ] [TIME]", command_beep ),
#endif /* ENABLE_BEEP */
SHELL_COMMANDS_BLOCK_ITEM( "DRIVE", "[F/B/R/L/RP/LP] [TIME]", command_drive ),
SHELL_COMMANDS_BLOCK_END()

#endif /* ENABLE_SHELL */

