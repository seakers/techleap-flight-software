//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_H
#define IMAGER_VNIR_H


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"


// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"

#include <Eigen/Dense>


/*! @brief basic Basilisk C++ module class */
class ImagerVNIR: public SysModel { // --> CHANGE
public:
    ImagerVNIR();  // --> CHANGE
    ~ImagerVNIR(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void readInputMessages();

public:

    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> MESSAGE IN
    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    // --> MESSAGE OUT
    Message<ImagerVNIROutMsgPayload> vnir_msg;
    int state;
    Eigen::MatrixXd red;
    Eigen::MatrixXd green;
    Eigen::MatrixXd blue;
    Eigen::MatrixXd nir;
    int captureMode;
    

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
