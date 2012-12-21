
/* TODO: If we support other I/O methods than the UART (like SPI or I2C), how *
 *       should we handle them here? Maybe an if/elif defined preprocessor    *
 *       block to find the correct header?                                    */
#include "uart.h"

#include "shell.h"

#include <stdint.h>

extern shell_command gao_commands[];
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

void shell_command_help( char** pc_args_in ) {

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void shell_uart0_isr( void ) {
   uint8_t i, j;
   static uint8_t i_command_buffer_pos = 0,
      i_arg_buffer_pos = 0,
      i_arg_buffer_sel = 0;
   static char ac_command_last[SHELL_COMMAND_LEN] = { '\0' },
      ac_args[SHELL_ARG_COUNT][SHELL_COMMAND_LEN];

   if( '\r' == UCA0RXBUF ) {
      /* "Enter" was received, so process the last command. */

      for( i = 0 ; i < SHELL_ARG_COUNT ; i++ ) {
         for( i = 0 ; i < SHELL_COMMAND_LEN ; i++ ) {
            ac_args[i][j] = '\0';
         }
      }


      /* Parse out the arguments from the command line. */
      i_arg_buffer_pos = 0;
      i_arg_buffer_sel = 0;
      i_command_buffer_pos = 0;
      while( NULL != ac_command_last[i_command_buffer_pos] ) {
         if( ' ' == ac_command_last[i_command_buffer_pos] ) {
            /* Move on to the next argument buffer if we can. */
            if( SHELL_ARG_COUNT <= (i_arg_buffer_sel + 1) ) {
               uart_echo( "\n\rTOO MANY ARGS\n\r" );
               break;
            } else {
               i_arg_buffer_sel++;
               i_arg_buffer_pos = 0;
            }
         } else {
            /* Add the next character to the current argument buffer. */
            /* TODO: Slim down the RAM used for each argument buffer safely. */
            ac_args[i_arg_buffer_sel][i_arg_buffer_pos] =
               ac_command_last[i_command_buffer_pos];
            i_command_buffer_pos++;
            i_arg_buffer_pos++;
         }

         i_command_buffer_pos++;
      }

      uart_echo( ac_args[0] );
      uart_echo( "\n\r" );

      /* Execute the client program's command handler, if there is one. */
      for( i = 0 ; i < gi_commands_count ; i++ ) {
         if( shell_strcmp( gao_commands[i].command, ac_command_last ) ) {
            gao_commands[i].handler( (char**)ac_args );
         }
      }

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
}

