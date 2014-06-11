
#define CPU msp430g2231

#define ENABLE_BEEP

#define ILED_PORT       PORT1
#define ILED            BIT0

#define WHEEL_D_F_PORT  PORT1
#define WHEEL_D_F       BIT6

#define WHEEL_D_R_PORT  PORT2
#define WHEEL_D_R       BIT5

#define WHEEL_T_R_PORT  PORT2
#define WHEEL_T_R       BIT4

#define WHEEL_T_F_PORT  PORT2
#define WHEEL_T_F       BIT3

#define SPEAKER_PORT    PORT1
#define SPEAKER         BIT7

#define STARTUP_TUNE BEEP_C, BEEP_E, BEEP_D, BEEP_NULL

