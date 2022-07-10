//
// Created by gabe :)
//

#include "ExternalModules/MessageConsumer/MessageConsumer.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"
#include <zmq.hpp>
#include "ExternalModules/MessageConsumer/insmessage.pb.h"


MessageConsumer::MessageConsumer() // --> CHANGE
{

}

MessageConsumer::~MessageConsumer() // --> CHANGE
{
    return;
}



void MessageConsumer::ZeroOutputVariables(){
    return;
}

void MessageConsumer::FCUinsSubscriber(){
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);

    std::stringstream url;
    url << "tcp://" << "127.0.0.1" << ":" << "5555";
    std::cout << "Subscribing to FCU message on " << url.str() << std::endl;

    try {
        subscriber.connect(url.str());
    } catch(zmq::error_t & e) {
        std::cerr << "Cannot open ZMQ socket: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "General error zmq subscriber" << std::endl;
    }

    std::string insFilter("ins");
    std::cout << "Setting subscription filter for "<< insFilter << " messages" << std::endl;
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "ins", 3);

    raven::fcu::swicd::RavenFCU_SWICD_INSmessage swicdINSmsg;

    while(true) {
        std::string address = MessageConsumer::s_recv(subscriber, 0);
        std::string contents = MessageConsumer::s_recv(subscriber, 0);
        swicdINSmsg.ParseFromString(contents);
        printf("INS Message [addr=%s]: ts=%f",
            address.c_str(),
            swicdINSmsg.timestamp()
        );
    }
    subscriber.close();
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
    MessageConsumerMsgPayload balloon_msg_buffer = this->balloon_msg.zeroMsgPayload;
    balloon_msg_buffer.state = this->state;
    balloon_msg_buffer.lat = this->lat;
    balloon_msg_buffer.lon = this->lon;
    balloon_msg_buffer.alt = this->alt;
    this->balloon_msg.write(&balloon_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

std::string MessageConsumer::s_recv (zmq::socket_t& socket, int flags = 0) {

    zmq::message_t message;
    socket.recv(&message, flags);

    return std::string(static_cast<char*>(message.data()), message.size());
}