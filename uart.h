
#ifndef UART_H
#define UART_H

/* = Includes = */

#include <msp430.h>

#include <stdlib.h>
#include <string.h>

#include "core.h"

/* = Constants = */

#ifdef ENABLE_SERIAL_HW
/* 9600 bps */
#define UART_UCA0BR0 104
#define UART_UCA0BR1 0
#else
#define UART_CPU_FREQ  1000000 # 1 Mhz
#define UART_BIT_TIME UART_CPU_FREQ / SERIAL_SW_BAUD
#define UART_HALF_BIT_TIME UART_BIT_TIME / 2
#endif /* ENABLE_SERIAL_HW */

/* = Structs = */

struct uart_task;
struct uart_task {
   BOOL (*task)( char );
   char* id;
   struct uart_task* prev;
};

/* = Function Prototypes = */

void uart_serial_init( void );
void uart_putc( const char );
void uart_echo( char* );
void uart_open( char*, BOOL (*)( char ) );

#if 0
/* This must be defined in any test program that calls this header, even if   *
 * is left empty in that program. It will be called by the UART's RX ISR and  *
 * passed a string containing any completed command entered via the UART.     */
void uart_command_handler( char* );
#endif

#endif /* UART_H */

