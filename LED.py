'''
Created on Aug 27, 2019

@author: broihier
'''
import RPi.GPIO as GPIO

class LED():
    '''
    Class for controlling the display LED
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.pinOut = 20
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pinOut, GPIO.OUT)
        GPIO.output(self.pinOut, False)

    def on(self):
        '''
        Turn on LED
        '''
        GPIO.output(self.pinOut, True)

    def off(self):
        '''
        Turn off LED
        '''
        GPIO.output(self.pinOut, False)

    def stop(self):
        '''
        Do stop stuff
        '''
        GPIO.output(self.pinOut, False)
        GPIO.cleanup()
