
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

   for( ; ; ) {
      /* Go to sleep. */
      __bis_SR_register( LPM3_bits + GIE );
   }

   uart_serial_init();

   //__delay_cycles( 2500000 );

   /* DEBUG: Turn on red LED. */
   /*P1DIR |= BIT0;
   P1OUT |= BIT0;*/

   uart_echo( "\r\n+STWMOD=0\r\n" );
   uart_echo( "\r\n+STNA=PSUBot\r\n" );
   //uart_echo( "\r\n+STPIN=Shi4w4s3-ga-aru\r\n" );
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

#if 0
BOOL uart_serial_handler( char* pc_command_in ) {
   if( uart_strcmp( pc_command_in, "LED" ) ) {
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

   } else if( uart_strcmp( pc_command_in, "EYE" ) ) {
      psubot_eye_pos( EYE_MAX_CYCLES_L / 2 );
      __delay_cycles( 500000 );

   }
}
#endif

void command_help( char* pc_args_in ) {

}

SHELL_COMMANDS_BLOCK_START( 2 )
//SHELL_COMMANDS_BLOCK_ITEM( "HELP", "DISPLAY THIS LIST.", command_help ),
SHELL_COMMANDS_BLOCK_ITEM( "LED", "CHANGE LED COLOR.", command_help ),
SHELL_COMMANDS_BLOCK_ITEM( "EYE", "POSITION EYE.", command_help ),
SHELL_COMMANDS_BLOCK_END()

