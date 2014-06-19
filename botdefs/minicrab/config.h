
#define CPU msp430g2211

#define ENABLE_SHELL

#define SERIAL_SW_TX_PSEL  P1SEL
#define SERIAL_SW_TX_PDIR  P1DIR
#define SERIAL_SW_TX       BIT2
#define SERIAL_SW_RX_PSEL  P1SEL
#define SERIAL_SW_RX       BIT1
#define SERIAL_SW_BAUD     9600

#define WHEEL_D_F_PDIR     P1DIR
#define WHEEL_D_F_POUT     P1OUT
#define WHEEL_D_F          BIT6

#define WHEEL_D_R_PDIR     P1DIR
#define WHEEL_D_R_POUT     P1OUT
#define WHEEL_D_R          BIT7

#define WHEEL_T_R_PDIR     P1DIR
#define WHEEL_T_R_POUT     P1OUT
#define WHEEL_T_R          BIT4

#define WHEEL_T_F_PDIR     P1DIR
#define WHEEL_T_F_POUT     P1OUT
#define WHEEL_T_F          BIT3

#define SPEAKER_PDIR       P1DIR
#define SPEAKER_POUT       P1OUT
#define SPEAKER_PSEL       P1SEL
#define SPEAKER            BIT5

#define STARTUP_TUNE BEEP_C, BEEP_E, BEEP_D, BEEP_NULL

