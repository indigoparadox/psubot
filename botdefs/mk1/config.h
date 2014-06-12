
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

/*
#define ENABLE_SERIAL_BT
#define SERIAL_BT_ID "PSUBot"
*/

#define ILED_PORT       PORT1
#define ILED            BIT0

#define EYE_LED_RED_PORT    PORT2
#define EYE_LED_RED         BIT0

#define EYE_LED_GREEN_PORT  PORT2
#define EYE_LED_GREEN       BIT1

#define EYE_LED_BLUE_PORT   PORT2
#define EYE_LED_BLUE        BIT2

#define EYE_SENSE_PORT  PORT1
#define EYE_SENSE       BIT3

/* Eye directions are from the robot's point of view. */
#define EYE_R_PORT      PORT1
#define EYE_R           BIT4

#define EYE_L_PORT      PORT1
#define EYE_L           BIT5

#define EYE_MAX_CYCLES_L 10500
#define EYE_MAX_LOOPS_R 40000

#define WHEEL_R_F_PORT  PORT1
#define WHEEL_R_F       BIT6

#define WHEEL_R_R_PORT  PORT2
#define WHEEL_R_R       BIT5

#define WHEEL_L_R_PORT  PORT2
#define WHEEL_L_R       BIT4

#define WHEEL_L_F_PORT  PORT2
#define WHEEL_L_F       BIT3

#define SPEAKER_PORT    PORT1
#define SPEAKER         BIT7

#define STARTUP_TUNE BEEP_C, BEEP_E, BEEP_D, BEEP_NULL

