
CC = msp430-gcc -mmcu=msp430g2553 -Wall

all: test

# test: A general test/debug suite.
test:
	$(CC) -c psubot.c
	$(CC) -c uart.c
	$(CC) -c test.c
	$(CC) -o test.out test.o uart.o psubot.o

test_bluetooth:
	$(CC) -c psubot.c
	$(CC) -c uart.c
	$(CC) -c test_bluetooth.c
	$(CC) -o test_bluetooth.out test_bluetooth.o uart.o psubot.o

# test_eyesense: A simple test that will change the eye color to blue when the
# 					  motor right position sensor is depressed.
test_eyesense:
	$(CC) -c psubot.c
	$(CC) -c test_eyesense.c
	$(CC) -o test_eyesense.out test_eyesense.o psubot.o

flash-test:
	mspdebug rf2500 "prog ./test.out"

flash-test_eyesense:
	mspdebug rf2500 "prog ./test_eyesense.out"

flash-test_bluetooth:
	mspdebug rf2500 "prog ./test_bluetooth.out"

clean:
	rm *.o
	rm *.out

