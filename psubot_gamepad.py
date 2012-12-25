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
   0: {
      'down': lambda: psubot_command( 'LED RED' ),
   },
   1: { 
      'down': lambda: psubot_command( 'LED GREEN' ),
   },
   2: {
      'down': lambda: psubot_command( 'LED BLUE' ),
   },
   3: {
      'down': lambda: psubot_command( 'BEEP 100 100' ),
   },
   4: {
      'down': lambda: psubot_command( 'EYE L' ),
      'up': lambda: psubot_command( 'EYE S' ),
   },
   5: {
      'down': lambda: psubot_command( 'EYE R' ),
      'up': lambda: psubot_command( 'EYE S' ),
   },
   6: {
      'down': lambda: psubot_command( 'DRIVE PL' ),
      'up': lambda: psubot_command( 'DRIVE S' ),
   },
   7: {
      'down': lambda: psubot_command( 'DRIVE PR' ),
      'up': lambda: psubot_command( 'DRIVE S' ),
   }
}

gamepad_axes = {
   3: { 
      -1.0: lambda: psubot_command( 'DRIVE L' ),
      1.0: lambda: psubot_command( 'DRIVE R' ),
      0.0: lambda: psubot_command( 'DRIVE S' ),
   },
   4: {
      -1.0: lambda: psubot_command( 'DRIVE F' ),
      1.0: lambda: psubot_command( 'DRIVE B' ),
      0.0: lambda: psubot_command( 'DRIVE S' ),
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
         action = None
         try:
            action = gamepad_buttons.get( event.button )['down']
         except KeyError:
            print event.button
         if None != action:
            action()
         else:
            print event.button
      if event.type == pygame.JOYBUTTONUP:
         action = None
         try:
            action = gamepad_buttons.get( event.button )['up']
         except KeyError:
            print event.button
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

