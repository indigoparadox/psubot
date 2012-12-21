
#include <msp430g2553.h>

#include "psubot.h"
#include "uart.h"
#include "shell.h"

int i_led_current = LED_RED;

int main( void ) {
   WDTCTL = WDTPW + WDTHOLD;

   psubot_init();

   psubot_eye_enable();
   psubot_eye_pos( EYE_MAX_CYCLES_L / 2 );

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

void command_led( char** pc_args_in ) {
   uart_echo( "\n\r" );
   uart_echo( pc_args_in[0] );
   uart_echo( "\n\r" );
   uart_echo( pc_args_in[0] );

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

   P2OUT = i_led_current;
}

void command_eye( char** pc_args_in ) {
   psubot_eye_pos( EYE_MAX_CYCLES_L / 2 );
   __delay_cycles( 500000 );
}

SHELL_COMMANDS_BLOCK_START( 2 )
SHELL_COMMANDS_BLOCK_ITEM( "LED", "CHANGE LED COLOR.", command_led ),
SHELL_COMMANDS_BLOCK_ITEM( "EYE", "POSITION EYE.", command_eye ),
SHELL_COMMANDS_BLOCK_END()

