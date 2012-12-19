
#ifndef UART_H
#define UART_H

#include <msp430g2553.h>

#include <stdint.h>

/* = Types = */

typedef int BOOL;

/* = Constants = */

#ifndef NULL
#define NULL 0
#endif /* NULL */

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#define SHELL_COMMAND_LEN 10
#define SHELL_COMMAND_COUNT 10

/* 9600 bps */
#define UART_UCA0BR0 104
#define UART_UCA0BR1 0

/* = Function Prototypes = */

void uart_serial_init( void );
void uart_shell_init( void );
void uart_putc( const char );
void uart_echo( char* );
BOOL uart_strcmp( char*, char* );

/* This must be defined in any test program that calls this header, even if   *
 * is left empty in that program. It will be called by the UART's RX ISR and  *
 * passed a string containing any completed command entered via the UART.     */
void uart_command_handler( char* );

#endif /* UART_H */

