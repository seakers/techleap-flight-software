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
#include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"



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
    // 1 - ManualPoint
    // 2 - Scan
    // 3 - TrackPlume
    int mode;


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;


    // --> MESSAGE OUT
    Message<ControllerOutMsgPayload>          controller_msg;
    Message<ControllerModeMsgPayload>         controller_mode_msg;
    Message<ControllerManualAnglesMsgPayload> controller_manual_angle_msg;

    // --> OUTPUT
    int   msg;       // controller_msg
    double pan;       // controller_mode_msg
    double tilt;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
