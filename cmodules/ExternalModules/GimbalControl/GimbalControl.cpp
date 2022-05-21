//
// Created by gabe :)
//

#include "ExternalModules/GimbalControl/GimbalControl.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



GimbalControl::GimbalControl() // --> CHANGE
{

}

GimbalControl::~GimbalControl() // --> CHANGE
{
    return;
}






void GimbalControl::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
    this->adcs_state = 0;
    this->adcs_yaw = 0.0;
    this->adcs_pitch = 0.0;
    this->adcs_roll = 0.0;
    this->cont_state = 0;
    this->cont_yaw = 0.0;
    this->cont_pitch = 0.0;
    this->cont_roll = 0.0;
}



void GimbalControl::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------

    // --> ADCS Angles
    AttitudeDeterminationAnglesMsgPayload adcs_angles_msg_payload = this->adcs_angles_msg();
    this->adcs_state = adcs_angles_msg_payload.state;
    this->adcs_yaw = adcs_angles_msg_payload.yaw;
    this->adcs_pitch = adcs_angles_msg_payload.pitch;
    this->adcs_roll = adcs_angles_msg_payload.roll;

    // --> Controller Angles
    ControllerManualAnglesMsgPayload controller_angles_msg_payload = this->controller_angles_msg();
    this->cont_state = controller_angles_msg_payload.state;
    this->cont_yaw = controller_angles_msg_payload.yaw;
    this->cont_pitch = controller_angles_msg_payload.pitch;
    this->cont_roll = controller_angles_msg_payload.roll;

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------

    this->state += 1;

    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
