import pytest
import sys

sys.path.insert(1, '/app')

# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from pymodules.Mock.Module import Mock

# --> Basilisk Imports
from Basilisk.architecture import messaging
from Basilisk.utilities import SimulationBaseClass
from Basilisk.utilities import unitTestSupport
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
    print('\n\n\n----> TESTING MODULE <-----')

    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=param1, duration=param2)

    # --> 2. Create target module
    module = Mock('MockModule2', mode='mtest_m2')



    # # --> 3. Create mock message
    # mock_message_data = messaging.CModuleTemplateMsgPayload()  # Create a structure for the input message
    # mock_message_data.dataVector = [1.0, -0.5, 0.7]  # Set up a list as a 3-vector
    # mock_message = messaging.CModuleTemplateMsg().write(mock_message_data)
    #
    # # --> 4. Subscribe to mock message
    # module.p_msg_in.subscribeTo(mock_message)

    # --> 5. Add module to simulation
    sim_client.new_py_module(module)

    sim_client.new_py_mock_message('MockModule2', 'p_msg_in', (1.0, -0.5, 0.7))

    # --> 6. Run simulation
    sim_client.run()

    return True











