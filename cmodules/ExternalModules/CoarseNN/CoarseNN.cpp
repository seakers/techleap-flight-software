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


void CoarseNN::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    // --> 1. Init image with zeros
    std::cout << "--> RESETTING MODULE: CoarseNN" << std::endl;
    this->state = 0;
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





    // --> 2. Log information
    bskLogger.bskLog(BSK_INFORMATION, "Variable state set to %f in reset.",this->state);
}



void CoarseNN::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // --> Create output buffer and copy instrument reading
    CoarsePredictionMsgPayload coarse_nn_out_msg_buffer; // --> CHANGE
    coarse_nn_out_msg_buffer.prediction = 0;


    // --> Write output buffer to output message
    this->coarse_nn_out_msg.write(&coarse_nn_out_msg_buffer, this->moduleID, CurrentSimNanos);


    // --> Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
