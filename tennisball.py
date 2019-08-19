'''
Created on Aug 19, 2019

@author: broihier
'''
import time
import RPi.GPIO as GPIO
import SendPing
import ProcessPing

class TennisBall(object):
    '''
    Tennis Ball class
    '''
    def __init__(self):
        self.sendPing = SendPing.SendPing()
        self.processPing = ProcessPing.ProcessPing()

    def run_tennisball(self):
        '''
        run_tennisball - send pings and reports time between ping detections
        '''
        print("Starting receiver")
        self.processPing.start()
        print("Starting sender")
        self.sendPing.start()
        while True:
            try:
                print("Delta: ", self.processPing.getDelta())
                time.sleep(1)
                self.sendPing.pulse()
            except KeyboardInterrupt:
                print("Terminating via keyboard stop")
                self.sendPing.stop()
                self.processPing.stop()
                GPIO.cleanup()
                break
            except Exception as err:
                print(err)


if __name__ == "__main__":
    BALL = TennisBall()
    BALL.run_tennisball()
