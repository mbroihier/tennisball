'''
Created on Aug 19, 2019

@author: broihier
'''
import time
import RPi.GPIO as GPIO
import Ping

class TennisBall(object):
    '''
    Tennis Ball class
    '''
    def __init__(self):
        self.ping = Ping.Ping()

    def run_tennisball(self):
        '''
        run_tennisball - send pings and reports time between ping detections
        '''
        print("Starting IO")
        self.ping.start()
        while True:
            try:
                print("Delta: ", self.ping.pulse())
                time.sleep(1)
            except KeyboardInterrupt:
                print("Terminating via keyboard stop")
                self.ping.stop()
                GPIO.cleanup()
                break
            except Exception as err:
                print(err)


if __name__ == "__main__":
    BALL = TennisBall()
    BALL.run_tennisball()
