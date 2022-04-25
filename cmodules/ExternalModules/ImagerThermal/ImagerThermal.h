//
// Created by gabe :)
//

#ifndef IMAGER_THERMAL_H
#define IMAGER_THERMAL_H



#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"


// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"




/*! @brief basic Basilisk C++ module class */
class ImagerThermal: public SysModel { // --> CHANGE
public:
    ImagerThermal();  // --> CHANGE
    ~ImagerThermal(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);


    void InitializeTensors();
    void ZeroOutputVariables();

public:

    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;

    // --> MESSAGE IN
    ReadFunctor<ImagerThermalOutMsgPayload> mock_msg;

    // --> MESSAGE OUT
    Message<ImagerThermalOutMsgPayload> thermal_msg;

    // --> OUTPUT
    int image_tensor[20][20];

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
