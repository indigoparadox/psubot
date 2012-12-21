#!/usr/bin/python
from Tkinter import *
import serial, time

sPSUBot = serial.Serial( '/dev/rfcomm0', 9600, timeout=1 );

def send_command( wnd_app_in, command_in ):
   sPSUBot.write( command_in + "\r" )

# Create the window...
wnd_info = Tk()
wnd_info.config( padx=10, pady=10, bg="black" )
wnd_info.title( "Alexander's Recovery Fob" )

# Create the window controls...

wnd_info.frm_buttons_led = Frame( wnd_info )
wnd_info.frm_buttons_led.btn_led_red = Button(
   wnd_info.frm_buttons_led,
   text="LED: Red",
   command=lambda: send_command( wnd_info, 'LED RED' )
)
wnd_info.frm_buttons_led.btn_led_green = Button(
   wnd_info.frm_buttons_led,
   text="LED: Green",
   command=lambda: send_command( wnd_info, 'LED GREEN' )
)
wnd_info.frm_buttons_led.btn_led_blue = Button(
   wnd_info.frm_buttons_led,
   text="LED: Blue",
   command=lambda: send_command( wnd_info, 'LED BLUE' )
)

wnd_info.frm_buttons_eye = Frame( wnd_info )
wnd_info.frm_buttons_eye.btn_eye_left = Button(
   wnd_info.frm_buttons_eye,
   text="Eye: Left",
   command=lambda: send_command( wnd_info, 'EYE L 10' )
)
wnd_info.frm_buttons_eye.btn_eye_right = Button(
   wnd_info.frm_buttons_eye,
   text="Eye: Right",
   command=lambda: send_command( wnd_info, 'EYE R 10' )
)

wnd_info.frm_buttons = Frame( wnd_info )
wnd_info.frm_buttons.btnClose = Button( 
   wnd_info.frm_buttons, text="Close", command=lambda: wnd_info.quit()
)

# Pack the window controls...
wnd_info.frm_buttons_led.pack( side=TOP )
wnd_info.frm_buttons_eye.pack( side=TOP )
wnd_info.frm_buttons.pack( side=TOP )
wnd_info.frm_buttons_led.btn_led_red.pack( side=LEFT )
wnd_info.frm_buttons_led.btn_led_green.pack( side=LEFT )
wnd_info.frm_buttons_led.btn_led_blue.pack( side=LEFT )
wnd_info.frm_buttons_eye.btn_eye_left.pack( side=LEFT )
wnd_info.frm_buttons_eye.btn_eye_right.pack( side=LEFT )
wnd_info.frm_buttons.btnClose.pack( side=RIGHT )

# Wait for user input...
wnd_info.mainloop()

try:
    wnd_info.destroy()
except:
    pass

