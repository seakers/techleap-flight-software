import sys
from copy import deepcopy

# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, macros
from Basilisk.architecture import messaging
from Basilisk import __path__
bskPath = __path__[0]

# --> Module Imports
sys.path.insert(1, '/app')
from modules.Template.Module import Template




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


    def new_c_task(self, task_name, time_step=None):
        if time_step is None:
            time_step = self.time_step
        else:
            time_step = macros.sec2nano(time_step)
        new_task = self.simulation.CreateNewTask(task_name, time_step)
        self.c_process.addTask(new_task)
        return None

    def new_c_module(self, module, task_name='base_task'):
        self.simulation.AddModelToTask(task_name, module)

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







    def new_py_mock_message(self, target_name, target_msg, data_vector):
        # --> 1. Create mock message from data_vector
        mock_message_data = messaging.CModuleTemplateMsgPayload()
        print('--> xx ', data_vector)
        mock_message_data.dataVector = deepcopy(data_vector)
        mock_message = messaging.CModuleTemplateMsg().write(mock_message_data)

        # --> 2. Subscribe to message
        if target_name in self.modules:
            target_module = self.modules[target_name]
            target_module_msg = getattr(target_module, target_msg, None)
            if target_module_msg is not None:
                target_module_msg.subscribeTo(mock_message)


    def run(self):
        self.simulation.InitializeSimulation()
        self.simulation.ConfigureStopTime(self.duration)
        self.simulation.ExecuteSimulation()




