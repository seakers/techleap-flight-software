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
    this->mode = 1;
}

Controller::~Controller() // --> CHANGE
{
    return;
}



void Controller::ZeroOutputVariables(){
}





void Controller::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "Controller ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
    this->mode = 1;
}



void Controller::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    ControllerOutMsgPayload          controller_msg_buffer              = this->controller_msg.zeroMsgPayload;
    ControllerModeMsgPayload         controller_mode_msg_buffer         = this->controller_mode_msg.zeroMsgPayload;
    ControllerManualMsgPayload controller_manual_msg_buffer = this->controller_manual_msg.zeroMsgPayload;

    // --> Zero internal output variables
    //this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO:
    if(this->fine_msg.isLinked()){
        std::cout << "Fine angles linked to controller!" << std::endl;
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        std::cout << "Fine state: " << this->fine_state << std::endl;
        this->pan = fine_msg_payload.pan;
        this->tilt = fine_msg_payload.tilt;
    }
    if(this->ins_msg.isLinked()){
        std::cout << "INS message linked to controller!" << std::endl;
        MessageConsumerMsgPayload ins_msg_payload = this->ins_msg();
        this->ins_state = ins_msg_payload.ins_state;
        std::cout << "INS State: " << this->ins_state << std::endl;
        this->lat = ins_msg_payload.lat;
        this->lon = ins_msg_payload.lon;
        this->alt = ins_msg_payload.alt;
        this->yaw = ins_msg_payload.yaw;
        this->pitch = ins_msg_payload.pitch;
        this->roll = ins_msg_payload.roll;
    }
    if(this->manual_msg.isLinked()){
        std::cout << "Payload message linked to controller!" << std::endl;
        MessageConsumerManualMsgPayload manual_msg_payload = this->manual_msg();
        this->manual_plume = manual_msg_payload.manual_plume;
        std::cout << "Manual plume: " << this->manual_plume << std::endl;
        this->manual_lat = manual_msg_payload.manual_lat;
        this->manual_lon = manual_msg_payload.manual_lon;
        this->manual_alt = manual_msg_payload.manual_alt;
    }

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO:
    if(this->ins_state == 0) {
        std::cout << "Idle mode" << std::endl;
        this->mode = 0;
    }
    else if(this->manual_plume == 1) {
        std::cout << "Manual plume mode" << std::endl;
        this->mode = 3;
    }
    else if(this->fine_state == 1) {
        std::cout << "Tracking mode" << std::endl;
        this->mode = 1;
    } else {
        std::cout << "Scanning mode" << std::endl;
        this->mode = 2;
    }

    
    // -------------------------
    // ----- Write Outputs -----
    // -------------------------

    controller_msg_buffer.state = this->state;
    controller_msg_buffer.msg = this->msg;
    this->controller_msg.write(&controller_msg_buffer, this->moduleID, CurrentSimNanos);

    controller_mode_msg_buffer.mode = this->mode;
    this->controller_mode_msg.write(&controller_mode_msg_buffer, this->moduleID, CurrentSimNanos);

    controller_manual_msg_buffer.manual_lat = this->manual_lat;
    controller_manual_msg_buffer.manual_lon = this->manual_lon;
    controller_manual_msg_buffer.manual_alt = this->manual_alt;
    this->controller_manual_msg.write(&controller_manual_msg_buffer, this->moduleID, CurrentSimNanos);






    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "Controller ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
