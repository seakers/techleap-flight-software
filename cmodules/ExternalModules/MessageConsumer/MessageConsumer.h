//
// Created by gabe :)
//

#ifndef MESSAGE_CONSUMER_H
#define MESSAGE_CONSUMER_H

#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/MessageConsumerMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class MessageConsumer: public SysModel { // --> CHANGE
public:
    MessageConsumer();  // --> CHANGE
    ~MessageConsumer(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();

public:


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
