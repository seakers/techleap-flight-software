import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/nvme/repos/techleap-flight-software')
#sys.path.insert(1, '/app')


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
        (1.0, 2.0),
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

def get_test_data():
    image_file = '/app/images/test_dataset_'
    return None



def run(param1, param2):

    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=param1, duration=param2)

    # --> 2. Create module
    test_module = GimbalControl.GimbalControl()
    test_module.ModelTag = "GimbalControl"
    sim_client.new_c_module(test_module)

    # --> 3. Create mock messages
    adcs_angles_msg_data = messaging.AttitudeDeterminationAnglesMsgPayload()
    adcs_angles_msg_data.state = 20
    adcs_angles_msg_data.yaw = 1.0
    adcs_angles_msg_data.pitch = 1.0
    adcs_angles_msg_data.roll = 1.0
    adcs_angles_msg = messaging.AttitudeDeterminationAnglesMsg().write(adcs_angles_msg_data)

    controller_angles_msg_data = messaging.ControllerManualAnglesMsgPayload()
    controller_angles_msg_data.state = 20
    controller_angles_msg_data.pan = 1.0
    controller_angles_msg_data.tilt = 1.0
    controller_angles_msg = messaging.ControllerManualAnglesMsg().write(controller_angles_msg_data)

    # --> 4. Subscribe to messages
    test_module.adcs_angles_msg.subscribeTo(adcs_angles_msg)
    test_module.controller_angles_msg.subscribeTo(controller_angles_msg)


    # --> 6. Set variable recording
    var1 = "GimbalControl.state"
    sim_client.new_logging_var(var1)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    var1 = sim_client.get_var_log_data(var1)

    return True



















