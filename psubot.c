#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT1
#define LED3 BIT2

int main( void ) {

   WDTCTL = WDTPW + WDTHOLD;
   P2DIR |= LED1;
   P2DIR |= LED2;
   P2DIR |= LED3;
   P2OUT = 0; /* Turn off all indicators.*/

   for (;;) {
      P2OUT = LED1;
      __delay_cycles( 500000 );
      P2OUT = LED2;
      __delay_cycles( 500000 );
      P2OUT = LED3;
      __delay_cycles( 500000 );
   }

   return 0;
}

