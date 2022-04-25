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
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"




/*! @brief basic Basilisk C++ module class */
class ImagerVNIR: public SysModel { // --> CHANGE
public:
    ImagerVNIR();  // --> CHANGE
    ~ImagerVNIR(); // --> CHANGE

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
    ReadFunctor<ImagerVNIROutMsgPayload> mock_msg;

    // --> MESSAGE OUT
    Message<ImagerVNIROutMsgPayload> vnir_msg;

    // --> OUTPUT
    int image_tensor[20][20];

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
