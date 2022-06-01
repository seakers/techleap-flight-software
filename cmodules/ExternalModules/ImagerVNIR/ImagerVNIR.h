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
    double red[3200][3200];
    double green[3200][3200];
    double blue[3200][3200];

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
