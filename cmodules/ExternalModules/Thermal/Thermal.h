//
// Created by gabe :) + ben :P
//

#ifndef THERMAL_H
#define THERMAL_H

#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ThermalMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class Thermal: public SysModel { // --> CHANGE
public:
    Thermal();
    ~Thermal();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void ReadMessages();

public:

    // -----------------------
    // ----- MESSAGE OUT -----
    // -----------------------

    Message<ThermalMsgPayload> thermal_msg;
    int state;
    int temps[4];


    // --> LOGGING
    BSKLogger bskLogger;
};




#endif
