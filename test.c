
#include "psubot.h"

/* 5 = 1 second */
#define DEBOUNCE_TIMEOUT   5

BUTTON_ENABLE();

int i_led_current = LED_RED;

int main( void ) {
   int i;

   WDTCTL = WDTPW + WDTHOLD;

   psubot_init();

   psubot_button_enable();

   psubot_eye_enable();
   psubot_eye_pos( EYE_MAX_CYCLES_L );
   psubot_eye_pos( EYE_MAX_CYCLES_L / 2 );

   P2OUT = i_led_current;

   psubot_wait();

   return 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2( void ) {
   if( P2IFG & BUTTON ) {
      /* Reset button status. */
      P2IFG &= ~BUTTON;
      P2IE &= ~BUTTON;

      if( gi_debounce_counter_button ) {
         return;
      }

      /* Start the debounce timer. */
      gi_debounce_counter_button = DEBOUNCE_TIMEOUT;
      BCSCTL3 |= LFXT1S_2;
      BCSCTL1 |= DIVA_1;
      TA0CCR0 = 1200;
      TA0CCTL0 = 0x10;
      TA0CTL = TASSEL_1 + MC_1;
      TACCTL0 |= CCIE;

      psubot_eye_pos( EYE_MAX_CYCLES_L / 2 );

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
   }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0( void ) {
   TACCTL0 &= ~CCIFG;

   if( gi_debounce_counter_button ) {
      /* Don't accept button presses yet. */
      P2IFG &= ~BUTTON;
      gi_debounce_counter_button--;
   } else {
      /* Debounce complete. */
      gi_debounce_counter_button = 0;
      TACCTL0 &= ~ CCIE;
      P2IE |= BUTTON;
   }
}

