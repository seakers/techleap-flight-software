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
    this->nn_model_path = "/home/gabe/repos/techleap/techleap-flight-software/neuralnet/trainednet.pt";
    this->state = 0;
}

FineNN::~FineNN() {
    return;
}



void FineNN::LoadModel(){
    std::cout << "--> LOADING NN MODEL: " + this->nn_model_path << std::endl;

    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        this->nn_model = torch::jit::load(this->nn_model_path);
        std::cout << "--> MODEL LOADED\n";
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
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





void FineNN::Reset(uint64_t CurrentSimNanos) {
    std::cout << "--> RESETTING MODULE: FineNN" << std::endl;

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();

    // --> 3. Load nn model
    this->LoadModel();
}



void FineNN::UpdateState(uint64_t CurrentSimNanos) {


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------

    // --> VNIR Reading
//    ImagerVNIROutMsgPayload vnir_msg_payload = ImagerVNIROutMsg_C_read(&this->vnir_msg);
//    ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();








    // --> Thermal Reading
    // ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();

    // --> Coarse Prediction
    CoarsePredictionMsgPayload coarse_msg_payload = CoarsePredictionMsg_C_read(&this->coarse_msg);
    std::cout << "--> STATE: " << coarse_msg_payload.prediction << std::endl;
    // CoarsePredictionMsgPayload coarse_msg_payload = this->coarse_msg();



    // --------------------------
    // ----- Process Inputs -----
    // --------------------------












    // -------------------------
    // ----- Write Outputs -----
    // -------------------------














//
//    // --> Create output buffer and copy instrument reading
//    FinePredictionMsgPayload fine_msg_buffer; // --> CHANGE
//    for(int x = 0; x < 20; x++){
//        for(int y = 0; y < 20; y++){
//            fine_msg_buffer.mask[x][y] = this->mask[x][y];
//        }
//    }
//
//    // --> Write output buffer to output message
//    this->fine_msg.write(&fine_msg_buffer, this->moduleID, CurrentSimNanos);


    // --> Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
