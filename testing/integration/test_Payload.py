import pytest
import sys
import numpy as np
import csv
sys.path.insert(1, '/home/ben/nvme/repos/techleap-flight-software')
# sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import ImagerVNIR, DataStorage, FineNN, MessageConsumer

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




def run(param1, param2):

    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=param1, duration=param2)

    # --> 2. Create modules
    vnir_module = ImagerVNIR.ImagerVNIR()
    vnir_module.ModelTag = "ImagerVNIR"

    fine_module = FineNN.FineNN()
    fine_module.ModelTag = "FineNN"

    storage_module = DataStorage.DataStorage()
    storage_module.ModelTag = "DataStorage"

    msg_module = MessageConsumer.MessageConsumer()
    msg_module.ModelTag = "MessageConsumer"

    sim_client.new_c_module(vnir_module,    priority=1)
    sim_client.new_c_module(fine_module,    priority=2)
    sim_client.new_c_module(storage_module, priority=3)
    sim_client.new_c_module(msg_module, priority = 2)

    messages = []

    with open('/home/ben/nvme/repos/techleap-flight-software/testing/operable_only.csv', mode='r') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {", ".join(row)}')
                line_count += 1
            messages.append(row)
            line_count += 1
        print(f'Processed {line_count} lines.')

    # --> 3. Create mock messages
    def get_fine_msg():
        fine_msg_data = messaging.FinePredictionMsgPayload()
        fine_msg_data.state = 0
        fine_msg_data.mask = np.zeros([512, 512], dtype=float).tolist()
        fine_msg = messaging.FinePredictionMsg().write(fine_msg_data)
        return fine_msg
    def get_imu_msg(yaw=10.0, pitch=15.0, roll=20.0, temperature=0.0):
        imu_msg_data = messaging.IMUOutMsgPayload()
        imu_msg_data.state = 0
        imu_msg_data.yaw = yaw
        imu_msg_data.pitch = pitch
        imu_msg_data.roll = roll
        imu_msg_data.temperature = temperature
        imu_msg = messaging.IMUOutMsg().write(imu_msg_data)
        return imu_msg
    def get_consumer_msg():
        consumer_msg_data = messaging.MessageConsumerMsgPayload()
        consumer_msg_data.state = 0
        consumer_msg_data.msg = 0
        consumer_msg_data.lat = float(messages[0]["Telemetry.INS.latitudeDecDeg"])
        consumer_msg_data.lon = float(messages[0]["Telemetry.INS.longitudeDecDeg"])
        consumer_msg_data.alt = float(messages[0]["Telemetry.INS.altitudeMeter"])
        print(consumer_msg_data.lat)
        print(consumer_msg_data.lon)
        print(consumer_msg_data.alt)
        consumer_msg = messaging.MessageConsumerMsg().write(consumer_msg_data)
        return consumer_msg

    # --> 4. Subscribe to messages

    fine_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)

    storage_module.vnir_msg.subscribeTo(vnir_module.vnir_msg)
    storage_module.fine_msg.subscribeTo(fine_module.fine_msg)
    storage_module.imu_msg.subscribeTo(get_imu_msg(5,15,20,45))
    storage_module.gps_msg.subscribeTo(msg_module.balloon_msg)


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

















