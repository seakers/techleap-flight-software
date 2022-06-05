import math
from IMU_setup import *
from pan_motor_setup import *
from pid import PID

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


pan_home = 27.5
pan_lims = [pan_home + 15, pan_home - 15]

Kp = 1
Ki = 0
Kd = 0

if __name__ == "__main__":

    IMU = IMU_init()
    pan_motor = pan_init()

    panPID = PID(Kp, Kd, Ki)

    pan_d = 35
    print("Pan_desired: ", pan_d)

    
    pan_m = IMU_read(IMU).x
    print("Pan_measured: ", pan_m)

    while(abs(pan_d - pan_m)>0.75):

        pan = panPID.update(pan_d - pan_m, 0)
        print("Pan-PID: ", pan)

    
        if pan_m < pan_lims[0] and pan_m > pan_lims[1]:
            current_encoder_pos = panBySteps(pan_motor, -deg2steps(pan))
            time.sleep(3)
        else:
            print('Limit hit')

        pan_m = IMU_read(IMU).x
        print("Pan_measured: ", pan_m)
