
#include "beep.h"

#include <msp430.h>

void beep_init( void ) {

   P1DIR |= SPEAKER;
   P1OUT &= ~SPEAKER;

}

void beep( int i_freq_in, int i_time_in ) {
   int i, j;

   P1OUT &= ~SPEAKER;

   for( i = 0 ; i < i_time_in ; i++ ) {
      P1OUT |= SPEAKER;
      for( j = 0 ; j < i_freq_in ; j++ ) {}
      P1OUT &= ~SPEAKER;
      for( j = 0 ; j < i_freq_in ; j++ ) {}
   }

   P1OUT &= ~SPEAKER;
}

