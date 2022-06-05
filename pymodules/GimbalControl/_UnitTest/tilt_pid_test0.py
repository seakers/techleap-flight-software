import math
from IMU_setup import *
from tilt_motor_setup import *
from pid import PID

def deg2steps(ndegrees):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 3.5866
    steps_per_deg = gear_ratio * steps_per_rev / one_rev
    return math.ceil(ndegrees * steps_per_deg)

def steps2deg(nsteps):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 3.5866
    deg_per_steps = one_rev / steps_per_rev / gear_ratio
    return nsteps * deg_per_steps


# global variables
# Limits
tilt_home = -80 # from IMU measurements at home
tilt_lims = [tilt_home + 30, tilt_home - 30]


def limitTilt(tilt, upperLimit=tilt_lims[0], lowerLimit=tilt_lims[1]):
    if tilt > upperLimit:
        tilt = upperLimit
    elif tilt < lowerLimit:
        tilt = lowerLimit
    return tilt


"""
def setupPID():
    Kp = 0.9
    Ki = 0
    Kd = 0.1
"""

Kp = 0.9
Ki = 0
Kd = 0.1



if __name__ == "__main__":

    IMU = IMU_init()
    tilt_motor = tilt_init()
    
    tiltPID = PID(Kp, Kd, Ki)

    # desired tilt angle

    tilt_d = -55
    print("Tilt_desired: ", tilt_d)


    # measured tilt angle
    ypr = IMU_read(IMU)
    tilt_m = ypr.y
    print("Tilt_measured: ", tilt_m)
    #current_encoder_pos = tiltBySteps(tilt_motor, deg2steps(5))

    tilt = tilt_d - tilt_m
    print("tilt", tilt)
    #current_encoder_pos = tiltBySteps(tilt_motor, -deg2steps(tilt))
    #time.sleep(5)
    
    ypr = IMU_read(IMU)
    tilt_m = ypr.y
    print("Tilt_measured: ", tilt_m)
    

    
    while (abs(tilt_d - tilt_m)>1):
        print('Init')
        
        #tilt_d = tilt_home - 12;
        print("Tilt_desired: ", tilt_d)
        
        ypr = IMU_read(IMU)
        tilt_m = ypr.y          # measured tilt angle
        print("Tilt_measured: ", tilt_m)
        
        # error -> change in pitch 
        tilt = tiltPID.update(tilt_d - tilt_m, 0)
        print('Tilt-PID:', tilt)
        
        

    
        if tilt_m < tilt_lims[0] and tilt_m > tilt_lims[1]: 
            current_encoder_pos = tiltBySteps(tilt_motor, -deg2steps(tilt))
            time.sleep(3)
        else:
            print('Limit hit')

        ypr = IMU_read(IMU)
        tilt_m = ypr.y          # measured tilt angle
        print("Tilt_measured: ", tilt_m)
    
    """
    for i in range(0, 5):
        current_encoder_pos = tiltBySteps(tilt_motor, deg2steps(-1))
        ypr = IMU_read(IMU)
        print(ypr.y)
    """

    
