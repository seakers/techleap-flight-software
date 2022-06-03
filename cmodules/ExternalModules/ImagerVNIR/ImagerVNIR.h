//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_H
#define IMAGER_VNIR_H


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

#include <Eigen/Dense>


// Include files for using StApi.
//#include <StApi_TL.h>
//#include <StApi_IP.h>

//#include "vn/StApi_TL.h"
//#include "vn/StApi_IP.h"


// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"




/*! @brief basic Basilisk C++ module class */
class ImagerVNIR: public SysModel {
public:
    ImagerVNIR();
    ~ImagerVNIR();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();

public:

    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> mock_msg;

    // --> MESSAGE OUT
    Message<ImagerVNIROutMsgPayload> vnir_msg;
    int state;
    Eigen::MatrixXd red;
    Eigen::MatrixXd green;
    Eigen::MatrixXd blue;
    Eigen::MatrixXd nir;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
