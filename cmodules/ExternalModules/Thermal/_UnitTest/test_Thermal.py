import pytest
import sys
import numpy as np
import os
# sys.path.insert(1, '/home/gabe/repos/techleap/techleap-flight-software')
sys.path.insert(1, '/home/ben/nvme/repos/techleap-flight-software')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import Thermal

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
    'image_file, label_file',
    [
        ('/app/images/VNIR/image_1.nc', '/app/images/VNIR/label_1.nc'),
    ]
)
def test_function(image_file, label_file):
    print('Running test...')

    # --> 1. Run test function
    result = run()
    # result = True

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


def run():

    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=1.0, duration=2.0)

    # --> 2. Create module
    test_module = Thermal.Thermal()
    test_module.ModelTag = "Thermal"
    sim_client.new_c_module(test_module)

    # --> 4. Subscribe to messages
    test_module.thermal_msg.subscribeTo(thermal_msg)

    # --> 5. Set output message recording
    output_rec = test_module.fine_msg.recorder()
    sim_client.new_c_module(output_rec)

    # --> 6. Set variable recording
    var1 = "Thermal.state"
    sim_client.new_logging_var(var1)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    var1 = sim_client.get_var_log_data(var1)

    print(output_rec.state)


    return True