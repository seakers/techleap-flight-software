import pytest
import sys
import numpy as np

# sys.path.insert(1, '/home/gabe/repos/techleap/techleap-flight-software')
sys.path.insert(1, '/app')

# --> Simulation Import
from simulation.api import SimulationClient
from simulation.mock_messages import get_controller_mode_msg, get_vnir_msg, get_thermal_msg, get_geo_msg, get_fine_msg

# --> Module Import
from Basilisk.ExternalModules import DataStorage

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


def run(param1, param2):
    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=param1, duration=param2)

    # --> 2. Create module
    test_module = DataStorage.DataStorage()
    test_module.ModelTag = "DataStorage"
    sim_client.new_c_module(test_module)

    # --> 3. Create mock messages
    vnir_msg = get_vnir_msg()
    thermal_msg = get_thermal_msg()
    fine_msg = get_fine_msg()
    mode_msg = get_controller_mode_msg()

    # --> 4. Subscribe to messages
    test_module.vnir_msg.subscribeTo(vnir_msg)
    test_module.thermal_msg.subscribeTo(thermal_msg)
    test_module.fine_msg.subscribeTo(fine_msg)
    test_module.mode_msg.subscribeTo(mode_msg)

    # --> 6. Set variable recording
    var1 = "DataStorage.state"
    sim_client.new_logging_var(var1)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    var1 = sim_client.get_var_log_data(var1)

    return True














