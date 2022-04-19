#
#  ISC License
#
#  Copyright (c) 2021, Autonomous Vehicle Systems Lab, University of Colorado at Boulder
#
#  Permission to use, copy, modify, and/or distribute this software for any
#  purpose with or without fee is hereby granted, provided that the above
#  copyright notice and this permission notice appear in all copies.
#
#  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#


#
#   Unit Test Script
#   Module Name:        cppModuleTemplateParametrized
#   Author:             (First Name) (Last Name)
#   Creation Date:      Month Day, Year
#

import pytest
import os, inspect
import numpy as np

filename = inspect.getframeinfo(inspect.currentframe()).filename
path = os.path.dirname(os.path.abspath(filename))
bskName = 'Basilisk'
splitPath = path.split(bskName)







# Import all of the pymodules that we are going to be called in this simulation
from Basilisk.utilities import SimulationBaseClass
from Basilisk.utilities import unitTestSupport                  # general support file with common unit test functions
import matplotlib.pyplot as plt
from Basilisk.ExternalModules import ImagerVNIR                # import the module that is to be tested
from Basilisk.utilities import macros
from Basilisk.architecture import messaging                      # import the message definitions
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
@pytest.mark.parametrize()
@pytest.mark.parametrize()
def test_module():
    
    # --> Call test function
    result = run_test()
    assert 0 < 1


def run_test():
    simulation = SimulationBaseClass.SimBaseClass()

    # --> Add process
    testProc = simulation.CreateNewProcess("TestProcess")

    # --> Add task
    testProc.addTask(simulation.CreateNewTask("UnitTask", macros.sec2nano(1.0)))

    # --> Create test module
    test_module = ImagerVNIR.ImagerVNIR()
    test_module.ModelTag = "ImagerVNIR"

    # --> Add module to task
    simulation.AddModelToTask("UnitTask", test_module)

    # --> Configure simulation runtime and execute
    simulation.ConfigureStopTime(macros.sec2nano(1.0))        # seconds to stop simulation
    simulation.ExecuteSimulation()

    return None


# --> Allow test to be ran stand-alone
if __name__ == "__main__":
    test_module()
