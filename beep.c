
#include "beep.h"

#include <msp430.h>

void beep_init( void ) {
   pins_dir_or( SPEAKER_PORT, SPEAKER );
   pins_out_and( SPEAKER_PORT, ~SPEAKER );
}

void beep( int i_freq_in, int i_time_in ) {
   int* pi_args_out = malloc( 2 * sizeof( int ) );

   pi_args_out[0] = i_freq_in;
   pi_args_out[1] = i_time_in;

   scheduler_buzz( beep_buzz, 2, pi_args_out, FALSE );

   #if 0
   int i, j;

   IE1 &= ~WDTIE;
   WDTCTL = WDTPW | WDTHOLD;

   pins_out_and( SPEAKER_PORT, ~SPEAKER );

   for( i = 0 ; i < i_time_in ; i++ ) {
      pins_out_or( SPEAKER_PORT, SPEAKER );
      for( j = 0 ; j < i_freq_in ; j++ ) {}
      pins_out_and( SPEAKER_PORT, ~SPEAKER );
      for( j = 0 ; j < i_freq_in ; j++ ) {}
   }

   pins_out_and( SPEAKER_PORT, ~SPEAKER );
   #endif
}

/* Purpose: Play an array of beep notes terminated by NULL.                   */
void beep_string( BEEP_NOTE* pe_notes_in, int i_note_time_in ) {
   BEEP_NOTE* pe_note_iter = pe_notes_in;

   while( BEEP_NULL != *pe_note_iter ) {
      beep( (int)*pe_note_iter, i_note_time_in );

      pe_note_iter++;
   }
}

BOOL beep_buzz( uint8_t i_argc_in, int* pi_argi_in ) {

   /*
   TACCR0 = 1000-1;
   TACCTL1 = OUTMOD_7;
   TACCR1 = 250;
   TACTL = TASSEL_2 + MC_1;
   P1SEL |= SPEAKER;
   */
   //pins_out_toggle( SPEAKER_PORT, SPEAKER );

   return FALSE;
}

