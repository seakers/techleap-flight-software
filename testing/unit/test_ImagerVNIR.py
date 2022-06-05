import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/repos/techleap-flight-software')
#sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerVNIR

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
    test_module = ImagerVNIR.ImagerVNIR()
    test_module.ModelTag = "ImagerVNIR"
    sim_client.new_c_module(test_module)

    # --> 3. Set output message recording
    output_rec = test_module.vnir_msg.recorder()
    sim_client.new_c_module(output_rec)

    # --> 4. Set variable recording
    var1 = "ImagerVNIR.state"
    var2 = "ImagerVNIR.red"
    var3 = "ImagerVNIR.green"
    var4 = "ImagerVNIR.blue"
    var5 = "ImagerVNIR.nir"
    sim_client.new_logging_var(var1)
    sim_client.new_logging_var(var2)
    sim_client.new_logging_var(var3)
    sim_client.new_logging_var(var4)
    sim_client.new_logging_var(var5)

    # --> 5. Run simulation
    sim_client.run()

    # --> 6. Get debug output
    var1 = sim_client.get_var_log_data(var1)
    var2 = sim_client.get_var_log_data(var2)
    var3 = sim_client.get_var_log_data(var3)
    var4 = sim_client.get_var_log_data(var4)
    var5 = sim_client.get_var_log_data(var5)
    print(var2.shape)

    print(output_rec.red)


    return True















