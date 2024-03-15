
'''

This is a pico macro keyboard based on CircuitPython adafruit_hid library
The PCB is currently in version one and thus the key layout is all crazy
like this:

    ****This will be fixed in Final version***

         key[0]    Key[1]   Key[2]

         key[4]    Key[7]   Key[3]

         key[5]    Key[6]   Key[8]

         key[11]    Key[10]   Key[9]

'''
#Import all the relevant Libraries


#Import all the relevant Libraries

import time
import board
import digitalio
import usb_hid
from adafruit_hid.keycode import Keycode
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS
from adafruit_hid.consumer_control import ConsumerControl
from adafruit_hid.consumer_control_code import ConsumerControlCode


# Set up Consumer Control - Control Codes can be found here: https://docs.circuitpython.org/projects/hid/en/latest/_modules/adafruit_hid/consumer_control_code.html#ConsumerControlCode
cc = ConsumerControl(usb_hid.devices)

# Set up a keyboard device. - Keycode can be found here: https://docs.circuitpython.org/projects/hid/en/latest/_modules/adafruit_hid/keycode.html#Keycode
keyboard = Keyboard(usb_hid.devices)

# Set up keyboard to write strings from macro
write_text = KeyboardLayoutUS(keyboard)

# Rearranged button order
#buttons = [board.GP0, board.GP1, board.GP2, board.GP4, board.GP7, board.GP3, board.GP5, board.GP6, board.GP8, board.GP11, board.GP10, board.GP9]
buttons = [board.GP0, board.GP1, board.GP2, board.GP3, board.GP7, board.GP4, board.GP5, board.GP6, board.GP8, board.GP11, board.GP10, board.GP9]
key = [digitalio.DigitalInOut(pin_name) for pin_name in buttons]
for x in range(0,len(buttons)):
    key[x].direction = digitalio.Direction.INPUT
    key[x].pull = digitalio.Pull.DOWN



while True:
    if key[0].value:
        keyboard.send(Keycode.ZERO)
        time.sleep(0.1)
        
    if key[1].value:
        keyboard.send(Keycode.ONE)
        time.sleep(0.1)
        
    if key[2].value:
        keyboard.send(Keycode.TWO)
        time.sleep(0.1)
        
    if key[3].value:
        keyboard.send(Keycode.THREE)
        time.sleep(0.1)
        
    if key[4].value:
        keyboard.send(Keycode.FOUR)
        time.sleep(0.1)
        
    if key[5].value:
        keyboard.send(Keycode.FIVE)
        time.sleep(0.1)
        
    if key[6].value:
        keyboard.send(Keycode.SIX)
        time.sleep(0.1)
        
    if key[7].value:
        keyboard.send(Keycode.SEVEN)
        time.sleep(0.1)
        
    if key[8].value:
        keyboard.send(Keycode.EIGHT)
        time.sleep(0.1)
        
    if key[9].value:
        cc.send(ConsumerControlCode.SCAN_PREVIOUS_TRACK)
        time.sleep(0.1)
        
    if key[10].value:
        cc.send(ConsumerControlCode.PLAY_PAUSE)

        time.sleep(0.1)
        
    if key[11].value:
        cc.send(ConsumerControlCode.SCAN_NEXT_TRACK)
        time.sleep(0.1)

    time.sleep(0.1)