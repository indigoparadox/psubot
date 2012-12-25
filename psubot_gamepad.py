#!/usr/bin/python

import pygame
import serial, time

pygame.init()

gamepad = None
for i in range( 0, pygame.joystick.get_count() ):
   j = pygame.joystick.Joystick( i )
   if 'USB Gamepad' == j.get_name().strip():
      print "Found " + j.get_name()
      gamepad = j
      gamepad.init()

if None == gamepad:
   print "Joystick not found!"
   die()

print "Ready!"

gamepad_buttons = {
   0: lambda: psubot_command( 'LED RED' ),
   1: lambda: psubot_command( 'LED GREEN' ),
   2: lambda: psubot_command( 'LED BLUE' ),
   3: lambda: psubot_command( 'BEEP 100 100' ),
   4: lambda: psubot_command( 'EYE L 10' ),
   5: lambda: psubot_command( 'EYE R 10' ),
   6: lambda: psubot_command( 'DRIVE PL 10000' ),
   7: lambda: psubot_command( 'DRIVE PR 10000' ),
}

gamepad_axes = {
   3: { 
      -1.0: lambda: psubot_command( 'DRIVE L 10000' ),
      1.0: lambda: psubot_command( 'DRIVE R 10000' ),
   },
   4: {
      -1.0: lambda: psubot_command( 'DRIVE F 10000' ),
      1.0: lambda: psubot_command( 'DRIVE B 10000' ),
   },
}

# Try to connect to the psubot.
serial_psubot = serial.Serial( '/dev/rfcomm0', 9600, timeout=1 );

def psubot_command( command_in ):
   global serial_psubot
   try:
      serial_psubot.write( command_in + "\r" )
   except serial.SerialException:
      while None == serial_psubot:
         print 'Connection lost. Attempting to reconnect...'
         serial_psubot = serial.Serial( '/dev/rfcomm0', 9600, timeout=1 );
      serial_psubot.write( command_in + "\r" )
   #print command_in

try:
   while True:
      event = pygame.event.poll()
      '''for i in range( 0, gamepad.get_numaxes() ):
         if gamepad.get_axis( i ) != 0.00:
            #print i
            pass'''
      '''for i in range( 0, gamepad.get_numbuttons() ):
         if gamepad.get_button( i ):
            print i'''

      if event.type == pygame.JOYBUTTONDOWN:
         action = gamepad_buttons.get( event.button )
         if None != action:
            action()
         else:
            print event.button
      elif event.type == pygame.JOYAXISMOTION:
         action = gamepad_axes.get( event.axis )
         if None != action:
            action = action.get( event.value )
         if None != action:
            action()
         else:
            print event.value
            print event.axis
         

except KeyboardInterrupt:
   gamepad.quit()

