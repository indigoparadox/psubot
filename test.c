
#include "psubot.h"

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

   psubot_serial_init();

   for( ; ; ) {
      /* Go to sleep. */
      __bis_SR_register( LPM3_bits + GIE );
   }

   return 0;
}

#if 0
#pragma vector=PORT2_VECTOR
__interrupt void Port_2( void ) {
   if( P2IFG & BUTTON ) {
      /* Reset button status. */
      P2IFG &= ~BUTTON;
      P2IE &= ~BUTTON;

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

      P2IE |= BUTTON;
   }
}
#endif

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR( void ) {
   /* Toggle LED if "A" RXed. */
   if( 65 == UCA0RXBUF ) {
      psubot_eye_pos( EYE_MAX_CYCLES_L / 2 );
      __delay_cycles( 500000 );

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

