//
// Created by gabe :)
//

#include "ExternalModules/Controller/Controller.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



Controller::Controller() // --> CHANGE
{
    // --> Always start FSW on mode 0
    this->mode = 0;
}

Controller::~Controller() // --> CHANGE
{
    return;
}



void Controller::ZeroOutputVariables(){
    // --> Not resetting mode
    this->yaw = 0.0;
    this->pitch = 0.0;
    this->roll = 0.0;
}

void Controller::ReadMessages(){
    if(this->consumer_msg.isLinked()){
        MessageConsumerMsgPayload consumer_msg_payload = this->consumer_msg();
        this->consumer_state = consumer_msg_payload.state;
        this->consumer_msg_content = consumer_msg_payload.msg;
    }
}



void Controller::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "Controller ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void Controller::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    ControllerModeMsgPayload         controller_mode_msg_buffer         = this->controller_mode_msg.zeroMsgPayload;
    ControllerManualAnglesMsgPayload controller_manual_angle_msg_buffer = this->controller_manual_angle_msg.zeroMsgPayload;

    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO:


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------

    controller_mode_msg_buffer.mode = this->mode;
    this->controller_mode_msg.write(&controller_mode_msg_buffer, this->moduleID, CurrentSimNanos);

    controller_manual_angle_msg_buffer.yaw = this->yaw;
    controller_manual_angle_msg_buffer.pitch = this->pitch;
    controller_manual_angle_msg_buffer.pitch = this->roll;
    controller_manual_angle_msg_buffer.state = this->state;
    this->controller_manual_angle_msg.write(&controller_manual_angle_msg_buffer, this->moduleID, CurrentSimNanos);






    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "Controller ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
