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

}

ImagerThermal::~ImagerThermal() // --> CHANGE
{
    return;
}

void ImagerThermal::InitializeTensors(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->image_tensor[x][y] = 0;
        }
    }
}

void ImagerThermal::ZeroOutputVariables(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->image_tensor[x][y] = 0;
        }
    }
}





void ImagerThermal::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "ImagerThermal - (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();
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
    this->state += 1;

    if(this->mock_msg.isLinked()){
        ImagerThermalOutMsgPayload mock_msg_payload = this->mock_msg();
        for(int x = 0; x < 20; x++){
            for(int y = 0; y < 20; y++){
                this->image_tensor[x][y] = mock_msg_payload.imageTensor[x][y];
            }
        }
    }
    else {
        for(int x = 0; x < 20; x++){
            for(int y = 0; y < 20; y++){
                this->image_tensor[x][y] = this->image_tensor[x][y];
            }
        }
    }




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write correct image dimensions

    thermal_msg_buffer.state = this->state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            thermal_msg_buffer.imageTensor[x][y] = this->image_tensor[x][y];
        }
    }
    this->thermal_msg.write(&thermal_msg_buffer, this->moduleID, CurrentSimNanos);


    // -------------------
    // ----- Logging -----
    // -------------------


    bskLogger.bskLog(BSK_INFORMATION, "ImagerThermal - ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
