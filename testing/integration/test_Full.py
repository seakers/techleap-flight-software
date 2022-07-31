import pytest
import sys
import numpy as np
import csv
sys.path.insert(1, '/home/ben/repos/techleap-flight-software')


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
        (1.0, 2.0, 1),
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
    # imu_module = InertialMeasurementUnit.InertialMeasurementUnit()
    # imu_module.ModelTag = "InertialMeasurementUnit"

    gc_module = GimbalControl.GimbalControl()
    gc_module.ModelTag = "GimbalControl"

    # sim_client.new_c_module(imu_module, task_name='C', priority=1)
    sim_client.new_c_module(gc_module, task_name='C', priority=5)


    # --> 3. Create mock messages
    vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    vnir_msg_data.state = 0
    # vnir_msg_data.red = np.zeros([512, 512], dtype=float).tolist()
    # vnir_msg_data.green = np.zeros([512, 512], dtype=float).tolist()
    # vnir_msg_data.blue = np.zeros([512, 512], dtype=float).tolist()
    # vnir_msg_data.nir = np.zeros([512, 512], dtype=float).tolist()
    red = np.genfromtxt('/home/ben/repos/techleap-flight-software/images/0_red.csv', delimiter=',', dtype=float)
    blue = np.genfromtxt('/home/ben/repos/techleap-flight-software/images/0_blue.csv', delimiter=',', dtype=float)
    green = np.genfromtxt('/home/ben/repos/techleap-flight-software/images/0_green.csv', delimiter=',', dtype=float)
    nir = np.genfromtxt('/home/ben/repos/techleap-flight-software/images/0_nir.csv', delimiter=',', dtype=float)
    vnir_msg_data.red = red.tolist()
    vnir_msg_data.green = green.tolist()
    vnir_msg_data.blue = blue.tolist()
    vnir_msg_data.nir = nir.tolist()
    vnir_msg = messaging.ImagerVNIROutMsg().write(vnir_msg_data)

    messages = []

    with open('/home/ben/repos/techleap-flight-software/testing/operable_only.csv', mode='r') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {", ".join(row)}')
                line_count += 1
            messages.append(row)
            line_count += 1
        print(f'Processed {line_count} lines.')

    # def get_consumer_msg():
    #     consumer_msg_data = messaging.MessageConsumerMsgPayload()
    #     consumer_msg_data.ins_state = param3
    #     consumer_msg_data.lat = float(messages[0]["Telemetry.INS.latitudeDecDeg"])
    #     consumer_msg_data.lon = float(messages[0]["Telemetry.INS.longitudeDecDeg"])
    #     consumer_msg_data.alt = float(messages[0]["Telemetry.INS.altitudeMeter"])
    #     consumer_msg_data.yaw = float(messages[0]["Telemetry.INS.yawDecDeg"])
    #     consumer_msg_data.pitch = float(messages[0]["Telemetry.INS.pitchDecDeg"])
    #     consumer_msg_data.roll = float(messages[0]["Telemetry.INS.rollDecDeg"])
    #     consumer_msg = messaging.MessageConsumerMsg().write(consumer_msg_data)
    #     return consumer_msg
    # def get_manual_msg():
    #     manual_msg_data = messaging.MessageConsumerManualMsgPayload()
    #     manual_msg_data.manual_plume = int(-1)
    #     manual_msg_data.lat = 15.0
    #     manual_msg_data.lon = 30.0
    #     manual_msg_data.alt = 0.0
    #     manual_msg = messaging.MessageConsumerManualMsg().write(manual_msg_data)
    #     return manual_msg


    # --> 4. Subscribe to messages
    fine_module.vnir_msg.subscribeTo(vnir_msg)

    storage_module.vnir_msg.subscribeTo(vnir_msg)
    storage_module.fine_msg.subscribeTo(fine_module.fine_msg)

    gc_module.fine_msg.subscribeTo(fine_module.fine_msg)
    gc_module.mode_msg.subscribeTo(cont_module.controller_mode_msg)
    gc_module.manual_msg.subscribeTo(msg_module.manual_msg)
    gc_module.ins_msg.subscribeTo(msg_module.balloon_msg)

    cont_module.fine_msg.subscribeTo(fine_module.fine_msg)
    cont_module.manual_msg.subscribeTo(msg_module.manual_msg)
    cont_module.ins_msg.subscribeTo(msg_module.balloon_msg)


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


















