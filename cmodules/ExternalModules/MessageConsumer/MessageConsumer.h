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

#include <zmq.hpp>


/*! @brief basic Basilisk C++ module class */
class MessageConsumer: public SysModel { // --> CHANGE
public:
    MessageConsumer();  // --> CHANGE
    ~MessageConsumer(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void OpenClientPairSocket();

public:

    // --> ZeroMQ
    std::string fcu_port;
    std::string fcu_ip;
    std::string fcu_interface;



    // --------------------
    // ----- MESSAGES -----
    // --------------------
    Message<MessageConsumerMsgPayload> consumer_msg;
    int state;
    int msg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
