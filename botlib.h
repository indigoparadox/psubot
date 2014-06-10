
#ifndef PSUBOT_H
#define PSUBOT_H

#include <msp430.h>

/* = Types = */

typedef int BOOL;

/* = Constants = */

#ifndef NULL
#define NULL 0
#endif /* NULL */

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */


typedef enum {
   DRIVING_STOPPED,
   DRIVING_FORWARD,
   DRIVING_REVERSE,
   DRIVING_RIGHT,
   DRIVING_LEFT,
   DRIVING_RIGHT_PIVOT,
   DRIVING_LEFT_PIVOT
} DRIVING_DIR;

typedef enum {
   EYE_RIGHT,
   EYE_LEFT,
   EYE_STOPPED
} EYE_DIR;

typedef enum {
   PORT1,
   PORT2
} PORT;

/* Configuration */

#define EYE_MAX_CYCLES_L 10500
#define EYE_MAX_LOOPS_R 40000

/* = Function Prototypes = */

void psubot_init( void );
int _psubot_pin_in( PORT, int );
void _psubot_pin_dir_or( PORT, int );
void _psubot_pin_out_or( PORT, int );
void psubot_button_enable( void );
void psubot_eye_enable( void );
void psubot_wheels_enable( void );
void psubot_eye_pos( int );
void psubot_eye_move( EYE_DIR );
void psubot_wheel_drive( DRIVING_DIR );
void psubot_wait( void );
void psubot_halt( void );

#endif /* PSUBOT_H */

