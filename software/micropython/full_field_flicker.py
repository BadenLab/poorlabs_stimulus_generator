import ports
import time
import support as sp
import sys

class FFF:
    def __init__(self):
        self.baseline = 2*1000 #baseline interval in ms
        self.onDuration  = 1000 #amount time each led is on in ms
        self.offDuration = 1000 #amount time each led is off in ms
        self.trials = 5 #number of trials
        #self.triggerInterval = 1000 #amount of time in between triggers in ms
        #self.triggerDuration = 100 #amount of time a trigger is turned on in ms
        self.ports = ports.Leds()
        
        
    def start(self,useBaseline=1):
        
        #users should always use baseline.
        #so that their recordings have a period
        #of no stimulation before the stimulation starts
        if useBaseline == 1:
            #count time for baseline
            print("starting baseline...",end = " ")
            sp.count_time_ms(counter=self.baseline,triggered=0)
            print("done baseline")
        for trial in range(self.trials):
            for led in self.ports.allLeds:
                
                led.duty(1024)
                print ("LED on info: ",led )
                #sys.stdout.write("\033")
                sp.count_time_ms(counter=self.onDuration,triggered=1,triggerPin=self.ports.trigger)
                led.duty(0)
                sp.count_time_ms(counter=self.offDuration,triggered=1,triggerPin=self.ports.trigger)
        
                
            