'''
Created on Aug 19, 2019

@author: broihier
'''
import LED
import Ping

class TennisBall():
    '''
    Tennis Ball class
    '''
    def __init__(self):
        self.ping = Ping.Ping()
        self.led = LED.LED()

    def run_tennisball(self):
        '''
        run_tennisball - send pings and reports time between ping detections
        '''
        print("Starting IO")
        average = self.ping.start()

        while True:
            try:
                currentDistance = self.ping.pulse()
                print("Delta: ", currentDistance)
                if currentDistance < average:
                    self.led.on()
                else:
                    self.led.off()
            except KeyboardInterrupt:
                print("Terminating via keyboard stop")
                self.ping.stop()
                self.led.stop()
                break
            except Exception as err:
                print(err)


if __name__ == "__main__":
    BALL = TennisBall()
    BALL.run_tennisball()
