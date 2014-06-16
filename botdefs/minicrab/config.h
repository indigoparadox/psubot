
#define CPU msp430g2211

#define ENABLE_SHELL

/* Software serial only supported on port 1. */

#define SERIAL_SW_TX    BIT2
#define SERIAL_SW_RX    BIT1
#define SERIAL_SW_BAUD  9600

#define ILED_PORT       PORT1
#define ILED            BIT0

#define WHEEL_D_F_PORT  PORT1
#define WHEEL_D_F       BIT6

#define WHEEL_D_R_PORT  PORT1
#define WHEEL_D_R       BIT7

#define WHEEL_T_R_PORT  PORT1
#define WHEEL_T_R       BIT4

#define WHEEL_T_F_PORT  PORT1
#define WHEEL_T_F       BIT3

#define SPEAKER_PORT    PORT1
#define SPEAKER         BIT5

#define STARTUP_TUNE BEEP_C, BEEP_E, BEEP_D, BEEP_NULL

