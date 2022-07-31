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
#include "msgPayloadDefC/MessageConsumerManualMsgPayload.h"
#include <zmq.hpp>


/*! @brief basic Basilisk C++ module class */
class MessageConsumer: public SysModel { // --> CHANGE
public:
    MessageConsumer();  // --> CHANGE
    ~MessageConsumer(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);
    void FCUinsSubscriber();
    void ZeroOutputVariables();
    void FCUpayloadSend(std::string payloadStr);
    std::string FCUpayloadReceiverListener();
    bool ConnectToPayloadServer(int pPayload);
    std::string s_recv (zmq::socket_t& socket, int flags);

public:


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    Message<MessageConsumerMsgPayload> balloon_msg;
    int ins_state;
    double lat;
    double lon;
    double alt;
    double yaw;
    double pitch;
    double roll;

    Message<MessageConsumerManualMsgPayload> manual_msg;
    int manual_plume;
    double manual_lat;
    double manual_lon;
    double manual_alt;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
