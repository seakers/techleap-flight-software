import pytest
import sys
sys.path.insert(1, '/app')

# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from modules.Mock.Module import Mock

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

    # --> 2. Add modules
    sim_client.new_py_module(Mock('MockModule1', mode='mtest_m1'))
    sim_client.new_py_module(Mock('MockModule2', mode='mtest_m2'))
    sim_client.new_py_module(Mock('MockModule3', mode='mtest_m3'))

    # --> 3. Link modules
    sim_client.new_py_link('MockModule1', 'p_msg_out', 'MockModule2', 'p_msg_in')
    sim_client.new_py_link('MockModule2', 'p_msg_out', 'MockModule3', 'p_msg_in')

    sim_client.run()

    return True











