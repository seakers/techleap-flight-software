//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_H  // --------------------------------------------> CHANGE
#define IMAGER_VNIR_H  // --------------------------------------------> CHANGE
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"



/*! @brief basic Basilisk C++ module class */
class ImagerVNIR: public SysModel { // --> CHANGE
public:
    ImagerVNIR();  // --> CHANGE
    ~ImagerVNIR(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;
    int imageTensor[20][20];

    // --> MESSAGE OUT
    Message<ImagerVNIROutMsgPayload> dataOutMsg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
