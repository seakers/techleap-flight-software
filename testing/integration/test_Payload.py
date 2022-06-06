import pytest
import sys
import numpy as np
sys.path.insert(1, '/home/ben/repos/techleap-flight-software')
# sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerThermal, ImagerVNIR, DataStorage, CoarseNN, FineNN

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

    # --> 2. Create modules
    vnir_module = ImagerVNIR.ImagerVNIR()
    vnir_module.ModelTag = "ImagerVNIR"

    fine_module = FineNN.FineNN()
    fine_module.ModelTag = "FineNN"

    storage_module = DataStorage.DataStorage()
    storage_module.ModelTag = "DataStorage"


    sim_client.new_c_module(vnir_module,    priority=1)
    sim_client.new_c_module(fine_module,    priority=2)
    sim_client.new_c_module(storage_module, priority=3)


    # --> 3. Create mock messages


    # --> 4. Subscribe to messages

    fine_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)

    storage_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)
    storage_module.fine_msg.subscribeTo(fine_module.fine_msg)


    # --> 5. Set output message recording
    vnir_rec = vnir_module.vnir_msg.recorder()
    fine_rec = fine_module.fine_msg.recorder()

    sim_client.new_c_module(vnir_rec)
    sim_client.new_c_module(fine_rec)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    print(vnir_rec.state)


    return True

















