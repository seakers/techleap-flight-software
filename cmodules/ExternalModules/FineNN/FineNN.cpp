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
    // this->nn_model_path = "/home/gabe/repos/techleap/techleap-flight-software/neuralnet/trainednet.pt";
    this->nn_model_path = "/app/neuralnet/trainednet.pt";
    this->state = 0;
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

void FineNN::InitializeTensors(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->thermal_tensor[x][y] = 0;
        }
    }
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->vnir_tensor[x][y] = 0;
        }
    }
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->mask[x][y] = 0;
        }
    }
}

void FineNN::ZeroOutputVariables(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->mask[x][y] = 0;
        }
    }
}







void FineNN::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();

    // --> 3. Load nn model
    this->LoadModel();
}



void FineNN::UpdateState(uint64_t CurrentSimNanos) {


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    FinePredictionMsgPayload fine_msg_buffer = this->fine_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO: Write correct image dimensions

    // --> VNIR Reading
    ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
    this->vnir_state = vnir_msg_payload.state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->vnir_tensor[x][y] = vnir_msg_payload.imageTensor[x][y];
        }
    }

    // --> Thermal Reading
    ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
    this->thermal_state = thermal_msg_payload.state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->thermal_tensor[x][y] = thermal_msg_payload.imageTensor[x][y];
        }
    }

    // --> Coarse Prediction
    CoarsePredictionMsgPayload coarse_msg_payload = this->coarse_msg();
    this->coarse_state = coarse_msg_payload.state;
    this->coarse_prediction = coarse_msg_payload.prediction;


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Implement pytorch model to classify input from thermal and vnir cameras
    // --> TODO: Copy mask prediction values over to local mask output variable

    this->state += 1;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->mask[x][y] = this->thermal_tensor[x][y];
        }
    }




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write correct image dimensions

    fine_msg_buffer.state = this->state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            fine_msg_buffer.mask[x][y] = this->mask[x][y];
        }
    }
    this->fine_msg.write(&fine_msg_buffer, this->moduleID, CurrentSimNanos);


    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
