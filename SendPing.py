'''
Created on Aug 19, 2019

@author: broihier
'''
import time
import RPi.GPIO as GPIO

class SendPing(object):
    '''
    Class for making a Send Ping object
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.state = "open"
        self.duration = 10e-6 
        self.pin = 17
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pin, GPIO.OUT)
        self.close()

    def open(self):
        '''
        Open the switch (off/false) - this will set the voltage at the collector of the transistor to five volts
        '''
        GPIO.output(self.pin, False)

    def close(self):
        '''
        Close the switch (on/true) - this will set the voltage at the collector of the transistor to zero
        '''
        GPIO.output(self.pin, True)

    def pulse(self):
        '''
        Press the switch closed for duration seconds
        '''
        start = time.time()
        self.open()
        while time.time() - start < self.duration:
            pass
        self.close()

    def start(self):
        '''
        Do startup stuff
        '''

    def stop(self):
        '''
        Do stop stuff
        '''

