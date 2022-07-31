import sys
import os

# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, macros
from Basilisk import __path__
bskPath = __path__[0]
fileName = os.path.basename(os.path.splitext(__file__)[0])



sys.path.insert(1, '/app')
from pymodules.ImagerVNIR.RGBImager import RGBImager
from pymodules.CoarseNN.Module import CoarseNN
from pymodules.GimbalControl.Gimbal import Gimbal



def run():
    simTaskName = "simTask"
    simProcessName = "simProcess"
    pyProcessName = "pyProcess"
    pyTaskName = "pyTask"

    # --> Create simulation
    simulation = SimulationBaseClass.SimBaseClass()
    simulationTime = macros.sec2nano(3.0)
    simulationTimeStep = macros.sec2nano(1.0)


    # --> Create process and task
    sim_process = simulation.CreateNewProcess(simProcessName, 10)
    sim_process.addTask(simulation.CreateNewTask(simTaskName, simulationTimeStep))
    py_process = simulation.CreateNewPythonProcess(pyProcessName, 9)
    py_process.createPythonTask(pyTaskName, simulationTimeStep)



    # --> Add pymodules to task
    module_rgb = RGBImager('RGBImager')
    module_ml = CoarseNN('Classifier')
    module_gimbal = Gimbal('Gimbal')
    py_process.addModelToTask(pyTaskName, module_rgb)
    py_process.addModelToTask(pyTaskName, module_ml)
    py_process.addModelToTask(pyTaskName, module_gimbal)



    # --> Connect module messaging
    module_ml.imageInMsg.subscribeTo(module_rgb.imageOutMsg)
    module_gimbal.classificationInMsg.subscribeTo(module_ml.classificationOutMsg)


    # --> Execute simulation
    simulation.InitializeSimulation()
    simulation.ConfigureStopTime(simulationTime)
    simulation.ExecuteSimulation()























if __name__ == "__main__":
    run()

