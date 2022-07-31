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
#include "ExternalModules/MessageConsumer/payloadmessage.pb.h"

zmq::context_t gContext(1);
zmq::socket_t gZMQpayloadSocket(gContext, ZMQ_PAIR);
int32_t gHWM = 0;
int32_t gLingerMilliseconds = 0;
int32_t gTimeoutMilliseconds = 1000;
int Payload1ServerPort = 5561;

MessageConsumer::MessageConsumer() // --> CHANGE
{

}

MessageConsumer::~MessageConsumer() // --> CHANGE
{
    return;
}

bool MessageConsumer::ConnectToPayloadServer(int pPayload)
{
    int payloadPortNumber = Payload1ServerPort;
    std::stringstream url;
    // Connect as payload
    switch (pPayload)
    {
        case 1:
        {
            payloadPortNumber = Payload1ServerPort;
            break;
        }
        default:
        {
            payloadPortNumber = Payload1ServerPort;
        }
    }

    url << "tcp://" << "10.42.0.154" << ":" << payloadPortNumber;
    std::cout << "Connecting payload to FCU on " << url.str() << std::endl;

    try {
        gZMQpayloadSocket.setsockopt(ZMQ_SNDHWM, &gHWM, sizeof(gHWM));
        gZMQpayloadSocket.setsockopt(ZMQ_RCVHWM, &gHWM, sizeof(gHWM));
        gZMQpayloadSocket.setsockopt(ZMQ_LINGER, &gLingerMilliseconds, sizeof(gLingerMilliseconds));
        gZMQpayloadSocket.setsockopt(ZMQ_RCVTIMEO, &gTimeoutMilliseconds, sizeof(gTimeoutMilliseconds));
        gZMQpayloadSocket.connect(url.str());
    } catch (zmq::error_t & e) {
        std::cerr << "Cannot open ZMQ payload socket: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "General error zmq payload client" << std::endl;
        return false;
    }

    return true;
}

void MessageConsumer::ZeroOutputVariables(){
    return;
}

void MessageConsumer::FCUinsSubscriber(){
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);

    std::stringstream url;
    url << "tcp://" << "10.42.0.154" << ":" << "5555";
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
    subscriber.setsockopt(ZMQ_LINGER, &gLingerMilliseconds, sizeof(gLingerMilliseconds));
    subscriber.setsockopt(ZMQ_RCVTIMEO, &gTimeoutMilliseconds, sizeof(gTimeoutMilliseconds));

    raven::fcu::swicd::RavenFCU_SWICD_INSmessage swicdINSmsg;


    std::string address = MessageConsumer::s_recv(subscriber, 0);
    std::string contents = MessageConsumer::s_recv(subscriber, 0);
    swicdINSmsg.ParseFromString(contents);
    printf("INS Message [addr=%s]: ts=%f, lat=%f, lon=%f, alt=%f, roll=%f, pitch=%f, yaw=%f\n",
        address.c_str(),
        swicdINSmsg.timestamp(),
        swicdINSmsg.latitude(),
        swicdINSmsg.longitude(),
        swicdINSmsg.altitude(),
        swicdINSmsg.roll(),
        swicdINSmsg.pitch(),
        swicdINSmsg.yaw()
    );
    this->ins_state = 0;
    this->lat = (double)swicdINSmsg.latitude();
    this->lon = (double)swicdINSmsg.longitude();
    this->alt = (double)swicdINSmsg.altitude();
    this->roll = (double)swicdINSmsg.roll();
    this->pitch = (double)swicdINSmsg.pitch();
    this->yaw = (double)swicdINSmsg.yaw();
    subscriber.close();
}

void MessageConsumer::FCUpayloadSend(std::string payloadStr)
{
    //
    // Send a message to the FCU
    //
    if (payloadStr.size() > 200) {
        payloadStr.resize(200);
    }
    std::cout << std::endl << "Payload message [" << payloadStr << "] size " << payloadStr.size() << std::endl;

    //
    // Google protocol buffer wire format section

    // Send the FCU a payload message to relay to the ground
    raven::fcu::swicd::RavenFCU_SWICD_PayloadMessage swicdPayloadMsg;

    // We don't need to set the message sent and received timestamps, The
    // flight controller will do that for us as it sends it down
    //swicdPayloadMsg.set_messagesentts();
    //swicdPayloadMsg.set_messagereceivedts();

    // We primarily want to send a generic 200 byte max message to our ground asset
    swicdPayloadMsg.set_data(payloadStr);

    std::string sendMsgPBstr;
    swicdPayloadMsg.SerializeToString(&sendMsgPBstr);

    //
    // ZMQ communication section
    zmq::message_t sendMsg(sendMsgPBstr.size());
    memcpy(sendMsg.data(), sendMsgPBstr.data(), sendMsgPBstr.size());

    if (!gZMQpayloadSocket.send(sendMsg)) {
        fprintf(stderr, "Failed to send a %lu byte message to the FCU\n", payloadStr.size());
    } else {
        printf("Payload sent a %lu byte message to the FCU\n", payloadStr.size());
    }
}

std::string MessageConsumer::FCUpayloadReceiverListener()
{
    std::string payloadStr;
    std::cout << "FCU Payload Receiver Testing starting" << std::endl;

    raven::fcu::swicd::RavenFCU_SWICD_PayloadMessage swicdPayloadMsg;

    //
    // Receive a message from the FCU
    zmq::message_t recvMsg;
    try
    {
        if (gZMQpayloadSocket.recv(&recvMsg))
        {
            // We got something from the flight controller
            std::cout << "We received a message from the flight controller" << std::endl;
            std::string recvMsgPBstr(static_cast<char*>(recvMsg.data()), recvMsg.size());
            swicdPayloadMsg.ParseFromString(recvMsgPBstr);

            const std::string & data = swicdPayloadMsg.data();
            printf("Payload message: sent ts=%f, recv ts=%f, data size=%ld\n",
                    swicdPayloadMsg.messagesentts(), swicdPayloadMsg.messagereceivedts(),
                    data.size());

            printf("Payload buffer [");
            for (auto c : data)
            {
                printf("%d,", (int)c);
            }
            printf("]\n");

        } else {
            std::cerr << "ZMQ payload receiver timed out" << std::endl;
            std::cerr << ".";
        }
    }
    catch(zmq::error_t & e)
    {
        std::cerr << "Cannot receive from ZMQ socket: " << e.what() << std::endl;
    }

    std::cout << "FCU Payload Receiver Testing finished" << std::endl;
    return payloadStr;
}

void MessageConsumer::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ (reset)");
    if (!this->ConnectToPayloadServer(1)) {
        fprintf(stderr, "Failed to connect to payload server\n");
    }
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

    this->FCUinsSubscriber();
    std::string payloadMsg = this->FCUpayloadReceiverListener();

    std::string delimiter = " ";
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = payloadMsg.find (delimiter, pos_start)) != std::string::npos) {
        token = payloadMsg.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (payloadMsg.substr (pos_start));
    if(res.at(0) == "LAT") {
        this->manual_plume = 1;
        this->manual_lat = std::stod(res.at(1));
        this->manual_lon = std::stod(res.at(3));
        this->manual_alt = std::stod(res.at(5));
    } else {
        this->manual_plume = 0;
    }
    

    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: create outputs for each module
    MessageConsumerMsgPayload balloon_msg_buffer = this->balloon_msg.zeroMsgPayload;
    balloon_msg_buffer.ins_state = this->ins_state;
    balloon_msg_buffer.lat = this->lat;
    balloon_msg_buffer.lon = this->lon;
    balloon_msg_buffer.alt = this->alt;
    balloon_msg_buffer.yaw = this->yaw;
    balloon_msg_buffer.pitch = this->pitch;
    balloon_msg_buffer.roll = this->roll;
    this->balloon_msg.write(&balloon_msg_buffer, this->moduleID, CurrentSimNanos);

    MessageConsumerManualMsgPayload manual_msg_buffer = this->manual_msg.zeroMsgPayload;
    manual_msg_buffer.manual_lat = this->manual_lat;
    manual_msg_buffer.manual_lon = this->manual_lon;
    manual_msg_buffer.manual_alt = this->manual_alt;

    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

std::string MessageConsumer::s_recv (zmq::socket_t& socket, int flags = 0) {

    zmq::message_t message;
    socket.recv(&message, 0);

    return std::string(static_cast<char*>(message.data()), message.size());
}