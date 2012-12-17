
#ifndef PSUBOT_H
#define PSUBOT_H

#include <msp430g2553.h>

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

#define LED_RED   BIT0
#define LED_GREEN BIT1
#define LED_BLUE  BIT2

#define BUTTON    BIT3
#define EYE_SENSE BIT4
#define SWITCH    BIT5

/* 5 = 1 second */
#define BUTTON_DEBOUNCE_TIMEOUT_INC   5

/* Eye directions are from the robot's point of view. */

#define EYE_R     BIT4
#define EYE_L     BIT3
#define EYE_ON    BIT5

#define EYE_MAX_CYCLES_L 27000

/* = Macros = */

#define SLEEP_ENABLE() extern BOOL gb_sleeping;

/* = Function Prototypes = */

void psubot_init( void );
void psubot_button_enable( void );
void psubot_eye_enable( void );
void psubot_eye_pos( int );
void psubot_eye_left( int );
void psubot_eye_right( int );
void psubot_wait( void );

#endif /* PSUBOT_H */

