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

}

CoarseNN::~CoarseNN() // --> CHANGE
{
    return;
}


void CoarseNN::LoadModel(){
    // --> TODO: Load coarse model (look at roshan code)
    bskLogger.bskLog(BSK_INFORMATION, "CoarseNN ------ loaded nn model");
}

void CoarseNN::InitializeTensors(){
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
}

void CoarseNN::ZeroOutputVariables(){


    this->prediction = 0;
}











void CoarseNN::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "CoarseNN ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();

    // --> 3. Load nn model
    this->LoadModel();
}



void CoarseNN::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    CoarsePredictionMsgPayload coarse_msg_buffer = this->coarse_msg.zeroMsgPayload;

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


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Implement roshan model to coarsely classify input from thermal and vnir cameras
    // --> TODO: Copy prediction to local output variable

    this->state += 1;
    this->prediction = 0;




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
