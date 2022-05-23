import pytest
import sys
import numpy as np
# sys.path.insert(1, '/home/gabe/repos/techleap/techleap-flight-software')
sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerThermal, ImagerVNIR, DataStorage, CoarseNN, FineNN, Controller, GimbalControl, InertialMeasurementUnit, MessageConsumer, Gps, Photogrammetry, AttitudeDetermination

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
        (1.0, 2.0),
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

def get_test_data():
    image_file = '/app/images/test_dataset_'
    return None



def run(param1, param2):

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

    thermal_module = ImagerThermal.ImagerThermal()
    thermal_module.ModelTag = "ImagerThermal"

    coarse_module = CoarseNN.CoarseNN()
    coarse_module.ModelTag = "CoarseNN"

    fine_module = FineNN.FineNN()
    fine_module.ModelTag = "FineNN"

    storage_module = DataStorage.DataStorage()
    storage_module.ModelTag = "DataStorage"

    sim_client.new_c_module(vnir_module, task_name='B', priority=1)
    sim_client.new_c_module(thermal_module, task_name='B', priority=2)
    sim_client.new_c_module(coarse_module, task_name='B', priority=3)
    sim_client.new_c_module(fine_module, task_name='B', priority=4)
    sim_client.new_c_module(storage_module, task_name='B', priority=5)

    # --> 4. Task C
    imu_module = InertialMeasurementUnit.InertialMeasurementUnit()
    imu_module.ModelTag = "InertialMeasurementUnit"

    gps_module = Gps.Gps()
    gps_module.ModelTag = "Gps"

    photo_module = Photogrammetry.Photogrammetry()
    photo_module.ModelTag = "Photogrammetry"

    ad_module = AttitudeDetermination.AttitudeDetermination()
    ad_module.ModelTag = "AttitudeDetermination"

    gc_module = GimbalControl.GimbalControl()
    gc_module.ModelTag = "GimbalControl"

    sim_client.new_c_module(imu_module, task_name='C', priority=1)
    sim_client.new_c_module(gps_module, task_name='C', priority=2)
    sim_client.new_c_module(photo_module, task_name='C', priority=3)
    sim_client.new_c_module(ad_module, task_name='C', priority=4)
    sim_client.new_c_module(gc_module, task_name='C', priority=5)


    # --> 3. Create mock messages
    vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    vnir_msg_data.state = 0
    vnir_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    vnir_msg = messaging.ImagerVNIROutMsg().write(vnir_msg_data)

    thermal_msg_data = messaging.ImagerThermalOutMsgPayload()
    thermal_msg_data.state = 0
    thermal_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    thermal_msg = messaging.ImagerThermalOutMsg().write(thermal_msg_data)


    # --> 4. Subscribe to messages
    vnir_module.mock_msg.subscribeTo(vnir_msg)
    thermal_module.mock_msg.subscribeTo(thermal_msg)

    coarse_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)
    coarse_module.thermal_msg.subscribeTo(thermal_module.thermal_msg)

    fine_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)
    fine_module.thermal_msg.subscribeTo(thermal_module.thermal_msg)
    fine_module.coarse_msg.subscribeTo(coarse_module.coarse_msg)

    storage_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)
    storage_module.thermal_msg.subscribeTo(thermal_module.thermal_msg)
    storage_module.fine_msg.subscribeTo(fine_module.fine_msg)


    # --> 5. Set output message recording
    vnir_rec = vnir_module.vnir_msg.recorder()
    thermal_rec = thermal_module.thermal_msg.recorder()
    coarse_rec = coarse_module.coarse_msg.recorder()
    fine_rec = fine_module.fine_msg.recorder()

    sim_client.new_c_module(vnir_rec)
    sim_client.new_c_module(thermal_rec)
    sim_client.new_c_module(coarse_rec)
    sim_client.new_c_module(fine_rec)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    print(vnir_rec.state)


    return True





def get_tensor(value, rows=20, cols=20):
    tensor = []
    for x in range(rows):
        row = []
        for y in range(cols):
            row.append(value)
        tensor.append(row)
    return tensor


















