//
// Created by gabe :)
//

#ifndef GIMBAL_CONTROL_H
#define GIMBAL_CONTROL_H

#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"
#include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class GimbalControl: public SysModel { // --> CHANGE
public:
    GimbalControl();  // --> CHANGE
    ~GimbalControl(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;

    // --> MESSAGE IN
    ReadFunctor<AttitudeDeterminationAnglesMsgPayload> adcs_angles_msg;
    ReadFunctor<ControllerManualAnglesMsgPayload>      controller_angles_msg;

    // --> INPUT
    int   adcs_state;
    float adcs_yaw;
    float adcs_pitch;
    float adcs_roll;
    int   cont_state;
    float cont_yaw;
    float cont_pitch;
    float cont_roll;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
