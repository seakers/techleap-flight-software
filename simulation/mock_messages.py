from Basilisk.architecture import messaging
import numpy as np


def get_vnir_msg():
    vnir_msg_data = messaging.ImagerVNIROutMsgPayload()
    vnir_msg_data.state = 0
    vnir_msg_data.red = np.zeros([512, 512], dtype=float).tolist()
    vnir_msg_data.green = np.zeros([512, 512], dtype=float).tolist()
    vnir_msg_data.blue = np.zeros([512, 512], dtype=float).tolist()
    vnir_msg_data.nir = np.zeros([512, 512], dtype=float).tolist()
    vnir_msg = messaging.ImagerVNIROutMsg().write(vnir_msg_data)
    return vnir_msg

def get_thermal_msg():
    thermal_msg_data = messaging.ImagerThermalOutMsgPayload()
    thermal_msg_data.state = 0
    thermal_msg_data.b1 = np.zeros([512, 512], dtype=float).tolist()
    thermal_msg_data.b2 = np.zeros([512, 512], dtype=float).tolist()
    thermal_msg_data.b3 = np.zeros([512, 512], dtype=float).tolist()
    thermal_msg_data.b4 = np.zeros([512, 512], dtype=float).tolist()
    thermal_msg = messaging.ImagerThermalOutMsg().write(thermal_msg_data)
    return thermal_msg

def get_fine_msg():
    fine_msg_data = messaging.FinePredictionMsgPayload()
    fine_msg_data.state = 0
    fine_msg_data.mask = np.zeros([512, 512], dtype=float).tolist()
    fine_msg = messaging.FinePredictionMsg().write(fine_msg_data)
    return fine_msg

def get_coarse_msg(prediction=0):
    coarse_msg_data = messaging.CoarsePredictionMsgPayload()
    coarse_msg_data.state = 0
    coarse_msg_data.prediction = prediction
    coarse_msg = messaging.CoarsePredictionMsg().write(coarse_msg_data)
    return coarse_msg

def get_geo_msg(lat=0.0, lon=0.0):
    geo_msg_data = messaging.GeoTrackingMsgPayload()
    geo_msg_data.state = 0
    geo_msg_data.lat = lat
    geo_msg_data.lon = lon
    geo_msg = messaging.GeoTrackingMsg().write(geo_msg_data)
    return geo_msg

def get_gps_msg(lat=0.0, lon=0.0, altitude=0.0):
    gps_msg_data = messaging.GpsOutMsgPayload()
    gps_msg_data.state = 0
    gps_msg_data.lat = lat
    gps_msg_data.lon = lon
    gps_msg_data.altitude = altitude
    gps_msg = messaging.GpsOutMsg().write(gps_msg_data)
    return gps_msg

def get_imu_msg(yaw=0.0, pitch=0.0, roll=0.0, temperature=0.0):
    imu_msg_data = messaging.InertialMeasurementUnitOutMsgPayload()
    imu_msg_data.state = 0
    imu_msg_data.yaw = yaw
    imu_msg_data.pitch = pitch
    imu_msg_data.roll = roll
    imu_msg_data.temperature = temperature
    imu_msg = messaging.InertialMeasurementUnitOutMsg().write(imu_msg_data)
    return imu_msg

def get_adcs_msg(yaw=0.0, pitch=0.0, roll=0.0):
    adcs_msg_data = messaging.AttitudeDeterminationAnglesMsgPayload()
    adcs_msg_data.state = 0
    adcs_msg_data.yaw = yaw
    adcs_msg_data.pitch = pitch
    adcs_msg_data.roll = roll
    adcs_msg = messaging.AttitudeDeterminationAnglesMsg().write(adcs_msg_data)
    return adcs_msg

def get_consumer_msg(msg=0):
    consumer_msg_data = messaging.MessageConsumerMsgPayload()
    consumer_msg_data.state = 0
    consumer_msg_data.msg = msg
    consumer_msg = messaging.MessageConsumerMsg().write(consumer_msg_data)
    return consumer_msg

def get_controller_mode_msg(mode=0):
    mode_msg_data = messaging.ControllerModeMsgPayload()
    mode_msg_data.mode = mode
    mode_msg = messaging.ControllerModeMsg().write(mode_msg_data)
    return mode_msg

def get_controller_angles_msg(yaw=0.0, pitch=0.0, roll=0.0):
    angles_msg_data = messaging.ControllerManualAnglesMsgPayload()
    angles_msg_data.state = 0
    angles_msg_data.pan = pan
    angles_msg_data.tilt = tilt
    angles_msg = messaging.ControllerManualAnglesMsg().write(angles_msg_data)
    return angles_msg