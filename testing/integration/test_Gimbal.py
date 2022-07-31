import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/nvme/repos/techleap-flight-software')
# sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import GimbalControl, Controller, InertialMeasurementUnit

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
        (1.0, 100.0),
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
    imu_module = InertialMeasurementUnit.InertialMeasurementUnit()
    imu_module.ModelTag = "IMU"

    gimbal_module = GimbalControl.GimbalControl()
    gimbal_module.ModelTag = "GimbalControl"

    controller_module = Controller.Controller()
    controller_module.ModelTag = "Controller"

    #sim_client.new_c_module(imu_module, priority=1)
    sim_client.new_c_module(gimbal_module, priority=2)
    sim_client.new_c_module(controller_module, priority=3)


    # --> 3. Create mock messages
    def get_fine_msg():
        fine_msg_data = messaging.FinePredictionMsgPayload()
        fine_msg_data.state = int(0)
        fine_msg_data.pan = int(0)
        fine_msg_data.tilt = int(0)
        fine_msg_data.mask = np.zeros([512, 512], dtype=float).tolist()
        fine_msg = messaging.FinePredictionMsg().write(fine_msg_data)
        return fine_msg

    def get_controller_mode_msg(mode=2):
        mode_msg_data = messaging.ControllerModeMsgPayload()
        mode_msg_data.mode = mode
        mode_msg = messaging.ControllerModeMsg().write(mode_msg_data)
        print(mode_msg_data.mode)
        return mode_msg

    # --> 4. Subscribe to messages
    #gimbal_module.fine_msg.subscribeTo(get_fine_msg())
    gimbal_module.mode_msg.subscribeTo(get_controller_mode_msg(2))


    # --> 5. Set output message recording
    #imu_rec = imu_module.imu_msg.recorder()

    #sim_client.new_c_module(imu_rec)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    #print(imu_rec.state)


    return True

















