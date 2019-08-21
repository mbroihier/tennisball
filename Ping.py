'''
Created on Aug 21, 2019

@author: broihier
'''
import time
import RPi.GPIO as GPIO

class Ping(object):
    '''
    Class for making a Send Ping object
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.state = "open"
        self.duration = 10e-6 
        self.pinOut = 17
        self.pinIn = 18
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pinOut, GPIO.OUT)
        GPIO.setup(self.pinIn, GPIO.IN)
        self.close()
        self.delta = 0.0

    def open(self):
        '''
        Open the switch (off/false) - this will set the voltage at the collector of the transistor to five volts
        '''
        GPIO.output(self.pinOut, False)

    def close(self):
        '''
        Close the switch (on/true) - this will set the voltage at the collector of the transistor to zero
        '''
        GPIO.output(self.pinOut, True)

    def pulse(self):
        '''
        Press the switch closed for duration seconds
        '''
        start = time.time()
        self.open()
        while time.time() - start < self.duration:
            pass
        self.close()
        entryTime = time.time()
        startTime = entryTime
        self.delta = 0.0
        while not GPIO.input(self.pinIn) and (startTime - entryTime < 0.5):
            startTime = time.time()
        while GPIO.input(self.pinIn):
            self.delta = time.time() - startTime
        return self.delta

    def start(self):
        '''
        Do startup stuff
        '''

    def stop(self):
        '''
        Do stop stuff
        '''
    def getDelta(self):
        '''
        Return delta time
        '''
        return self.delta

