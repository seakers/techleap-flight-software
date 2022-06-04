import math
import numpy as np
import matplotlib.pyplot as plt
from IMU_setup import *
from tilt_motor_setup import *

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
    tilt_motor = tilt_init()

    ypr = IMU_read(IMU)
    current_pitch = ypr.y

    pitch_log = np.array([])
    pitch_log = np.append(pitch_log, current_pitch)

    encoder_log = np.array([])
    
    for i in range(0, 31):

        current_encoder_pos = tiltBySteps(tilt_motor, deg2steps(1))
        current_encoder_pos = current_encoder_pos.split('=')
        current_encoder_pos = steps2deg(int(current_encoder_pos[1])) 
        encoder_log = np.append(encoder_log, current_encoder_pos)
        
        ypr = IMU_read(IMU)
        current_pitch = ypr.y
        pitch_log = np.append(pitch_log, current_pitch)

        print(ypr)
        print(current_encoder_pos)

    gear_ratio = (encoder_log[-1] - encoder_log[0])/(pitch_log[-1] - pitch_log[0])

    print('Gear ratio = ', gear_ratio)
    
    plt.plot(pitch_log, marker="o", label='Pitch-IMU measurements')
    plt.plot(encoder_log/gear_ratio, marker="+", label='Tilt-encoder-angle-w/-gearRatio')
    plt.legend(loc="center left")
    plt.show()
