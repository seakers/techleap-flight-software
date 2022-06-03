//
// Created by gabe :)
//

#include "ExternalModules/CoarseNN/CoarseNN.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



CoarseNN::CoarseNN() // --> CHANGE
{
    this->state = 0;
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
}

CoarseNN::~CoarseNN() // --> CHANGE
{
    return;
}


void CoarseNN::LoadModel(){
    // --> TODO: Load coarse model (look at roshan code)
    bskLogger.bskLog(BSK_INFORMATION, "CoarseNN ------ loaded nn model");
}

void CoarseNN::ZeroOutputVariables(){
    this->prediction = 0;
}

void CoarseNN::ReadMessages(){

    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    if(this->vnir_msg.isLinked()){
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->red = vnir_msg_payload.red;
        this->green = vnir_msg_payload.green;
        this->blue = vnir_msg_payload.blue;
        this->nir = vnir_msg_payload.nir;
    }
}

void CoarseNN::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "CoarseNN ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Zero output variables
    this->ZeroOutputVariables();

    // --> 3. Load nn model
    this->LoadModel();
}

void CoarseNN::UpdateState(uint64_t CurrentSimNanos)
{
    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    CoarsePredictionMsgPayload coarse_msg_buffer = this->coarse_msg.zeroMsgPayload;
    this->ZeroOutputVariables();

    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    coarse_msg_buffer.state = this->state;
    coarse_msg_buffer.prediction = this->prediction;
    this->coarse_msg.write(&coarse_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "CoarseNN ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
