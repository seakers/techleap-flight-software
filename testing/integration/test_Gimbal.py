import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/repos/techleap-flight-software')
# sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import InertialMeasurementUnit, GimbalControl

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
    gimbal_module = GimbalControl.GimbalControl()
    gimbal_module.ModelTag = "GimbalControl"

    imu_module = InertialMeasurementUnit.InertialMeasurementUnit()
    imu_module.ModelTag = "IMU"

    sim_client.new_c_module(gimbal_module, priority=1)
    sim_client.new_c_module(imu_module, priority=1)


    # --> 3. Create mock messages


    # --> 4. Subscribe to messages
    gimbal_module.imu_angles_msg.subscribeTo(imu_module.imu_msg)


    # --> 5. Set output message recording
    imu_rec = imu_module.imu_msg.recorder()

    sim_client.new_c_module(imu_rec)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    print(imu_rec.state)


    return True

















