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
#include "msgPayloadDefC/GeoTrackingMsgPayload.h"

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

    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;
    Eigen::MatrixXd b1;
    Eigen::MatrixXd b2;
    Eigen::MatrixXd b3;
    Eigen::MatrixXd b4;
    int thermal_state;

    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    Eigen::MatrixXd fine_mask;
    int fine_state;

    ReadFunctor<GeoTrackingMsgPayload> geo_msg;
    double geo_lat;
    double geo_lon;
    int geo_state;



    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
