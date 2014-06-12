
#include "uart.h"

struct uart_task* gps_uart_tasks = NULL;

#ifndef ENABLE_SERIAL_HW

/* Constants required for  software UART. */

static volatile uint8_t gi_bit_count;
static volatile unsigned int gi_tx_byte;
static volatile unsigned int gi_rx_byte;
static volatile BOOL gi_receiving = FALSE;
static volatile BOOL gi_has_received = FALSE;

#endif /* ENABLE_SERIAL_HW */

void uart_serial_init( void ) {
   
   #ifdef ENABLE_SERIAL_HW
   /* Use hardware serial interface. */

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

   #else
   /* TODO: Use software serial approximation. */

   /* Software serial only supported on port 1. */
   P1SEL |= SERIAL_SW_TX;
   P1DIR |= SERIAL_SW_TX;

   P1IES |= SERIAL_SW_RX;
   P1IFG &= ~SERIAL_SW_RX;
   P1IE |= SERIAL_SW_RX;

   #endif /* ENABLE_SERIAL_HW */
}

void uart_putc( const char c_char_in ) {
   #ifdef ENABLE_SERIAL_HW
   /* Use hardware serial interface. */

   while( !(IFG2 & UCA0TXIFG) );
   UCA0TXBUF = c_char_in;
   
   #else
   /* TODO: Use software serial approximation. */
   #endif /* ENABLE_SERIAL_HW */
}

void uart_echo( char* pc_string_in ) {
   char* pc_iter = pc_string_in;

   while( 0 != *pc_iter ) {
      uart_putc( *pc_iter );

      /* Next character. */
      pc_iter++;
   }
}

void uart_open( char* pc_id_in, void (*task_in)( char ) ) {
   struct uart_task* ps_task_iter,
      * ps_task_new;

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct uart_task ) );
   memset( ps_task_new, 0, sizeof( struct uart_task ) );
   ps_task_new->task = task_in;
   ps_task_new->id = pc_id_in;

   /* Add the task to the tasks list. */
   if( NULL == gps_uart_tasks ) {
      gps_uart_tasks = ps_task_new;
   } else {
      ps_task_iter = gps_uart_tasks;
      while( NULL != ps_task_iter->next ) {
         ps_task_iter = ps_task_iter->next;
      }
      ps_task_iter->next = ps_task_new;
   }
}

#ifdef ENABLE_SERIAL_HW

#pragma vector=USCIAB0RX_VECTOR
__interrupt void uart_uart0_isr( void ) {
   struct uart_task* ps_task_iter = gps_uart_tasks;

   /* TODO: Look for control characters and do special things with them. */

   while( NULL != ps_task_iter ) {
      (*ps_task_iter->task)( UCA0RXBUF );
      
      ps_task_iter = ps_task_iter->next;
   }
}

#else

/* TODO: Implement software UART. */

#endif /* ENABLE_SERIAL_HW */

