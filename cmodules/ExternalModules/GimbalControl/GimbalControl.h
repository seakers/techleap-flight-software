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
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"
#include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class GimbalControl: public SysModel {
public:
    GimbalControl();
    ~GimbalControl();

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

    ReadFunctor<AttitudeDeterminationAnglesMsgPayload> adcs_angles_msg;
    int   adcs_state;
    double adcs_yaw;
    double adcs_pitch;
    double adcs_roll;

    ReadFunctor<ControllerManualAnglesMsgPayload> cont_angles_msg;
    int   cont_state;
    double cont_yaw;
    double cont_pitch;
    double cont_roll;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
