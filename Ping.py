'''
Created on Aug 21, 2019

@author: broihier
'''
import time
import RPi.GPIO as GPIO

class Ping():
    '''
    Class for making a Send Ping object
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.state = "open"
        self.duration = 10e-6
        self.pinOut = 21
        self.pinIn = 18
        self.average = 0.0
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pinOut, GPIO.OUT)
        GPIO.setup(self.pinIn, GPIO.IN)
        self.close()
        self.delta = 0.0

    def open(self):
        '''
        Open the switch (off/false) - this will set the voltage at the collector of the transistor
        to 5 volts
        '''
        GPIO.output(self.pinOut, False)

    def close(self):
        '''
        Close the switch (on/true) - this will set the voltage at the collector of the transistor to
        zero
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
        count = 0
        accumulator = 0.0
        while count < 100:
            sample = self.pulse()
            accumulator += sample
            count += 1
            if abs(sample*count - accumulator) > 0.000100 * count:
                print("Reseting at count:", count, " sample:", sample, " accumulator:", accumulator)
                count = 0
                accumulator = 0.0
        self.average = accumulator/count
        return self.average

    def stop(self):
        '''
        Do stop stuff
        '''
    def getDelta(self):
        '''
        Return delta time
        '''
        return self.delta
