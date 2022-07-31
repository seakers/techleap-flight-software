import pytest
import sys
import numpy as np
# sys.path.insert(1, '/home/gabe/repos/techleap/techleap-flight-software')
sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerThermal, ImagerVNIR, DataStorage, CoarseNN, FineNN

# --> Messaging Import
from Basilisk.architecture import messaging

# --> Basilisk logging
from Basilisk.architecture import bskLogging

from Basilisk.utilities import macros





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
        (1.0, 10.0),
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
    sim_client.new_c_task('A', time_step=1)
    sim_client.new_c_task('B', time_step=2)


    # --> 2. Create modules
    vnir_module = ImagerVNIR.ImagerVNIR()
    vnir_module.ModelTag = "ImagerVNIR"

    storage_module = DataStorage.DataStorage()
    storage_module.ModelTag = "DataStorage"


    sim_client.new_c_module(vnir_module, task_name='A', priority=1)
    sim_client.new_c_module(storage_module, task_name='B', priority=1)


    # --> 3. Create mock messages
    fine_nn_msg_data = messaging.FinePredictionMsgPayload()
    fine_nn_msg_data.state = 0
    fine_nn_msg_data.mask = np.zeros([20, 20], dtype=int).tolist()
    fine_nn_msg = messaging.FinePredictionMsg().write(fine_nn_msg_data)

    thermal_msg_data = messaging.ImagerThermalOutMsgPayload()
    thermal_msg_data.state = 0
    thermal_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    thermal_msg = messaging.ImagerThermalOutMsg().write(thermal_msg_data)


    # --> 4. Subscribe to messages
    storage_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)



    # --> 6. Run simulation
    sim_client.run()



    return True















