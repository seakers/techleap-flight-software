//
// Created by gabe :)
//

#include "ExternalModules/GimbalControl/GimbalControl.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



GimbalControl::GimbalControl()
{
    this->state = 0;
}

GimbalControl::~GimbalControl()
{
    return;
}

void GimbalControl::ReadMessages(){
    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    if(this->adcs_angles_msg.isLinked()){
        AttitudeDeterminationAnglesMsgPayload adcs_angles_msg_payload = this->adcs_angles_msg();
        this->adcs_state = adcs_angles_msg_payload.state;
        this->adcs_yaw = adcs_angles_msg_payload.yaw;
        this->adcs_pitch = adcs_angles_msg_payload.pitch;
        this->adcs_roll = adcs_angles_msg_payload.roll;
    }

    if(this->cont_angles_msg.isLinked()){
        ControllerManualAnglesMsgPayload cont_angles_msg_payload = this->cont_angles_msg();
        this->cont_state = cont_angles_msg_payload.state;
        this->cont_yaw = cont_angles_msg_payload.yaw;
        this->cont_pitch = cont_angles_msg_payload.pitch;
        this->cont_roll = cont_angles_msg_payload.roll;
    }
}




void GimbalControl::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void GimbalControl::UpdateState(uint64_t CurrentSimNanos)
{
    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------


    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
