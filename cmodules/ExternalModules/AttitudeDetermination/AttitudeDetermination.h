//
// Created by gabe :)
//

#ifndef ATTITUDE_DETERMINATION_H
#define ATTITUDE_DETERMINATION_H


#include <torch/script.h>
#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/GpsOutMsgPayload.h"
#include "msgPayloadDefC/InertialMeasurementUnitOutMsgPayload.h"
#include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class AttitudeDetermination: public SysModel { // --> CHANGE
public:
    AttitudeDetermination();  // --> CHANGE
    ~AttitudeDetermination(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();

public:


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;

    // --> MESSAGE IN
    ReadFunctor<InertialMeasurementUnitOutMsgPayload> imu_msg;
    ReadFunctor<GpsOutMsgPayload> gps_msg;

    // --> INPUT
    int   gps_state;    // gps_msg
    float gps_lat;      // gps_msg
    float gps_lon;      // gps_msg
    float gps_altitude; // gps_msg

    int   imu_state;  // imu_msg
    float imu_yaw;    // imu_msg
    float imu_pitch;  // imu_msg
    float imu_roll;   // imu_msg

    // --> MESSAGE OUT
    Message<AttitudeDeterminationAnglesMsgPayload> ad_msg;

    // --> OUTPUT
    float yaw;
    float pitch;
    float roll;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
