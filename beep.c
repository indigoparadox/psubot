
#include "beep.h"

#include <msp430.h>

void beep_init( void ) {
   pins_dir_or( SPEAKER_PORT, SPEAKER );
   pins_out_and( SPEAKER_PORT, ~SPEAKER );
}

void beep( int i_freq_in, int i_time_in ) {
   int i, j;

   pins_out_and( SPEAKER_PORT, ~SPEAKER );

   for( i = 0 ; i < i_time_in ; i++ ) {
      pins_out_or( SPEAKER_PORT, SPEAKER );
      for( j = 0 ; j < i_freq_in ; j++ ) {}
      pins_out_and( SPEAKER_PORT, ~SPEAKER );
      for( j = 0 ; j < i_freq_in ; j++ ) {}
   }

   pins_out_and( SPEAKER_PORT, ~SPEAKER );
}

/* Purpose: Play an array of beep notes terminated by NULL.                   */
void beep_string( BEEP_NOTE* pe_notes_in, int i_note_time_in ) {
   BEEP_NOTE* pe_note_iter = pe_notes_in;

   while( BEEP_NULL != *pe_note_iter ) {
      beep( (int)*pe_note_iter, i_note_time_in );

      pe_note_iter++;
   }
}

