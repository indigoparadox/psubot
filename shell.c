
/* TODO: If we support other I/O methods than the UART (like SPI or I2C), how *
 *       should we handle them here? Maybe an if/elif defined preprocessor    *
 *       block to find the correct header?                                    */
#include "uart.h"

#include "shell.h"

#include <msp430g2553.h>

extern shell_command ao_commands[];
extern int gi_commands_count;

void shell_init( void ) {
   /* Enable UART RX interrupt. */
   IE2 |= UCA0RXIE;
}

BOOL shell_strcmp( char* pc_string1_in, char* pc_string2_in ) {
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
__interrupt void shell_uart0_isr( void ) {
   uint8_t i;
   static uint8_t i_command_buffer_pos = 0;
   static char ac_command_last[SHELL_COMMAND_LEN] = { NULL },
      c_char_last = NULL;

   if( '\r' == UCA0RXBUF ) {
      /* "Enter" was received, so process the last command. */

      /* Execute the client program's command handler, if there is one. */
      //uart_command_handler( ac_command_last );
      for( i = 0 ; i < SHELL_COMMAND_LEN ; i++ ) {
         ac_command_last[i] = NULL;
      }
      i_command_buffer_pos = 0;
      uart_echo( "\n\rREADY\n\r" );

   } else if( '\r' != UCA0RXBUF ) {
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

