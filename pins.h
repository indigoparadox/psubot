
#ifndef PINS_H
#define PINS_H

#include "core.h"

int pins_in( PORT, int );
void pins_dir_or( PORT, int );
void pins_out_or( PORT, int );
void pins_out_and( PORT, int );

#endif /* PINS_H */

