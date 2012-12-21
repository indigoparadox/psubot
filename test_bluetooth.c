
#include <msp430g2553.h>

#include "psubot.h"
#include "uart.h"
#include "shell.h"

#include <stdlib.h>

SHELL_ENABLE();

int i_led_current = LED_RED;

int main( void ) {
   WDTCTL = WDTPW + WDTHOLD;

   psubot_init();

   psubot_eye_enable();
   psubot_eye_pos( 50 );

   P2OUT = i_led_current;

   uart_serial_init();

   uart_echo( "\r\n+STWMOD=0\r\n" );
   uart_echo( "\r\n+STNA=PSUBot\r\n" );
   uart_echo( "\r\n+STPIN=2222\r\n" );
   uart_echo( "\r\n+STOAUT=1\r\n" );

   __delay_cycles( 500000 );
   uart_echo( "\r\n+INQ=1\r\n" );
   __delay_cycles( 500000 );

   shell_init();

   for( ; ; ) {
      /* Go to sleep. */
      __bis_SR_register( LPM3_bits + GIE );
   }

   return 0;
}

void command_led( void ) {
   
   if( shell_strcmp( "RED", gac_args[1] ) ) {
      i_led_current = LED_RED;
   } else if( shell_strcmp( "GREEN", gac_args[1] ) ) {
      i_led_current = LED_GREEN;
   } else if( shell_strcmp( "BLUE", gac_args[1] ) ) {
      i_led_current = LED_BLUE;
   }

   #if 0
   switch( i_led_current ) {
      case LED_RED:
         i_led_current = LED_GREEN;
         break;

      case LED_GREEN:
         i_led_current = LED_BLUE;
         break;

      case LED_BLUE:
         i_led_current = LED_RED;
         break;

   }
   #endif

   P2OUT = i_led_current;
}

void command_eye( void ) {
   psubot_eye_pos( atoi( gac_args[1] ) );
}

SHELL_COMMANDS_BLOCK_START( 2 )
SHELL_COMMANDS_BLOCK_ITEM( "LED", "CHANGE LED COLOR.", command_led ),
SHELL_COMMANDS_BLOCK_ITEM( "EYE", "POSITION EYE.", command_eye ),
SHELL_COMMANDS_BLOCK_END()

