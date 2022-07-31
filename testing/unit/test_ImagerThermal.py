import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/nvme/repos/techleap-flight-software')
# sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerThermal

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
    test_module = ImagerThermal.ImagerThermal()
    test_module.ModelTag = "ImagerThermal"
    sim_client.new_c_module(test_module)

    # --> 3. Set output message recording
    output_rec = test_module.thermal_msg.recorder()
    sim_client.new_c_module(output_rec)

    # --> 4. Set variable recording
    var1 = "ImagerThermal.state"
    var2 = "ImagerThermal.image_tensor"
    sim_client.new_logging_var(var1)
    sim_client.new_logging_var(var2)

    # --> 5. Run simulation
    sim_client.run()

    # --> 6. Get debug output
    var1 = sim_client.get_var_log_data(var1)
    var2 = sim_client.get_var_log_data(var2)

    print(output_rec.state)


    return True





def get_tensor(value, rows=20, cols=20):
    tensor = []
    for x in range(rows):
        row = []
        for y in range(cols):
            row.append(value)
        tensor.append(row)
    return tensor


















