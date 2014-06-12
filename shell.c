
/* TODO: If we support other I/O methods than the UART (like SPI or I2C), how *
 *       should we handle them here? Maybe an if/elif defined preprocessor    *
 *       block to find the correct header?                                    */
#include "uart.h"

#include "shell.h"

#include <stdint.h>

extern shell_command gao_commands[];
extern uint8_t gi_commands_count;

char gac_args[SHELL_ARG_COUNT][SHELL_COMMAND_LEN];

void shell_init( void ) {
   #ifdef ENABLE_SHELL_HELP
   uart_open( "shell", shell_task );
   uart_echo( "\n\r" );
   uart_echo( SHELL_STRING_PROMPT );
   #endif /* ENABLE_SHELL_HELP */
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
   } while( '\0' != *pc_iter_1 && '\0' != *pc_iter_2 );

   return TRUE;
}

#ifdef ENABLE_SHELL_HELP

void shell_command_help( void ) {
   uint8_t i;

   for( i = 0 ; i < gi_commands_count ; i++ ) {
      uart_echo( gao_commands[i].command );
      uart_echo( "\t" );
      uart_echo( gao_commands[i].helptext );
      uart_echo( "\r\n" );
   }
}

#endif /* ENABLE_SHELL_HELP */

BOOL shell_task( char c_char_in ) {
   uint8_t i, j;
   static uint8_t i_command_buffer_pos = 0,
      i_arg_buffer_pos = 0,
      i_arg_buffer_sel = 0;
   static char ac_command_last[SHELL_COMMAND_LEN] = { '\0' };

   if( '\r' == c_char_in ) {
      /* "Enter" was received, so process the last command. */

      uart_echo( "\n\r" );

      for( i = 0 ; i < SHELL_ARG_COUNT ; i++ ) {
         for( j = 0 ; j < SHELL_COMMAND_LEN ; j++ ) {
            gac_args[i][j] = '\0';
         }
      }

      /* Parse out the arguments from the command line. */
      i_arg_buffer_pos = 0;
      i_arg_buffer_sel = 0;
      i_command_buffer_pos = 0;
      while( '\0' != ac_command_last[i_command_buffer_pos] ) {
         if( ' ' == ac_command_last[i_command_buffer_pos] ) {
            /* Move on to the next argument buffer if we can. */
            if( SHELL_ARG_COUNT <= (i_arg_buffer_sel + 1) ) {
               #ifdef ENABLE_SHELL_HELP
               /* TODO: Do we need to handle this situation if it can't occur *
                *       with the selected variable sizing?                    */
               uart_echo( SHELL_STRING_TOOMANYARGS );
               #endif /* ENABLE_SHELL_HELP */
               break;
            } else {
               i_arg_buffer_sel++;
               i_arg_buffer_pos = 0;
            }
         } else {
            /* Add the next character to the current argument buffer. */
            /* TODO: Slim down the RAM used for each argument buffer safely. */
            gac_args[i_arg_buffer_sel][i_arg_buffer_pos] =
               ac_command_last[i_command_buffer_pos];
            i_arg_buffer_pos++;
         }

         i_command_buffer_pos++;
      }

      /* Execute the client program's command handler, if there is one. */
      for( i = 0 ; i < gi_commands_count ; i++ ) {
         if( shell_strcmp( gao_commands[i].command, ac_command_last ) ) {
            gao_commands[i].handler();
         }
      }

      for( i = 0 ; i < SHELL_COMMAND_LEN ; i++ ) {
         ac_command_last[i] = '\0';
      }
      i_command_buffer_pos = 0;
      uart_echo( SHELL_STRING_PROMPT );

   } else if( 0x7f == c_char_in ) {
      /* Delete character. */
      if( 0 < i_command_buffer_pos ) {
         uart_putc( 0x08 ); /* Backspace */
         i_command_buffer_pos--;
      }

   } else if( '\r' != c_char_in ) {
      if( i_command_buffer_pos <= (SHELL_COMMAND_LEN - 1) ) {
         /* Add the character to the current buffer and echo it back. */
         ac_command_last[i_command_buffer_pos] = c_char_in;
         i_command_buffer_pos++;
         uart_putc( c_char_in );
      } else {
         /* The command is too long, so reset. */
         i_command_buffer_pos = 0;
         #ifdef ENABLE_SHELL_HELP
         uart_echo( SHELL_STRING_TOOLONG );
         #endif /* ENABLE_SHELL_HELP */
      }
   }

   /* The shell should always be the last task. */
   return TRUE;
}

