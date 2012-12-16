
CC = msp430-gcc -mmcu=msp430g2553

all: test

test:
	$(CC) -c psubot.c
	$(CC) -c uart.c
	$(CC) -c test.c
	$(CC) -o test.out test.o uart.o psubot.o

flash-test:
	mspdebug rf2500 "prog ./test.out"

clean:
	rm *.o
	rm *.out

