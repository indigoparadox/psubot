
CC = msp430-gcc

CFLAGS = -mmcu=msp430g2553 -Wall

BASE_SYSTEM = botlib.o beep.o
INTERACTIVE_SYSTEM = uart.o shell.o

all: psubot

psubot: psubot.o $(BASE_SYSTEM) $(INTERACTIVE_SYSTEM)
	$(CC) $(CFLAGS) -o $@.out $^

# = Generic Utility Definitions =

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

flash:
	mspdebug rf2500 "prog ./psubot.out"

clean:
	rm *.o
	rm *.out

