
#ifndef CORE_H
#define CORE_H

#include <msp430.h>

/* = Types = */

typedef int BOOL;

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

#endif /* CORE_H */

