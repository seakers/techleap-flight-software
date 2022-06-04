//
// Created by gabe :)
//

#include "ExternalModules/InertialMeasurementUnit/InertialMeasurementUnit.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"


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

void InertialMeasurementUnit::Reset(uint64_t CurrentSimNanos){
    bskLogger.bskLog(BSK_INFORMATION, "InertialMeasurementUnit ------ (reset)");

    // --> 1. Get sensor
//    const std::string SensorPort = "/dev/ttyUSB0";
//    const uint32_t SensorBaudrate = 115200;
//    vn::sensors::VnSensor vs;
//    vs.connect(SensorPort, SensorBaudrate);
//    vs.writeAsyncDataOutputFrequency(2);

    // --> 2. Reset module state
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

//    const std::string SensorPort = "/dev/ttyUSB0";
//    const uint32_t SensorBaudrate = 115200;
//    vn::sensors::VnSensor vs;
//    vs.connect(SensorPort, SensorBaudrate);
//
//    // --> Read yaw pitch roll
//    vn::math::vec3f ypr = vs.readYawPitchRoll();
//
//    // --> Read temperature
//    vn::sensors::ImuMeasurementsRegister reg = vs.readImuMeasurements();

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
