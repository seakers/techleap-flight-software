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
    void readInputMessages();

public:

    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> mock_msg;

    // --> MESSAGE OUT
    Message<ImagerVNIROutMsgPayload> vnir_msg;
    int state;
    double red[512][512];
    double green[512][512];
    double blue[512][512];
    double nir[512][512];
    string captureMode;
    

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
