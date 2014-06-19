
#define CPU msp430g2553

#define ENABLE_EYE
#define ENABLE_BEEP
#define ENABLE_SHELL
#define ENABLE_SHELL_HELP
#define ENABLE_SERIAL_HW

#if 0
#define SERIAL_SW_TX    BIT1
#define SERIAL_SW_RX    BIT2
#define SERIAL_SW_BAUD  9600
#endif

#if 0
#define ENABLE_SERIAL_BT
#define SERIAL_BT_ID "PSUBot"
#define SERIAL_BT_CONNECT_MAC "00:1B:DC:0F:5C:4D"
#endif

#define EYE_LED_RED_PDIR   P2DIR
#define EYE_LED_RED_POUT   P2OUT
#define EYE_LED_RED        BIT0

#define EYE_LED_GREEN_PDIR P2DIR
#define EYE_LED_GREEN_POUT P2OUT
#define EYE_LED_GREEN      BIT1

#define EYE_LED_BLUE_PDIR  P2DIR
#define EYE_LED_BLUE_POUT  P2OUT
#define EYE_LED_BLUE       BIT2

#define EYE_SENSE_PDIR     P1DIR
#define EYE_SENSE_PIN      P1IN
#define EYE_SENSE          BIT3

/* Eye directions are from the robot's point of view. */
#define EYE_R_PDIR         P1DIR
#define EYE_R_POUT         P1OUT
#define EYE_R              BIT4

#define EYE_L_PDIR         P1DIR
#define EYE_L_POUT         P1OUT
#define EYE_L              BIT5

#define EYE_MAX_CYCLES_L   10500
#define EYE_MAX_LOOPS_R    40000

#define WHEEL_R_F_PDIR     P1DIR
#define WHEEL_R_F_POUT     P1OUT
#define WHEEL_R_F          BIT7

#define WHEEL_R_R_PDIR     P2DIR
#define WHEEL_R_R_POUT     P2OUT
#define WHEEL_R_R          BIT5

#define WHEEL_L_R_PDIR     P2DIR
#define WHEEL_L_R_POUT     P2OUT
#define WHEEL_L_R          BIT4

#define WHEEL_L_F_PDIR     P2DIR
#define WHEEL_L_F_POUT     P2OUT
#define WHEEL_L_F          BIT3

#define SPEAKER_PDIR       P1DIR
#define SPEAKER_POUT       P1OUT
#define SPEAKER_PSEL       P1SEL
#define SPEAKER            BIT6

#define STARTUP_TUNE       BEEP_C, BEEP_E, BEEP_D, BEEP_NULL

