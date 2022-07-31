//
// Created by gabe :)
//

#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ControllerOutMsgPayload.h"
#include "msgPayloadDefC/ControllerManualMsgPayload.h"
#include "msgPayloadDefC/MessageConsumerManualMsgPayload.h"
#include "msgPayloadDefC/MessageConsumerMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"



/*! @brief basic Basilisk C++ module class */
class Controller: public SysModel { // --> CHANGE
public:
    Controller();  // --> CHANGE
    ~Controller(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();

public:

    // ---------------------
    // ----- FSW MODEs -----
    // ---------------------
    // 0 - Standby
    // 1 - TrackPlume
    // 2 - Scan
    // 3 - ManualPoint
    int mode;


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;

    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    int fine_state;
    Eigen::MatrixXd fine_mask;

    ReadFunctor<MessageConsumerManualMsgPayload> manual_msg;
    int manual_plume;
    double manual_lat;
    double manual_lon;
    double manual_alt;

    ReadFunctor<MessageConsumerMsgPayload> ins_msg;
    int ins_state;
    double lat;
    double lon;
    double alt;
    double yaw;
    double pitch;
    double roll;

    // --> MESSAGE OUT
    Message<ControllerOutMsgPayload>          controller_msg;
    Message<ControllerModeMsgPayload>         controller_mode_msg;
    Message<ControllerManualMsgPayload> controller_manual_msg;

    // --> OUTPUT
    int   msg;       // controller_msg
    double pan;       // controller_mode_msg
    double tilt;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
