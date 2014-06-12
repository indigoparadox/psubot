
#ifndef SHELL_H
#define SHELL_H

/* = Includes = */

#include <msp430.h>

/* = Constants = */

#define SHELL_COMMAND_LEN 12
#define SHELL_ARG_COUNT 5

#ifdef ENABLE_SHELL_HELP
#define SHELL_STRING_PROMPT "READY\n\r"
#define SHELL_STRING_TOOLONG "TOO LONG\n\r"
#define SHELL_STRING_TOOMANYARGS "TOO MANY ARGS\n\r" 
#else
#define SHELL_STRING_PROMPT "R\n\r"
#define SHELL_STRING_TOOLONG "L\n\r"
#define SHELL_STRING_TOOMANYARGS "A\n\r" 
#endif /* ENABLE_SHELL_HELP */

/* = Structures = */

typedef struct {
   char* command;
   #ifdef ENABLE_SHELL_HELP
   char* helptext;
   #endif /* ENABLE_SHELL_HELP */
   void (*handler)( void );
} shell_command;

/* = Macros = */

#ifdef ENABLE_SHELL_HELP
#define SHELL_COMMANDS_BLOCK_ITEM( command, help, handler ) \
   { command, help, handler }

/* "count" should have the number of system-defined commands added to it in-  *
 * macro.                                                                     */
#define SHELL_COMMANDS_BLOCK_START( count ) \
   uint8_t gi_commands_count = (count + 1); \
   shell_command gao_commands[count + 1] = { \
   { "HELP", "DISPLAY THIS LIST.", shell_command_help },

#else

#define SHELL_COMMANDS_BLOCK_ITEM( command, help, handler ) \
   { command, handler }

/* "count" should have the number of system-defined commands added to it in-  *
 * macro.                                                                     */
#define SHELL_COMMANDS_BLOCK_START( count ) \
   uint8_t gi_commands_count = (count); \
   shell_command gao_commands[count] = { \

#endif /* ENABLE_SHELL_HELP */

#define SHELL_COMMANDS_BLOCK_END() };

#define SHELL_ENABLE() extern char gac_args[SHELL_ARG_COUNT][SHELL_COMMAND_LEN];

/* = Function Prototypes = */

void shell_init( void );
BOOL shell_strcmp( char*, char* );
#ifdef ENABLE_SHELL_HELP
void shell_command_help( void );
#endif /* ENABLE_SHELL_HELP */
void shell_task( char );

#endif /* SHELL_H */

