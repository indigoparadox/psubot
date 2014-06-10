
#include "pins.h"

int pins_in( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         return P1IN & i_bit_in;

      case PORT2:
         return P2IN & i_bit_in;
   }

   return 0;
}

void pins_dir_or( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         P1DIR |= i_bit_in;
         break;

      case PORT2:
         P2DIR |= i_bit_in;
         break;
   }
}

void pins_out_or( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         P1OUT |= i_bit_in;
         break;

      case PORT2:
         P2OUT |= i_bit_in;
         break;
   }
}

void pins_out_and( PORT i_port_in, int i_bit_in ) {
   switch( i_port_in ) {
      case PORT1:
         P1OUT &= i_bit_in;
         break;

      case PORT2:
         P2OUT &= i_bit_in;
         break;
   }
}

