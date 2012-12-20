
#include "uart.h"

BOOL b_shell_enabled = FALSE;

void uart_serial_init( void ) {
   
   /* Set DCO to 1MHz. */
   BCSCTL1 = CALBC1_1MHZ;
   DCOCTL = CALDCO_1MHZ;
   
   /* Set P1.1 and P1.2 to RX and TX. */
   P1SEL = BIT1 + BIT2;                
   P1SEL2 = BIT1 + BIT2;
   
   /* Use SMCLK. */
   UCA0CTL1 |= UCSSEL_2;

   /* Set bitrate to 9600. */
   UCA0BR0 = UART_UCA0BR0;
   UCA0BR1 = UART_UCA0BR1;
   
   /* Use modulation. */
   UCA0MCTL = UCBRS_1;

   /* Start USCI. */
   UCA0CTL1 &= ~UCSWRST;

   /* Enable UART RX interrupt. */
   IE2 |= UCA0RXIE;
}

void uart_shell_init( void ) {
   b_shell_enabled = TRUE;
   uart_echo( "\n\rREADY\n\r" );
}

void uart_putc( const char c_char_in ) {
   while( !(IFG2 & UCA0TXIFG) );
   UCA0TXBUF = c_char_in;
}

void uart_echo( char* pc_string_in ) {
   char* pc_iter = pc_string_in;

   while( 0 != *pc_iter ) {
      uart_putc( *pc_iter );

      /* Next character. */
      pc_iter++;
   }
}

BOOL uart_strcmp( char* pc_string1_in, char* pc_string2_in ) {
   char* pc_iter_1 = pc_string1_in,
      * pc_iter_2 = pc_string2_in;
   
   do {
      if( *pc_iter_1 != *pc_iter_2 ) {
         return FALSE;
      }

      pc_iter_1++;
      pc_iter_2++;
   } while( NULL != *pc_iter_1 && NULL != *pc_iter_2 );

   return TRUE;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR( void ) {
   uint8_t i;
   static uint8_t i_command_buffer_pos = 0;
   static char ac_command_last[SHELL_COMMAND_LEN] = { NULL },
      c_char_last = NULL;

   /* If the shell isn't enabled then just pass the whole kit on to the       *
    * program's serial handler.                                               */
   if( !b_shell_enabled ) {
      uart_command_handler( NULL );
   }

   if( 13 == UCA0RXBUF && 13 != c_char_last ) {
      /* "Enter" was received, so process the last command. */

      /* Execute the client program's command handler, if there is one. */
      uart_command_handler( ac_command_last );
      for( i = 0 ; i < SHELL_COMMAND_LEN ; i++ ) {
         ac_command_last[i] = NULL;
      }
      i_command_buffer_pos = 0;
      uart_echo( "\n\rREADY\n\r" );

   } else if( 13 != UCA0RXBUF ) {
      if( i_command_buffer_pos <= (SHELL_COMMAND_LEN - 1) ) {
         /* Add the character to the current buffer and echo it back. */
         ac_command_last[i_command_buffer_pos] = UCA0RXBUF;
         i_command_buffer_pos++;
         uart_putc( UCA0RXBUF );
      } else {
         /* The command is too long, so reset. */
         i_command_buffer_pos = 0;
         uart_echo( "\n\rTOO LONG\n\r" );
      }
   }

   /* Remember this character for later. */
   c_char_last = UCA0RXBUF;
}

