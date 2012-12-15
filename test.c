
#include "psubot.h"

BUTTON_ENABLE();
SLEEP_ENABLE();

int i_led_current = LED_RED;

int main( void ) {
   int i;

   WDTCTL = WDTPW + WDTHOLD;

   psubot_init();

   psubot_button_enable();

   psubot_eye_enable();
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

      BUTTON_DEBOUNCE( 1 );

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

