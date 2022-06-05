//
// Created by gabe :)
//

#ifndef ATTITUDE_DETERMINATION_H
#define ATTITUDE_DETERMINATION_H


#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/GpsOutMsgPayload.h"
#include "msgPayloadDefC/InertialMeasurementUnitOutMsgPayload.h"
#include "msgPayloadDefC/GeoTrackingMsgPayload.h"

#include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class AttitudeDetermination: public SysModel { // --> CHANGE
public:
    AttitudeDetermination();  // --> CHANGE
    ~AttitudeDetermination(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void ReadMessages();

public:
    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<InertialMeasurementUnitOutMsgPayload> imu_msg;
    int   imu_state;
    double imu_yaw;
    double imu_pitch;
    double imu_roll;

    ReadFunctor<GpsOutMsgPayload> gps_msg;
    int   gps_state;
    double gps_lat;
    double gps_lon;
    double gps_altitude;

    ReadFunctor<GeoTrackingMsgPayload> geo_msg;
    double geo_lat;
    double geo_lon;
    int geo_state;


    // -----------------------
    // ----- MESSAGE OUT -----
    // -----------------------

    Message<AttitudeDeterminationAnglesMsgPayload> ad_msg;
    int state;
    double yaw;
    double pitch;
    double roll;


    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
