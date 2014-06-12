
#ifndef CORE_H
#define CORE_H

#include <msp430.h>

#include <stdint.h>

/* = Types = */

typedef uint8_t BOOL;

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

typedef enum {
   PORT1,
   PORT2
} PORT;

/* = Macros = */

#define STR_HELPER( num ) #num
#define STR( num ) STR_HELPER( num )

#endif /* CORE_H */

