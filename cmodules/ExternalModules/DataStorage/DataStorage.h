//
// Created by gabe :)
//

#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"
#include "msgPayloadDefC/MessageConsumerMsgPayload.h"
#include "msgPayloadDefC/GeoTrackingMsgPayload.h"
#include "msgPayloadDefC/IMUOutMsgPayload.h"

#include <Eigen/Dense>

/*! @brief basic Basilisk C++ module class */
class DataStorage: public SysModel { // --> CHANGE
public:
    DataStorage();
    ~DataStorage();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ReadMessages();

public:

    int state;

    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<ImagerVNIROutMsgPayload> vnir_msg;
    Eigen::MatrixXd red;
    Eigen::MatrixXd green;
    Eigen::MatrixXd blue;
    Eigen::MatrixXd nir;
    int vnir_state;

    ReadFunctor<IMUOutMsgPayload> imu_msg;
    int   imu_state;
    double imu_yaw;
    double imu_pitch;
    double imu_roll;
    double imu_temp;

    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;
    Eigen::MatrixXd b1;
    int thermal_state;

    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    Eigen::MatrixXd fine_mask;
    int fine_state;

    ReadFunctor<GeoTrackingMsgPayload> geo_msg;
    double geo_lat;
    double geo_lon;
    int geo_state;

    ReadFunctor<MessageConsumerMsgPayload> gps_msg;
    double gps_lat;
    double gps_lon;
    double gps_alt;
    int gps_state;


    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
