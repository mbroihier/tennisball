'''
Created on Aug 19, 2019

@author: broihier
'''
import time
import RPi.GPIO as GPIO

class ProcessPing(object):
    '''
    Class for processing a Send Ping return
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.pin = 18
        self.lastPulseTime = time.time()
        self.delta = 0
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pin, GPIO.IN)

    def pulseReturn(self):
        '''
        Press the switch closed for duration seconds
        '''
        nextLastTime = time.time()
        self.delta = nextLastTime - self.lastPulseTime
        self.lastPulseTime = nextLastTime

    def start(self):
        '''
        Do startup stuff
        '''
        GPIO.remove_event_detect(self.pin)
        time.sleep(5)
        GPIO.add_event_detect(self.pin, GPIO.RISING, callback=self.pulseReturn)

    def stop(self):
        '''
        Do stop stuff
        '''
        GPIO.remove_event_detect(self.pin)
        
    def getDelta(self):
        '''
        Return the difference in time
        '''
        return self.delta
