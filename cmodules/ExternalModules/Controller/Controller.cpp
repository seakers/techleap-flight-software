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
    this->msg = 0;
    this->pan = 0.0;
    this->tilt = 0.0;
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
    ControllerManualAnglesMsgPayload controller_manual_angle_msg_buffer = this->controller_manual_angle_msg.zeroMsgPayload;

    // --> Zero internal output variables
    //this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO:

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO:


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------

    controller_msg_buffer.state = this->state;
    controller_msg_buffer.msg = this->msg;
    this->controller_msg.write(&controller_msg_buffer, this->moduleID, CurrentSimNanos);

    controller_mode_msg_buffer.mode = this->mode;
    this->controller_mode_msg.write(&controller_mode_msg_buffer, this->moduleID, CurrentSimNanos);

    controller_manual_angle_msg_buffer.pan = this->pan;
    controller_manual_angle_msg_buffer.tilt = this->tilt;
    this->controller_manual_angle_msg.write(&controller_manual_angle_msg_buffer, this->moduleID, CurrentSimNanos);






    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "Controller ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
