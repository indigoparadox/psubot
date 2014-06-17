
#ifndef PINS_H
#define PINS_H

#include "core.h"

/* = Constants = */

typedef enum {
   PORT1,
   PORT2
} PORT;

/* = Function Prototypes = */

int pins_in( PORT, int );
void pins_dir_or( PORT, int );
void pins_out_or( PORT, int );
void pins_out_and( PORT, int );
void pins_out_toggle( PORT, int );

#endif /* PINS_H */

