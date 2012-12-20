
#ifndef SHELL_H
#define SHELL_H

/* = Constants = */

#define SHELL_COMMAND_LEN 10
#define SHELL_COMMAND_COUNT 10

/* = Structures = */

typedef struct {
   char* command;
   char* helptext;
   void (*handler)( char* );
} shell_command;

/* = Macros = */

#define SHELL_COMMANDS_BLOCK_START( count ) \
   int gi_commands_count = count; \
   shell_command ao_commands[count] = {

#define SHELL_COMMANDS_BLOCK_ITEM( command, help, handler ) \
   { command, help, handler }

#define SHELL_COMMANDS_BLOCK_END() };

/* = Function Prototypes = */

void shell_init( void );
BOOL shell_strcmp( char*, char* );

#endif /* SHELL_H */

