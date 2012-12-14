
#include "psubot.h"

int i_led_current = LED_RED,
   i_debounce_counter = 0;

int main( void ) {
   int i;

   WDTCTL = WDTPW + WDTHOLD;

   psubot_init();

   psubot_button_enable();

   P2OUT = i_led_current;

   /* Move the eye around. */
   P1OUT |= EYE_ON;
   P1OUT |= EYE_R;
   for( i = 0 ; i < 20000 ; i++ ) {
   }
   P1OUT &= ~EYE_R;
   P1OUT |= EYE_L;
   for( i = 0 ; i < 20000 ; i++ ) {
   }
   P1OUT &= ~EYE_L;
   P1OUT &= ~EYE_ON;

   psubot_wait();

   return 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2( void ) {
   P2IFG &= ~BUTTON;
   P2IE &= ~BUTTON;

   if( i_debounce_counter ) {
      return;
   }

   /* Start the debounce timer. */
   i_debounce_counter = DEBOUNCE_TIMEOUT;
   BCSCTL3 |= LFXT1S_2;
   BCSCTL1 |= DIVA_1;
   TA0CCR0 = 1200;
   TA0CCTL0 = 0x10;
   TA0CTL = TASSEL_1 + MC_1;
   TACCTL0 |= CCIE;

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

   /* wake up the main program */
   //__bic_SR_register_on_exit( LPM0_bits );
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0( void ) {
   TACCTL0 &= ~CCIFG;

   if( i_debounce_counter ) {
      /* Don't accept button presses yet. */
      P2IFG &= ~BUTTON;
      i_debounce_counter--;
   } else {
      /* Debounce complete. */
      i_debounce_counter = 0;
      TACCTL0 &= ~ CCIE;
      P2IE |= BUTTON;
   }
}

