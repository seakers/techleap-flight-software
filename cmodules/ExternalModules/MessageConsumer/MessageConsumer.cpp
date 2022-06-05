//
// Created by gabe :)
//

#include "ExternalModules/MessageConsumer/MessageConsumer.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



MessageConsumer::MessageConsumer(){
    this->fcu_ip = "10.1.3.7";
    this->fcu_port = "5561";
    this->fcu_interface = "eth0";
}

MessageConsumer::~MessageConsumer(){
    return;
}



void MessageConsumer::ZeroOutputVariables(){
    this->msg = 0;
    return;
}

void MessageConsumer::OpenClientPairSocket(){
    std::string source_endpoint = this->fcu_ip + ":" + this->fcu_port;
    std::string dest_endpoint = this->fcu_interface + ":" + this->fcu_port;


    return;
}




void MessageConsumer::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void MessageConsumer::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    MessageConsumerMsgPayload consumer_msg_buffer = this->consumer_msg.zeroMsgPayload;

    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO: read balloon messaging system here

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: determine which module to send the message to



    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: create outputs for each module
    consumer_msg_buffer.state = this->state;
    consumer_msg_buffer.msg = this->msg;
    this->consumer_msg.write(&consumer_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
