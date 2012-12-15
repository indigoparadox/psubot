
#ifndef PSUBOT_H
#define PSUBOT_H

#include <msp430g2553.h>

/* = Types = */

typedef int BOOL;

/* = Constants = */

#define TRUE 1
#define FALSE 0

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

#define BUTTON_ENABLE() extern int gi_debounce_counter_button;
#define BUTTON_DEBOUNCE( i_sec_in ) if( gi_debounce_counter_button ) { \
      /* Button may still be bouncing. */ \
      return; \
   } else { \
      /* Start the debounce timer. */ \
      gi_debounce_counter_button = BUTTON_DEBOUNCE_TIMEOUT_INC * i_sec_in; \
   } \
   /* Enable the background maintenance timer. */ \
   BCSCTL3 |= LFXT1S_2; \
   BCSCTL1 |= DIVA_1; \
   TA1CCR0 = 1200; \
   TA1CCTL0 = 0x10; \
   TA1CTL = TASSEL_1 + MC_1; \
   TA1CCTL0 |= CCIE;
#define SLEEP_ENABLE() extern BOOL gb_sleeping;
#define BOT_WAIT_CYCLES( cycles_in ) __delay_cycles( cycles_in );

/* = Function Prototypes = */

void psubot_init( void );
void psubot_button_enable( void );
void psubot_eye_enable( void );
void psubot_eye_pos( int );
void psubot_eye_left( int );
void psubot_eye_right( int );
void psubot_wait( void );

#endif /* PSUBOT_H */

