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


void ImagerVNIR::ZeroOutputVariables(){
    for(int y = 0; y < 3200; y++){
        for(int z = 0; z < 3200; z++){
            this->red[y][z] = 0;
            this->green[y][z] = 0;
            this->blue[y][z] = 0;
        }
    }
}



void ImagerVNIR::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "ImagerVNIR ---- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->ZeroOutputVariables();
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

//    if(this->mock_msg.isLinked()){
//        ImagerVNIROutMsgPayload mock_msg_payload = this->mock_msg();
//        for(int x = 0; x < 20; x++){
//            for(int y = 0; y < 20; y++){
//                this->image_tensor[x][y] = mock_msg_payload.imageTensor[x][y];
//            }
//        }
//    }
//    else{
//        for(int x = 0; x < 20; x++){
//            for(int y = 0; y < 20; y++){
//                // this->image_tensor[x][y] = this->image_tensor[x][y];
//                this->image_tensor[x][y] = this->state;
//            }
//        }
//    }




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write correct image dimensions
    vnir_msg_buffer.state = this->state;
    for(int y = 0; y < 3200; y++){
        for(int z = 0; z < 3200; z++){
            vnir_msg_buffer.red[y][z] = this->red[y][z];
            vnir_msg_buffer.green[y][z] = this->green[y][z];
            vnir_msg_buffer.blue[y][z] = this->blue[y][z];
        }
    }
    this->vnir_msg.write(&vnir_msg_buffer, this->moduleID, CurrentSimNanos);


    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "ImagerVNIR ---- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
