import sys
from copy import deepcopy

# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, macros
from Basilisk.architecture import messaging
from Basilisk import __path__
bskPath = __path__[0]

# --> Module Imports
sys.path.insert(1, '/app')
from pymodules.Template.Module import Template




class SimulationClient:

    def __init__(self, time_step=1.0, duration=10.0):
        self.duration = macros.sec2nano(duration)
        self.time_step = macros.sec2nano(time_step)
        self.modules = {}

        # --> 1. Create simulation
        self.simulation = SimulationBaseClass.SimBaseClass()

        # --> 2. Create processes
        self.c_process = self.simulation.CreateNewProcess("c_process", 10)
        self.py_process = self.simulation.CreateNewPythonProcess("py_process", 9)

        # --> 3. Create base tasks
        self.new_c_task('base_task')
        self.new_py_task('base_task')

    def get_simulation(self):
        return self.simulation


    def new_c_task(self, task_name, time_step=None, priority=None):
        if time_step is None:
            time_step = self.time_step
        else:
            time_step = macros.sec2nano(time_step)
        if priority is None:
            new_task = self.simulation.CreateNewTask(task_name, time_step)
        else:
            new_task = self.simulation.CreateNewTask(task_name, time_step, priority)
        self.c_process.addTask(new_task)
        return None

    def new_c_module(self, module, task_name='base_task', priority=None):
        if priority is None:
            self.simulation.AddModelToTask(task_name, module)
        else:
            self.simulation.AddModelToTask(task_name, module, priority)

    def new_py_task(self, task_name, time_step=None):
        if time_step is None:
            time_step = self.time_step
        else:
            time_step = macros.sec2nano(time_step)
        self.py_process.createPythonTask(task_name, time_step)
        return None

    def new_py_module(self, module, task_name='base_task'):
        self.modules[module.get_name()] = module
        self.py_process.addModelToTask(task_name, module)

    def new_py_link(self, src_name, src_msg, dst_name, dst_msg):
        if src_name in self.modules and dst_name in self.modules:
            src_module = self.modules[src_name]
            dst_module = self.modules[dst_name]
            if hasattr(src_module, src_msg) and hasattr(dst_module, dst_msg):
                print('--> LINK:', src_name, src_msg, ' ---> ', dst_name, dst_msg)
                getattr(dst_module, dst_msg).subscribeTo(getattr(src_module, src_msg))
            else:
                print('--> MODULES DO NOT CONTAIN PROPER MESSAGE VARIABLES')
        else:
            print('--> MODULES NOT FOUND IN SIMULATION')


    def new_logging_var(self, variable, rate=None):
        if rate is None:
            rate = self.time_step
        self.simulation.AddVariableForLogging(variable, rate)

    def get_var_log_data(self, variable):
        return self.simulation.GetLogVariableData(variable)

    def run(self):
        self.simulation.InitializeSimulation()
        self.simulation.ConfigureStopTime(self.duration)
        self.simulation.ExecuteSimulation()




