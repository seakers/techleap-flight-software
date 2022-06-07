import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/repos/techleap-flight-software')
# sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import GimbalControl

# --> Messaging Import
from Basilisk.architecture import messaging

# --> Basilisk logging
from Basilisk.architecture import bskLogging




# """
#   _______        _     ______                _   _
#  |__   __|      | |   |  ____|              | | (_)
#     | | ___  ___| |_  | |__ _   _ _ __   ___| |_ _  ___  _ __
#     | |/ _ \/ __| __| |  __| | | | '_ \ / __| __| |/ _ \| '_ \
#     | |  __/\__ \ |_  | |  | |_| | | | | (__| |_| | (_) | | | |
#     |_|\___||___/\__| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
#
# """


@pytest.mark.parametrize(
    'param1, param2',
    [
        (1.0, 1.0),
    ]
)
def test_function(param1, param2):

    # --> 1. Run test function
    result = run(param1, param2)

    # --> 2. Assert result
    assert result is True

    # --> 3. Set options
    __tracebackhide__ = True





# """
#    _____  _                    _         _    _
#   / ____|(_)                  | |       | |  (_)
#  | (___   _  _ __ ___   _   _ | |  __ _ | |_  _   ___   _ __
#   \___ \ | || '_ ` _ \ | | | || | / _` || __|| | / _ \ | '_ \
#   ____) || || | | | | || |_| || || (_| || |_ | || (_) || | | |
#  |_____/ |_||_| |_| |_| \__,_||_| \__,_| \__||_| \___/ |_| |_|
#
# """


def run(param1, param2):

    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=param1, duration=param2)

    # --> 2. Create modules
    #imu_module = InertialMeasurementUnit.InertialMeasurementUnit()
    #imu_module.ModelTag = "IMU"

    gimbal_module = GimbalControl.GimbalControl()
    gimbal_module.ModelTag = "GimbalControl"

    controller_module = Controller.Controller()

    #sim_client.new_c_module(imu_module, priority=1)
    sim_client.new_c_module(gimbal_module, priority=2)
    sim_client.new_c_module(controller_module, priority=3)


    # --> 3. Create mock messages
    def get_imu_msg(yaw=0.0, pitch=0.0, roll=0.0, temperature=0.0):
        imu_msg_data = messaging.IMUOutMsgPayload()
        imu_msg_data.state = 0
        imu_msg_data.yaw = yaw
        imu_msg_data.pitch = pitch
        imu_msg_data.roll = roll
        imu_msg_data.temperature = temperature
        imu_msg = messaging.IMUOutMsg().write(imu_msg_data)
        return imu_msg

    # --> 4. Subscribe to messages
    gimbal_module.imu_msg.subscribeTo(get_imu_msg())
    gimbal_module.mode_msg.subscribeTo(controller_module.controller_mode_msg)


    # --> 5. Set output message recording
    #imu_rec = imu_module.imu_msg.recorder()

    #sim_client.new_c_module(imu_rec)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    #print(imu_rec.state)


    return True

















