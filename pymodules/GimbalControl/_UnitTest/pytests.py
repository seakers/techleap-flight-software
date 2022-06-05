import math
import numpy as np
from tilt_motor_setup import *
# print('SP'+str(200))

def deg2steps(ndegrees, steps_per_deg):
    return math.ceil(ndegrees * steps_per_deg)


tilt_home = -80
tilt_lims = [tilt_home + 45, tilt_home - 45]
print(tilt_lims[0])
