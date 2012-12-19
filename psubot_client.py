#!/usr/bin/python
import serial, time

sPSUBot = serial.Serial( '/dev/msp430serial', 9600, timeout=1 );

while 1:
   #s = sPSUBot.readline()
   sPSUBot.write( "LED\r" )
   time.sleep( 2 )

