
#include "beep.h"

#include <msp430.h>

void beep_init( void ) {
   SPEAKER_PDIR |= SPEAKER;
   SPEAKER_POUT &= ~SPEAKER;
}

void beep( int i_freq_in, int i_time_in ) {
   scheduler_buzz(
      &SPEAKER_PSEL, SPEAKER, 1000 - 1, 250, i_time_in * 10, BUZZER_MODE_UP,
      NULL, NULL, 0, NULL
   );
}

/* Purpose: Play an array of beep notes terminated by NULL.                   */
void beep_string( BEEP_NOTE* pe_notes_in, int i_note_time_in ) {
   BEEP_NOTE* pe_note_iter = pe_notes_in;

   while( BEEP_NULL != *pe_note_iter ) {
      beep( (int)*pe_note_iter, i_note_time_in );

      pe_note_iter++;
   }
}

