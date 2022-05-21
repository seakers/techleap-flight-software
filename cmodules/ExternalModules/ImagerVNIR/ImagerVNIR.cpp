//
// Created by gabe :)
//

#include "ExternalModules/ImagerVNIR/ImagerVNIR.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



ImagerVNIR::ImagerVNIR() // --> CHANGE
{

}

ImagerVNIR::~ImagerVNIR() // --> CHANGE
{
    return;
}

void ImagerVNIR::InitializeTensors(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->image_tensor[x][y] = 0;
        }
    }
}

void ImagerVNIR::ZeroOutputVariables(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->image_tensor[x][y] = 0;
        }
    }
}



void ImagerVNIR::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "ImagerVNIR ---- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();
}



void ImagerVNIR::UpdateState(uint64_t CurrentSimNanos) {


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    ImagerVNIROutMsgPayload vnir_msg_buffer = this->vnir_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Implement SDK reading vnir sensor and copy values over to image_tensor
    this->state += 1;

    if(this->mock_msg.isLinked()){
        ImagerVNIROutMsgPayload mock_msg_payload = this->mock_msg();
        for(int x = 0; x < 20; x++){
            for(int y = 0; y < 20; y++){
                this->image_tensor[x][y] = mock_msg_payload.imageTensor[x][y];
            }
        }
    }
    else{
        for(int x = 0; x < 20; x++){
            for(int y = 0; y < 20; y++){
                // this->image_tensor[x][y] = this->image_tensor[x][y];
                this->image_tensor[x][y] = this->state;
            }
        }
    }




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write correct image dimensions

    vnir_msg_buffer.state = this->state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            vnir_msg_buffer.imageTensor[x][y] = this->image_tensor[x][y];
        }
    }
    this->vnir_msg.write(&vnir_msg_buffer, this->moduleID, CurrentSimNanos);


    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "ImagerVNIR ---- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
