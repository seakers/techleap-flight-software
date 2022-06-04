# VectorNAV
from vnpy import *


def IMU_init(): 
    s = VnSensor()
    try:
        s.connect('COM3', 115200)
        return s
    except Exception as e:
        print ('Error opening IMU')
        exit()


    
def IMU_read(s):
    #print(s.read_model_number())
    ypr = s.read_yaw_pitch_roll()
    #print(ypr)  
    return ypr




