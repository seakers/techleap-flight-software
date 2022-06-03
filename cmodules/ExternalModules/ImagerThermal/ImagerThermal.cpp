//
// Created by gabe :)
//

#include "ExternalModules/ImagerThermal/ImagerThermal.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



ImagerThermal::ImagerThermal() // --> CHANGE
{
    this->b1.setZero(512, 512);
    this->b2.setZero(512, 512);
    this->b3.setZero(512, 512);
    this->b4.setZero(512, 512);
}

ImagerThermal::~ImagerThermal() // --> CHANGE
{
    return;
}

void ImagerThermal::ZeroOutputVariables(){
    this->b1.setZero(512, 512);
    this->b2.setZero(512, 512);
    this->b3.setZero(512, 512);
    this->b4.setZero(512, 512);
}





void ImagerThermal::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "ImagerThermal - (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->ZeroOutputVariables();
}



void ImagerThermal::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{

    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    ImagerThermalOutMsgPayload thermal_msg_buffer = this->thermal_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Implement SDK reading thermal sensor and copy values over to image_tensor

//    if(this->mock_msg.isLinked()){
//        ImagerThermalOutMsgPayload mock_msg_payload = this->mock_msg();
//        for(int x = 0; x < 20; x++){
//            for(int y = 0; y < 20; y++){
//                this->image_tensor[x][y] = mock_msg_payload.imageTensor[x][y];
//            }
//        }
//    }
//    else {
//        for(int x = 0; x < 20; x++){
//            for(int y = 0; y < 20; y++){
//                this->image_tensor[x][y] = this->image_tensor[x][y];
//            }
//        }
//    }




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write correct image dimensions
    thermal_msg_buffer.state = this->state;
    thermal_msg_buffer.b1 = this->b1;
    thermal_msg_buffer.b2 = this->b2;
    thermal_msg_buffer.b3 = this->b3;
    thermal_msg_buffer.b4 = this->b4;
    this->thermal_msg.write(&thermal_msg_buffer, this->moduleID, CurrentSimNanos);


    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "ImagerThermal - ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
