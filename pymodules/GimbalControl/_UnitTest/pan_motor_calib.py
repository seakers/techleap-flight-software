import math
import numpy as np
import matplotlib.pyplot as plt
from IMU_setup import *
from pan_motor_setup import *

def deg2steps(ndegrees):
    steps_per_rev = 51200
    one_rev = 360
    steps_per_deg = 1 * steps_per_rev / one_rev
    return math.ceil(ndegrees * steps_per_deg)

def steps2deg(nsteps):
    steps_per_rev = 51200
    one_rev = 360
    deg_per_steps = one_rev / steps_per_rev
    return nsteps * deg_per_steps
    

if __name__ == "__main__":

    IMU = IMU_init()
    pan_motor = pan_init()

    ypr = IMU_read(IMU)
    current_yaw = ypr.x

    yaw_log = np.array([])
    yaw_log = np.append(yaw_log, current_yaw)

    encoder_log = np.array([])
    
    for i in range(0, 21):

        current_encoder_pos = panBySteps(pan_motor, deg2steps(1))
        current_encoder_pos = current_encoder_pos.split('=')
        current_encoder_pos = steps2deg(int(current_encoder_pos[1])) 
        encoder_log = np.append(encoder_log, current_encoder_pos)
        
        ypr = IMU_read(IMU)
        current_yaw = ypr.x
        yaw_log = np.append(yaw_log, current_yaw)
        print(i)
        print(ypr)
        print(current_encoder_pos)

    gear_ratio = (encoder_log[-1] - encoder_log[0])/(yaw_log[-1] - yaw_log[0])

    print('Gear ratio = ', gear_ratio)
    
    plt.plot(yaw_log, marker="o", label='Yaw-IMU measurements')
    plt.plot(encoder_log, marker="+", label='pan-encoder-angle-w/o-gearRatio')
    plt.legend(loc="center left")
    plt.grid()
    plt.show()
