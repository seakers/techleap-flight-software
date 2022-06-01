//
// Created by gabe :)
//

#include "ExternalModules/AttitudeDetermination/AttitudeDetermination.h"
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



AttitudeDetermination::AttitudeDetermination() // --> CHANGE
{
    this->state = 0;
}

AttitudeDetermination::~AttitudeDetermination() // --> CHANGE
{
    return;
}

void AttitudeDetermination::ZeroOutputVariables(){
    this->yaw = 0.0;
    this->pitch = 0.0;
    this->roll = 0.0;
}

void AttitudeDetermination::ReadMessages(){

    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    if(this->imu_msg.isLinked()){
        InertialMeasurementUnitOutMsgPayload imu_msg_payload = this->imu_msg();
        this->imu_state = imu_msg_payload.state;
        this->imu_yaw = imu_msg_payload.yaw;
        this->imu_pitch = imu_msg_payload.pitch;
        this->imu_roll = imu_msg_payload.roll;
    }

    if(this->gps_msg.isLinked()){
        GpsOutMsgPayload gps_msg_payload = this->gps_msg();
        this->gps_state = gps_msg_payload.state;
        this->gps_lat = gps_msg_payload.lat;
        this->gps_lon = gps_msg_payload.lon;
        this->gps_altitude = gps_msg_payload.altitude;
    }

    if(this->geo_msg.isLinked()){
        GeoTrackingMsgPayload geo_msg_payload = this->geo_msg();
        this->geo_state = geo_msg_payload.state;
        this->geo_lat = geo_msg_payload.lat;
        this->geo_lon = geo_msg_payload.lon;
    }
}

void AttitudeDetermination::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "AttitudeDetermination ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
}

void AttitudeDetermination::UpdateState(uint64_t CurrentSimNanos)
{
    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    AttitudeDeterminationAnglesMsgPayload ad_msg_buffer = this->ad_msg.zeroMsgPayload;
    this->ZeroOutputVariables();

    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    ad_msg_buffer.state = this->state;
    ad_msg_buffer.yaw = this->yaw;
    ad_msg_buffer.pitch = this->pitch;
    ad_msg_buffer.roll = this->roll;
    this->ad_msg.write(&ad_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "AttitudeDetermination ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
