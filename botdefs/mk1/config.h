
#define ILED_PORT       PORT1
#define ILED            BIT0

#define LED_RED_PORT    PORT2
#define LED_RED         BIT0

#define LED_GREEN_PORT  PORT2
#define LED_GREEN       BIT1

#define LED_BLUE_PORT   PORT2
#define LED_BLUE        BIT2

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

