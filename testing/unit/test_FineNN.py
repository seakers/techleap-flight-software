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
    # bskLogging.setDefaultLogLevel(bskLogging.BSK_WARNING)

    # fine_msg_rec = test_module.fine_msg.recorder()
    # sim_client.new_c_module(fine_msg_rec)












    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=param1, duration=param2)


    # --> 2. Create module
    test_module = FineNN.FineNN()
    test_module.ModelTag = "FineNN"
    sim_client.new_c_module(test_module)



    # --> 3. Create mock messages
    # vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    # vnir_msg_data.state = 20
    # vnir_msg_data.imageTensor = np.zeros([20, 20]).tolist()
    # vnir_msg = messaging.ImagerVNIROutMsg().write(vnir_msg_data)

    coarse_msg_data = messaging.CoarsePredictionMsgPayload()
    coarse_msg_data.prediction = 3
    coarse_msg = messaging.CoarsePredictionMsgPayload().write(coarse_msg_data)


    # --> 4. Subscribe to messages
    # test_module.vnir_msg.subscribeTo(vnir_msg)
    test_module.coarse_msg.subscribeTo(coarse_msg)


    # --> 5. Setup message recording


    # --> 6. Run simulation
    sim_client.run()

    return True






















