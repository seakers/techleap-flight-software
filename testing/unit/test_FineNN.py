import pytest
import sys
import numpy as np
# sys.path.insert(1, '/home/gabe/repos/techleap/techleap-flight-software')
sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import FineNN

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
    test_module = FineNN.FineNN()
    test_module.ModelTag = "FineNN"
    sim_client.new_c_module(test_module)

    # --> 3. Create mock messages
    vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    vnir_msg_data.state = 20
    vnir_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    vnir_msg = messaging.ImagerVNIROutMsg().write(vnir_msg_data)

    thermal_msg_data = messaging.ImagerThermalOutMsgPayload()
    thermal_msg_data.state = 20
    thermal_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    thermal_msg = messaging.ImagerThermalOutMsg().write(thermal_msg_data)

    coarse_msg_data = messaging.CoarsePredictionMsgPayload()
    coarse_msg_data.prediction = 1
    coarse_msg_data.state = 1
    coarse_msg = messaging.CoarsePredictionMsg().write(coarse_msg_data)

    # --> 4. Subscribe to messages
    test_module.vnir_msg.subscribeTo(vnir_msg)
    test_module.thermal_msg.subscribeTo(thermal_msg)
    test_module.coarse_msg.subscribeTo(coarse_msg)

    # --> 5. Set output message recording
    output_rec = test_module.fine_msg.recorder()
    sim_client.new_c_module(output_rec)

    # --> 6. Set variable recording
    var1 = "FineNN.coarse_state"
    var2 = "FineNN.coarse_prediction"
    sim_client.new_logging_var(var1)
    sim_client.new_logging_var(var2)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
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


















