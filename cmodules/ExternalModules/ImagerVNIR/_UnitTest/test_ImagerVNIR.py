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
from Basilisk.moduleTemplates import ImagerVNIR                # import the module that is to be tested
from Basilisk.utilities import macros
from Basilisk.architecture import messaging                      # import the message definitions
from Basilisk.architecture import bskLogging


# Uncomment this line is this test is to be skipped in the global unit test run, adjust message as needed.
# @pytest.mark.skipif(conditionstring)
# Uncomment this line if this test has an expected failure, adjust message as needed.
# @pytest.mark.xfail(conditionstring)
# Provide a unique test method name, starting with 'test_'.
# The following 'parametrize' function decorator provides the parameters and expected results for each
# of the multiple test runs for this test.  Note that the order in that you add the parametrize method
# matters for the documentation in that it impacts the order in which the test arguments are shown.
# The first parametrize arguments are shown last in the pytest argument list
@pytest.mark.parametrize("accuracy", [1e-12])
@pytest.mark.parametrize("param1, param2", [
     (1, 1)
    ,(1, 3)
    ,(2, 2)
])

# update "module" in this function name to reflect the module name
def test_module(show_plots, param1, param2, accuracy):
    r"""
    **Validation Test Description**

    Compose a general description of what is being tested in this unit test script.  Add enough information so the
    reader understands the purpose and limitations of the test.  As this test script is not parameterized, only one
    version of this script will run.  Note that the ``pytest`` HTML report will list each parameterized test case
    individually.  This way it is clear what set of parameters passed.  But, this also means that this doc-string
    content will be copied into each report so each test description is individually complete.  If there is a
    discussion you want to include that is specific to the a parameterized test case, then include this at the
    end of the file with a conditional print() statement that only executes for that particular parameterized test.

    **Test Parameters**

    As this is a parameterized unit test, note that the test case parameters values are shown automatically in the
    pytest HTML report.  This sample script has the parameters param1 and param 2.  Provide a description of what
    each parameter controls.  This is a convenient location to include the accuracy variable used in the
    validation test.

    Args:
        param1 (int): Dummy test parameter for this parameterized unit test
        param2 (int): Dummy test parameter for this parameterized unit test
        accuracy (float): absolute accuracy value used in the validation tests

    **Description of Variables Being Tested**

    Here discuss what parameters are being checked.  For example, in this file we are checking the values of the
    variables

    - ``dummy``
    - ``dataVector[3]``

    **Figure Discussion**

    If the test script produces figures you might include a brief discussion on what the simulation results show.
    Discuss why these results validate the operation of the BSK module.

    **General Documentation Comments**

    If the script generates figures, these figures will be automatically pulled from ``matplotlib`` and included below.
    Make sure that the figures have appropriate axes labels and a figure title if needed.  The figures content
    should be understood by just looking at the figure.

    At the end of the script where a print statement says that the script passes.

    Don't use any of the AutoTeX methods we used to use as the goal is to have all the validation reporting
    contained within this HTML ``pytest`` report.
    """
    # each test method requires a single assert method to be called
    result= cppModuleTestFunction(show_plots, param1, param2, accuracy)
    assert 0 < 1


def cppModuleTestFunction(show_plots, param1, param2, accuracy):



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


#
# This statement below ensures that the unitTestScript can be run as a
# stand-along python script
#
if __name__ == "__main__":
    test_module(              # update "module" in function name
                 False,
                 1,           # param1 value
                 1,           # param2 value
                 1e-12        # accuracy
               )
