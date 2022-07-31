import pytest
import sys
import numpy as np
import csv
sys.path.insert(1, '/home/ben/nvme/repos/techleap-flight-software')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerVNIR, DataStorage, FineNN, Controller, GimbalControl, InertialMeasurementUnit, MessageConsumer

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
    'param1, param2, param3',
    [
        (1.0, 1000.0, 1),
    ]
)
def test_function(param1, param2, param3):

    # --> 1. Run test function
    result = run(param1, param2, param3)

    # --> 2. Assert result
    assert result is True

    # --> 3. Set options
    __tracebackhide__ = False





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

    # --> 1. Create simulation client + tasks
    sim_client = SimulationClient(time_step=param1, duration=param2)
    sim_client.new_c_task('A', time_step=1)
    sim_client.new_c_task('B', time_step=1)
    sim_client.new_c_task('C', time_step=1)

    # --> 2. Task A
    cont_module = Controller.Controller()
    cont_module.ModelTag = "Controller"

    msg_module = MessageConsumer.MessageConsumer()
    msg_module.ModelTag = "MessageConsumer"

    sim_client.new_c_module(cont_module, task_name='A', priority=1)
    sim_client.new_c_module(msg_module, task_name='A', priority=2)


    # --> 3. Task B
    vnir_module = ImagerVNIR.ImagerVNIR()
    vnir_module.ModelTag = "ImagerVNIR"

    fine_module = FineNN.FineNN()
    fine_module.ModelTag = "FineNN"

    storage_module = DataStorage.DataStorage()
    storage_module.ModelTag = "DataStorage"


    sim_client.new_c_module(vnir_module, task_name='B', priority=1)
    sim_client.new_c_module(fine_module, task_name='B', priority=2)
    sim_client.new_c_module(storage_module, task_name='B', priority=3)

    # --> 4. Task C
    imu_module = InertialMeasurementUnit.InertialMeasurementUnit()
    imu_module.ModelTag = "InertialMeasurementUnit"

    gc_module = GimbalControl.GimbalControl()
    gc_module.ModelTag = "GimbalControl"

    sim_client.new_c_module(imu_module, task_name='C', priority=1)
    sim_client.new_c_module(gc_module, task_name='C', priority=5)


    # --> 4. Subscribe to messages
    fine_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)

    storage_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)
    storage_module.fine_msg.subscribeTo(fine_module.fine_msg)
    storage_module.imu_msg.subscribeTo(imu_module.imu_msg)
    storage_module.ins_msg.subscribeTo(msg_module.balloon_msg)
    storage_module.controller_msg.subscribeTo(cont_module.controller_msg)

    gc_module.fine_msg.subscribeTo(fine_module.fine_msg)
    gc_module.mode_msg.subscribeTo(cont_module.controller_mode_msg)
    gc_module.manual_msg.subscribeTo(msg_module.manual_msg)
    gc_module.ins_msg.subscribeTo(msg_module.balloon_msg)
    gc_module.imu_msg.subscribeTo(imu_module.imu_msg)

    cont_module.fine_msg.subscribeTo(fine_module.fine_msg)
    cont_module.manual_msg.subscribeTo(msg_module.manual_msg)
    cont_module.ins_msg.subscribeTo(msg_module.balloon_msg)
    cont_module.imu_msg.subscribeTo(imu_module.imu_msg)
    cont_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)

    msg_module.telemetry_msg.subscribeTo(storage_module.telemetry_msg)

    # --> 5. Set output message recording
    vnir_rec = vnir_module.vnir_msg.recorder()
    fine_rec = fine_module.fine_msg.recorder()

    sim_client.new_c_module(vnir_rec)
    sim_client.new_c_module(fine_rec)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    print(vnir_rec.state)


    return True


















