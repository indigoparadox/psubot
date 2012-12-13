
CC = msp430-gcc -mmcu=msp430g2553

all: bin

bin:
	$(CC) -c psubot.c
	$(CC) -o psubot.out psubot.o

flash:
	mspdebug rf2500 "prog ./psubot.out"

clean:
	rm *.o
	rm *.out

