import pytest
import sys
import netCDF4
import numpy as np
import os
import cv2
import pickle
# sys.path.insert(1, '/home/gabe/repos/techleap/techleap-flight-software')
sys.path.insert(1, '/app')


# --> Simulation Import
from simulation.api import SimulationClient

# --> Module Import
from Basilisk.ExternalModules import FineNN

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
    'image_file, label_file',
    [
        ('/app/images/VNIR/image_1.nc', '/app/images/VNIR/label_1.nc'),
    ]
)
def test_function(image_file, label_file):
    print('Running test...')

    # --> 1. Get test data image and label
    vnir_image, thermal_image, label = get_test_data(image_file, label_file)

    # --> 1. Run test function
    result = run(vnir_image, thermal_image)
    # result = True



    # --> 2. Assert result
    assert result is True

    # --> 3. Set options
    __tracebackhide__ = True



def get_test_data(image_file, label_file):
    image_ds = netCDF4.Dataset(image_file)  # 3248 x 3200 | 3232 x 3200
    label_ds = netCDF4.Dataset(label_file)  # 406  x 400  | 404  x 400

    image_processed = process_image(image_ds)
    label_processed = process_label(label_ds)

    split = np.split(image_processed, [3], axis=0)
    vnir_image = split[0]
    thermal_image = split[1]

    print(vnir_image.shape)       # (3, 3200, 3200)
    print(thermal_image.shape)    # (4, 3200, 3200)
    print(label_processed.shape)  # (3200, 3200)

    return vnir_image, thermal_image, label_processed

def process_image(image_ds, remove_outliers=True):
    bands = image_ds['observation_data'].variables
    img_data = None
    specific_bands = ['M01', 'M03', 'M05', 'M07', 'M09', 'M11', 'M15']  # or band == "M11" or band == "M15":  # band == "M03":
    for band in bands:
        # bands --> M01: 0.402-0.422 | M03: 0.478-0.488 | M05: 0.662-0.682 | M07: 0.846-0.885 | M09: 1.371-1.386
        if band in specific_bands:
            band_data = bands[band]  # 3248 x 3200 | 3232 x 3200
            band_data = band_data[:3200, :] # 3200 x 3200

            # upscale image
            # band_data = cv2.resize(band_data, dsize=(9600, 9600), interpolation=cv2.INTER_LINEAR)
            if img_data is None:
                img_data = np.array(band_data)
                img_data = img_data[None]
            else:
                img_data = np.concatenate((img_data, [np.array(band_data)]))

    # --> 2. Apply mask to 3D array
    if remove_outliers is True:
        img_data = np.ma.masked_greater(img_data, 6.55e4)  # mask outliers
        img_data = np.ma.filled(img_data, np.nan)
    return img_data

def process_label(label_ds):

    # --> 1. Get labels in 3D array and reshape to 2D: 406 x 400 x 4 --> 406 x 400
    labels = label_ds['geophysical_data'].variables['Corrected_Optical_Depth_Land']
    labels = np.array(labels)
    labels = labels[:400, :, 1]

    # --> 2. Get quality in 2D array: 406 x 400
    # - (must be 3 for land observations. labels gives aerosol optical depth at X um)
    quality = label_ds['geophysical_data'].variables['Land_Ocean_Quality_Flag']
    quality = np.array(quality)
    quality = quality[:400, :]
    for i in range(np.size(quality, 0)):  # 400
        for j in range(np.size(quality, 1)):  # 400
            if labels[i][j] > 0.3:
                labels[i][j] = 1.0
            else:
                labels[i][j] = 0.0

    # --> 3. Reshape labels through interpolation: 400 x 400 --> 3200 x 3200
    labels = cv2.resize(labels, dsize=(3200, 3200), interpolation=cv2.INTER_NEAREST_EXACT)
    return labels



# """
#    _____  _                    _         _    _
#   / ____|(_)                  | |       | |  (_)
#  | (___   _  _ __ ___   _   _ | |  __ _ | |_  _   ___   _ __
#   \___ \ | || '_ ` _ \ | | | || | / _` || __|| | / _ \ | '_ \
#   ____) || || | | | | || |_| || || (_| || |_ | || (_) || | | |
#  |_____/ |_||_| |_| |_| \__,_||_| \__,_| \__||_| \___/ |_| |_|
#
# """

def get_vinr_message(vnir_image):
    vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    vnir_msg_data.state = 0

    print('--> FILLING VNIR MESSAGE')
    # vnir_msg_data.red = np.zeros([3200, 3200], dtype=int).tolist()
    # vnir_msg_data.green = np.zeros([3200, 3200], dtype=int).tolist()
    # vnir_msg_data.blue = np.zeros([3200, 3200], dtype=int).tolist()
    print(len(vnir_msg_data.red))
    vnir_msg_data.red = np.array(np.zeros([3200, 3200], dtype=int).tolist())

    # for x in range(3200):
    #     for y in range(3200):
    #         vnir_msg_data.red[x][y] = vnir_image[0][x][y]
    #         vnir_msg_data.green[x][y] = vnir_image[1][x][y]
    #         vnir_msg_data.blue[x][y] = vnir_image[2][x][y]
    print('--> FINISHED VNIR MESSAGE')
    exit(0)
    return messaging.ImagerVNIROutMsg().write(vnir_msg_data)


def get_thermal_message(thermal_image):
    thermal_msg_data = messaging.ImagerThermalOutMsgPayload()
    thermal_msg_data.state = 0

    print('--> FILLING THERMAL MESSAGE')
    thermal_msg_data.b1 = np.zeros([3200, 3200], dtype=double).tolist()
    thermal_msg_data.b2 = np.zeros([3200, 3200], dtype=double).tolist()
    thermal_msg_data.b3 = np.zeros([3200, 3200], dtype=double).tolist()
    thermal_msg_data.b4 = np.zeros([3200, 3200], dtype=double).tolist()
    for x in range(3200):
        for y in range(3200):
            thermal_msg_data.b1[x][y] = thermal_image[0][x][y]
            thermal_msg_data.b2[x][y] = thermal_image[1][x][y]
            thermal_msg_data.b3[x][y] = thermal_image[2][x][y]
            thermal_msg_data.b4[x][y] = thermal_image[3][x][y]
    return messaging.ImagerThermalOutMsg().write(thermal_msg_data)



def run(vnir_image, thermal_image):

    # --> 1. Create simulation client
    sim_client = SimulationClient(time_step=1.0, duration=2.0)

    # --> 2. Create module
    test_module = FineNN.FineNN()
    test_module.ModelTag = "FineNN"
    sim_client.new_c_module(test_module)

    # --> 3. Create mock messages
    # vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    # vnir_msg_data.state = 20
    # vnir_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    # vnir_msg = messaging.ImagerVNIROutMsg().write(vnir_msg_data)
    vnir_msg = get_vinr_message(vnir_image)

    # thermal_msg_data = messaging.ImagerThermalOutMsgPayload()
    # thermal_msg_data.state = 20
    # thermal_msg_data.imageTensor = np.zeros([20, 20], dtype=int).tolist()
    # thermal_msg = messaging.ImagerThermalOutMsg().write(thermal_msg_data)
    thermal_msg = get_thermal_message(thermal_image)

    coarse_msg_data = messaging.CoarsePredictionMsgPayload()
    coarse_msg_data.prediction = 1
    coarse_msg_data.state = 1
    coarse_msg = messaging.CoarsePredictionMsg().write(coarse_msg_data)

    # --> 4. Subscribe to messages
    test_module.vnir_msg.subscribeTo(vnir_msg)
    test_module.thermal_msg.subscribeTo(thermal_msg)
    test_module.coarse_msg.subscribeTo(coarse_msg)

    # --> 5. Set output message recording
    output_rec = test_module.fine_msg.recorder()
    sim_client.new_c_module(output_rec)

    # --> 6. Set variable recording
    var1 = "FineNN.state"
    sim_client.new_logging_var(var1)

    # --> 6. Run simulation
    sim_client.run()

    # --> 7. Get debug output
    var1 = sim_client.get_var_log_data(var1)

    print(output_rec.state)


    return True





def get_tensor(value, rows=20, cols=20):
    tensor = []
    for x in range(rows):
        row = []
        for y in range(cols):
            row.append(value)
        tensor.append(row)
    return tensor


















