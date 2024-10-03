#!/usr/bin/env python3

import numpy as np
from rospy import is_shutdown,Rate,sleep

class PID():
    def __init__(self, Kp, Ki, Kd, setpoint, feedBack):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.setpoin = setpoint
        self.feedBack = feedBack

        self.time = 1
        self.integral = 0
        self.time_prev = -1e-6
        self.e_prev = 0
        self.error = setpoint - feedBack

        self.upperOut = 1000
        self.lowerOut = -1000
        self.round = 8
        self.roundError = 8

        self.out = 0

    def pid(self, setpoint, feedBack):

        self.time = self.time + 0.1*self.time
        # Value of offset. - when the error is equal zero
        # offset = 320
        
        # PID calculations
        self.error = round(setpoint - feedBack, self.roundError)
            
        P = self.Kp*self.error
        self.integral = self.integral + self.Ki*self.error*(self.time - self.time_prev)
        D = self.Kd*(self.error - self.e_prev)/(self.time - self.time_prev)

        # calculate manipulated variable - MV 
        self.out = round(P + self.integral + D , self.round)
        
        if(self.out >= self.upperOut):
            self.out = self.upperOut
        if(self.out <= self.lowerOut):
            self.out = self.lowerOut

        # update stored data for next iteration
        self.e_prev = self.error
        self.time_prev = self.time

        return self.out
    
    def pidCompute(self, feedBack):     #not complet yet

        while not is_shutdown() and self.error != 0 :

            self.out = self.pid(self.setpoin, feedBack)
            Rate.sleep(20)
            return self.out
        print("finish")
        

    def reset(self):

        self.e_prev = 0
        self.time = 1
        self.integral = 0
        self.time_prev = -1e-6
        self.out = 0

    def setLimite(self, upperOut, lowerOut):

        self.upperOut = upperOut
        self.lowerOut = lowerOut

    def setOutRound(self, roundIndex):
        
        self.round = roundIndex

    def setErrorRound(self, roundIndex):
        
        self.roundError = roundIndex


