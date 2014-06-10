
#include "botlib.h"

//int gi_debounce_counter_button = 0;
BOOL gb_sleeping = 0;

void psubot_init( void ) {

   /* Turn off all indicators.*/
   P1OUT = 0;
   P2OUT = 0;

   #if 0
   /* This should be used on button inputs to debounce or something,          *
    * probably. Our original intention was to use it to control wheel drivine *
    * before we switched to the new continuous driving model. The timer       *
    * itself conflicts with the low power modes.                              */

   /* Enable the maintenance timer. */
   WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL;
   /* WDTCTL = WDT_ADLY_1000; */
   IE1 |= WDTIFG;
   /* BCSCTL2 = DIVS_2; */
   _EINT();
   #else
   WDTCTL = WDTPW | WDTHOLD;
   #endif
}

/* Purpose: Halt all activity and blink the internal LED.                     */
void psubot_halt( void ) {
   
   /* Disable all interrupts. */
   P1IE = 0;
   P2IE = 0;
   IE2 = 0;

   /* Turn off all outputs. */
   P1OUT = 0;
   P2OUT = 0;

   /* Blink indefinitely. */
   pins_dir_or( ILED_PORT, ILED );
   for(;;) {
      pins_out_or( ILED_PORT, ILED );
      __delay_cycles( 250000 );
      pins_out_and( ILED_PORT, ~ILED );
      __delay_cycles( 250000 );
   }
}

#if 0
/* See WDT notes above. */
#pragma vector=WDT_VECTOR
__interrupt void psubot_wdt_isr( void ) {
   P1OUT ^= BIT0;
   IFG1 &= ~WDTIFG;
}
#endif

