import RPi.GPIO as GPIO
import os, sys
import time
import argparse
camTrigger = 17
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(camTrigger, GPIO.IN, GPIO.PUD_UP) # gpio pin set to high by default 
def picture(shutter):
    os.system(f'{shutter}')
shutter = "libcamera-still -t 10000 --autofocus -0 newimage.jpg"
def capture():
    while True:
        if GPIO.input(camTrigger) == True:
            print("Shutter Triggered")
            picture(shutter)
            while GPIO.input(camTrigger) == True:
                time.sleep(0.1)
            print("Shutter Off")
capture()
GPIO.cleanup()
