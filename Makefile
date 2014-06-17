
# = Feature Checks =

BASE_SYSTEM = scheduler.o drive.o

USE_EYE_CHECK=$(shell grep '^.define ENABLE_EYE' botdefs/$(BOT)/config.h 2>/dev/null)
ifneq ($(USE_EYE_CHECK),)
BASE_SYSTEM += eye.o
endif

USE_BEEP_CHECK=$(shell grep '^.define ENABLE_BEEP' botdefs/$(BOT)/config.h 2>/dev/null)
ifneq ($(USE_BEEP_CHECK),)
BASE_SYSTEM += beep.o
endif

USE_SHELL_CHECK=$(shell grep '^.define ENABLE_SHELL' botdefs/$(BOT)/config.h 2>/dev/null)
ifneq ($(USE_SHELL_CHECK),)
BASE_SYSTEM += shell.o uart.o
endif

CPU=$(shell grep '^.define CPU' botdefs/$(BOT)/config.h | awk '{print $$3}' 2>/dev/null)

CC = msp430-gcc
CFLAGS = -mmcu=$(CPU) -Wall -Os -Wl,-gc-sections -include botdefs/$(BOT)/config.h

all: psubot

psubot: psubot.o $(BASE_SYSTEM) $(INTERACTIVE_SYSTEM)
	$(CC) $(CFLAGS) -Wl,-Map=$@.map,--cref -o $@.out $^
	#msp430-strip $@.out

# = Generic Utility Definitions =

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

flash:
	sudo mspdebug rf2500 "prog ./psubot.out"

clean:
	rm *.o
	rm *.out

