//
// Created by gabe :)
//

#include "ExternalModules/InertialMeasurementUnit/InertialMeasurementUnit.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include <string>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

<<<<<<< HEAD
=======
#include "vn/sensors.h"

using namespace vn::math;
using namespace vn::sensors;

>>>>>>> origin

InertialMeasurementUnit::InertialMeasurementUnit() // --> CHANGE
{

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


void InertialMeasurementUnit::ReadMessages(){
    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }
}

void InertialMeasurementUnit::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    bskLogger.bskLog(BSK_INFORMATION, "AttitudeDetermination ------ (reset)");
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
    InertialMeasurementUnitOutMsgPayload imu_msg_buffer = this->imu_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

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

//    this->yaw = ypr.x;
//    this->pitch = ypr.y;
//    this->roll = ypr.z;
//    this->temperature = reg.temp;

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
