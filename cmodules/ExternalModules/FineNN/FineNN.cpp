//
// Created by gabe :)
//

#include "ExternalModules/FineNN/FineNN.h" // --> CHANGE
#include <iostream>
#include <memory>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



FineNN::FineNN() {
    this->nn_model_path = "/home/ben/repos/techleap-flight-software/neuralnet/trainednet.pt";
    this->state = 0;
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
    this->b1.setZero(512, 512);
    this->b2.setZero(512, 512);
    this->b3.setZero(512, 512);
    this->b4.setZero(512, 512);
    this->mask.setZero(512, 512);
}

FineNN::~FineNN() {
    return;
}

void FineNN::LoadModel(){
    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        this->nn_model = torch::jit::load(this->nn_model_path);
        bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- loaded nn model");
    }
    catch (const c10::Error& e) {
        bskLogger.bskLog(BSK_ERROR, "FineNN -------- failed to load nn model");
    }
}

void FineNN::ZeroOutputVariables(){
    this->mask.setZero(512, 512);
}

void FineNN::ReadMessages(){

    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    // --> VNIR Reading
    if(this->vnir_msg.isLinked()){
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->red = vnir_msg_payload.red;
        this->green = vnir_msg_payload.green;
        this->blue = vnir_msg_payload.blue;
        this->nir = vnir_msg_payload.nir;
    }

    // --> Thermal Reading
    if(this->thermal_msg.isLinked()){
        ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
        this->thermal_state = thermal_msg_payload.state;
        this->b1 = thermal_msg_payload.b1;
        this->b2 = thermal_msg_payload.b2;
        this->b3 = thermal_msg_payload.b3;
        this->b4 = thermal_msg_payload.b4;
    }

    if(this->coarse_msg.isLinked()){
        CoarsePredictionMsgPayload coarse_msg_payload = this->coarse_msg();
        this->coarse_state = coarse_msg_payload.state;
        this->coarse_prediction = coarse_msg_payload.prediction;
    }
}

void FineNN::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Load nn model
    this->LoadModel();
}

void FineNN::UpdateState(uint64_t CurrentSimNanos){
    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    FinePredictionMsgPayload fine_msg_buffer = this->fine_msg.zeroMsgPayload;
    this->ZeroOutputVariables();

    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();



    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Implement pytorch model to classify input from thermal and vnir cameras
    // --> TODO: Copy mask prediction values over to local mask output variable


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    fine_msg_buffer.state = this->state;
    fine_msg_buffer.mask  = this->mask;
    this->fine_msg.write(&fine_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
