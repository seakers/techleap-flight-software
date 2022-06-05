import math
from IMU_setup import *
from pan_motor_setup import *

def deg2steps(ndegrees):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 1
    steps_per_deg = gear_ratio * steps_per_rev / one_rev
    return math.ceil(ndegrees * steps_per_deg)

def steps2deg(nsteps):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 1
    deg_per_steps = one_rev / steps_per_rev / gear_ratio
    return nsteps * deg_per_steps

if __name__ == "__main__":

    IMU = IMU_init()
    pan_motor = pan_init()

    
    for i in range(0, 5):
        current_encoder_pos = panBySteps(pan_motor, deg2steps(-2))
        ypr = IMU_read(IMU)
        print(ypr.x)
        

    
