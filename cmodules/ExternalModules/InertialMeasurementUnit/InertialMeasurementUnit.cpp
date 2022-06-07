//
// Created by gabe :)
//

#include "ExternalModules/InertialMeasurementUnit/InertialMeasurementUnit.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include <string>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

#include "vn/sensors.h"

using namespace vn::math;
using namespace vn::sensors;


InertialMeasurementUnit::InertialMeasurementUnit() // --> CHANGE
{
    this->state = 0;
}

InertialMeasurementUnit::~InertialMeasurementUnit() // --> CHANGE
{
    return;
}

void InertialMeasurementUnit::ZeroOutputVariables(){
    this->yaw = 0.0;
    this->pitch = 0.0;
    this->roll = 0.0;
    this->temperature = 0.0;
}



void InertialMeasurementUnit::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    bskLogger.bskLog(BSK_INFORMATION, "InertialMeasurementUnit ------ (reset)");
    const std::string SensorPort = "/dev/ttyUSB0";
    const uint32_t SensorBaudrate = 115200;
    VnSensor vs;
	vs.connect(SensorPort, SensorBaudrate);
    vs.writeAsyncDataOutputFrequency(2);
    // --> 1. Reset module state
    this->state = 0;
}



void InertialMeasurementUnit::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{
    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    IMUOutMsgPayload imu_msg_buffer = this->imu_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    VnSensor vs;
    const std::string SensorPort = "/dev/ttyUSB0";
    const uint32_t SensorBaudrate = 115200;
	vs.connect(SensorPort, SensorBaudrate);
    // --> Read yaw pitch roll
    vec3f ypr = vs.readYawPitchRoll();

    // --> Read temperature
    ImuMeasurementsRegister reg = vs.readImuMeasurements();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------

    this->yaw = ypr.x;
    this->pitch = ypr.y;
    this->roll = ypr.z;
    std::cout << "yaw" << this->yaw << std::endl;
    std::cout << "pitch" << this->pitch << std::endl;
    this->temperature = reg.temp;

    // -------------------------
    // ----- Write Outputs -----
    // -------------------------

    imu_msg_buffer.state = this->state;
    imu_msg_buffer.yaw = this->yaw;
    imu_msg_buffer.pitch = this->pitch;
    imu_msg_buffer.roll = this->roll;
    imu_msg_buffer.temperature = this->temperature;
    this->imu_msg.write(&imu_msg_buffer, this->moduleID, CurrentSimNanos);


}
