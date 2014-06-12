
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

static void _uart_listen( void ) {
   #ifdef ENABLE_SERIAL_HW
   /* Use hardware serial interface. */

   /* Enable UART RX interrupt. */
   IE2 |= UCA0RXIE;

   #else
   /* TODO: Use software serial approximation. */

   #endif /* ENABLE_SERIAL_HW */
}

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

   #else
   /* Use software serial approximation. */

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

   gi_tx_byte = c_char_in;

   #if 0
   /* Wait for line to be free. */
   while( gi_receiving );
   #endif

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

uint8_t uart_count_tasks( void ) {
   struct uart_task* ps_task_iter = gps_uart_tasks;
   uint8_t i_task_count = 0;

   while( NULL != ps_task_iter ) {
      i_task_count++;
      ps_task_iter = ps_task_iter->prev;
   }

   return i_task_count;
}

void uart_open( char* pc_id_in, BOOL (*task_in)( char ) ) {
   struct uart_task* ps_task_new;
   uint8_t i_tasks_count;

   /* Get the task count before we add anything. */
   i_tasks_count = uart_count_tasks();

   /* Enable UART RX interrupt. */
   IE2 |= UCA0RXIE;

   /* Create the new task. */
   ps_task_new = malloc( sizeof( struct uart_task ) );
   memset( ps_task_new, 0, sizeof( struct uart_task ) );
   ps_task_new->task = task_in;
   ps_task_new->id = pc_id_in;

   /* Add the task to the tasks list. */
   if( NULL == gps_uart_tasks ) {
      gps_uart_tasks = ps_task_new;
   } else {
      ps_task_new->prev = gps_uart_tasks;
      gps_uart_tasks = ps_task_new;
   }

   /* If this is the first task, start the listener. */
   if( 1 > i_tasks_count ) {
      _uart_listen();
   }
}

#ifdef ENABLE_SERIAL_HW

#pragma vector=USCIAB0RX_VECTOR
__interrupt void uart_uart0_isr( void ) {
   struct uart_task* ps_task_iter = gps_uart_tasks;

   /* TODO: Look for control characters and do special things with them. */

   /* TODO: Implement tasks as a stack so latest programs block earlier. */
   
   while( NULL != ps_task_iter ) {
      if( (*ps_task_iter->task)( UCA0RXBUF ) ) {
         /* The task blocked later tasks. */
         break;
      }
      
      ps_task_iter = ps_task_iter->prev;
   }
}

#else

/* TODO: Implement software UART. */

#endif /* ENABLE_SERIAL_HW */

