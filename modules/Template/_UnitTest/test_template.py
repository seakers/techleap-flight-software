import pytest
import sys
sys.path.insert(1, '/app')

# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from modules.Template.Module import Template

# --> Basilisk Imports
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
    'param1, param2, param3',
    [
        (1, 2, 3),
        (1, 2, 3),
        (1, 2, 3),
    ]
)
def test_function(param1, param2, param3):

    # --> 1. Run test function
    result = run(param1, param2, param3)

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


def run(param1, param2, param3):
    print('\n\n\n--> TESTING MODULE')

    sim_client = SimulationClient()
    sim_client.new_py_module(Template('TemplateModule'))
    sim_client.run()

    return True











