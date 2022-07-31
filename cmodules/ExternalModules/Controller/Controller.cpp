//
// Created by gabe :)
//

#include "ExternalModules/Controller/Controller.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

#include <unistd.h>
#include <sys/reboot.h>

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
}





void Controller::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "Controller ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
    this->mode = 0;
    this->plumeStartTime = 0.0;
    this->movedToPlume = false;
    this->resetAfterPlume = false;
    this->errorCount = 0;
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

    this->controller_status = "";
    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO:
    if(this->fine_msg.isLinked()){
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        this->pan = fine_msg_payload.pan;
        this->tilt = fine_msg_payload.tilt;
    }
    if(this->ins_msg.isLinked()){
        MessageConsumerMsgPayload ins_msg_payload = this->ins_msg();
        this->ins_state = ins_msg_payload.ins_state;
        this->lat = ins_msg_payload.lat;
        this->lon = ins_msg_payload.lon;
        this->alt = ins_msg_payload.alt;
        this->yaw = ins_msg_payload.yaw;
        this->pitch = ins_msg_payload.pitch;
        this->roll = ins_msg_payload.roll;
    }
    if(this->manual_msg.isLinked()){
        MessageConsumerManualMsgPayload manual_msg_payload = this->manual_msg();
        this->manual_plume = manual_msg_payload.manual_plume;
        this->manual_lat = manual_msg_payload.manual_lat;
        this->manual_lon = manual_msg_payload.manual_lon;
        this->manual_alt = manual_msg_payload.manual_alt;
    }
    if(this->vnir_msg.isLinked()){
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->vnir_status = vnir_msg_payload.status_msg;
    }
    if(this->imu_msg.isLinked()){
        IMUOutMsgPayload imu_msg_payload = this->imu_msg();
        this->imu_state = imu_msg_payload.state;
        this->imu_status = imu_msg_payload.status_msg;
    }

    if(this->imu_state == 1) {
        this->errorCount +=1;
        this->controller_status = this->controller_status + imu_status;
    }
    if(this->vnir_state == 1) {
        this->errorCount +=1;
        this->controller_status = this->controller_status + vnir_status;
    }

    if(this->errorCount > 100) {
        sync();
        reboot(RB_AUTOBOOT);
    }

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO:
    if(this->movedToPlume == true && (float) CurrentSimNanos/(1e9) > 30.0+this->plumeStartTime && this->resetAfterPlume == false) {
        std::cout << "Resetting to scanning mode" << std::endl;
        this->mode = 2;
        this->resetAfterPlume = true;
    }
    if(false) { // CHANGE BEFORE FLIGHT
        std::cout << "Idle mode" << std::endl;
        this->mode = 0;
    }
    else if(this->manual_plume == 1 && this->movedToPlume == false) {
        std::cout << "Manual plume mode" << std::endl;
        this->mode = 3;
        this->plumeStartTime = (float) CurrentSimNanos/(1e9);
        this->movedToPlume = true;
    }
    else if(this->fine_state == 1 && this->movedToPlume == false) {
        std::cout << "Tracking mode" << std::endl;
        this->mode = 1;
    } else if(this->movedToPlume == false) {
        std::cout << "Scanning mode" << std::endl;
        this->mode = 2;
    }
    std::cout << "Plume start time: " << this->plumeStartTime << std::endl;

    //this->mode = 2;
    
    // -------------------------
    // ----- Write Outputs -----
    // -------------------------

    controller_msg_buffer.state = this->state;
    controller_msg_buffer.msg = this->controller_status;
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
