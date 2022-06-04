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
#include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"
#include "msgPayloadDefC/MessageConsumerMsgPayload.h"




/*! @brief basic Basilisk C++ module class */
class Controller: public SysModel { // --> CHANGE
public:
    Controller();  // --> CHANGE
    ~Controller(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void ReadMessages();

public:
    // --------------------
    // ----- MESSAGES -----
    // --------------------
    ReadFunctor<MessageConsumerMsgPayload> consumer_msg;
    int consumer_state;
    int consumer_msg_content;


    // ---------------------
    // ----- FSW MODEs -----
    // ---------------------
    // 0 - Standby
    // 1 - ManualPoint
    // 2 - Scan
    // 3 - TrackPlume
    Message<ControllerModeMsgPayload> controller_mode_msg;
    int mode;

    Message<ControllerManualAnglesMsgPayload> controller_manual_angle_msg;
    int state;
    double yaw;
    double pitch;
    double roll;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
