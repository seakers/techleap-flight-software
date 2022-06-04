import math
from tilt_motor_setup import *
# print('SP'+str(200))

def deg2steps(ndegrees, steps_per_deg):
    return math.ceil(ndegrees * steps_per_deg)


steps_per_rev = 51200
one_rev = 360

steps_per_deg = 3 * steps_per_rev / one_rev

#print(steps_per_deg)
#print()

try:
    ser = motor_init() # Initialise the serial port
    ser.open()         # open the serial port
    time.sleep(1)
except Exception as e:
    print ('error opening serial port')
    exit()

if ser.isOpen():
    try:
        ser.flushInput()
        ser.flushOutput()
        motor_setup(ser)  # Complete motor setup and enable motor
    except Exception as e1:
        print ("Error Communicating...: " + str(e1))
else:
    print ("Cannot open serial port ")


nsteps = deg2steps(90,steps_per_deg)
current_encoder_pos = tiltBySteps(ser, nsteps)

print(current_encoder_pos)
