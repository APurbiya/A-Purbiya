import RPi.GPIO as GPIO
import time


GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)


pins = [11, 13, 15, 8, 10, 12]  


for pin in pins:
    GPIO.setup(pin, GPIO.OUT, initial=GPIO.HIGH)

try:
    while True:
        # Pattern for center direction (C)
        GPIO.output(8, GPIO.LOW)  # Green
        GPIO.output(10, GPIO.HIGH)  # Red
        GPIO.output(12, GPIO.HIGH)  # Blue

        GPIO.output(15, GPIO.HIGH)  # Blue
        GPIO.output(11, GPIO.HIGH)  # Red
        GPIO.output(13, GPIO.LOW)  # Green

        time.sleep(10)  # Wait for 10 seconds

        GPIO.output(8, GPIO.HIGH)  # Green
        GPIO.output(10, GPIO.HIGH)  # Red
        GPIO.output(12, GPIO.HIGH)  # Blue

        GPIO.output(15, GPIO.HIGH)  # Blue
        GPIO.output(11, GPIO.HIGH)  # Red
        GPIO.output(13, GPIO.HIGH)  # Green

        time.sleep(3)  # Wait for 3 seconds

        # Pattern for left direction (L)
        GPIO.output(15, GPIO.HIGH)  # Blue
        GPIO.output(11, GPIO.HIGH)  # Red
        GPIO.output(13, GPIO.HIGH)  # Green

        GPIO.output(8, GPIO.HIGH)  # Green
        GPIO.output(10, GPIO.HIGH)  # Red
        GPIO.output(12, GPIO.LOW)  # Blue

        time.sleep(10)  # Wait for 10 seconds

        GPIO.output(12, GPIO.HIGH)  # Blue
        GPIO.output(15, GPIO.HIGH)  # Blue

        GPIO.output(8, GPIO.HIGH)  # Green
        GPIO.output(10, GPIO.HIGH)  # Red

        time.sleep(3)  # Wait for 3 seconds

        # Pattern for right direction (R)
        GPIO.output(8, GPIO.HIGH)  # Green
        GPIO.output(10, GPIO.HIGH)  # Red
        GPIO.output(12, GPIO.HIGH)  # Blue

        GPIO.output(15, GPIO.HIGH)  # Blue
        GPIO.output(11, GPIO.LOW)  # Green
        GPIO.output(13, GPIO.HIGH)  # Red

        time.sleep(10)  # Wait for 10 seconds

        GPIO.output(15, GPIO.HIGH)  # Blue
        GPIO.output(11, GPIO.HIGH)  # Red
        GPIO.output(13, GPIO.HIGH)  # Green

        GPIO.output(8, GPIO.HIGH)  # Green
        GPIO.output(10, GPIO.HIGH)  # Red
        GPIO.output(12, GPIO.HIGH)  # Blue

        time.sleep(3)  # Wait for 3 seconds

except KeyboardInterrupt:
    
    GPIO.cleanup()
