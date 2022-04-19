//
// Created by gabe :)
//

#include "ExternalModules/FineNN/FineNN.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



FineNN::FineNN() // --> CHANGE
{

}

FineNN::~FineNN() // --> CHANGE
{
    return;
}


void FineNN::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    // --> 1. Init image with zeros
    std::cout << "--> RESETTING MODULE: FineNN" << std::endl;
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
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->mask[x][y] = 0;
        }
    }





    // --> 2. Log information
    bskLogger.bskLog(BSK_INFORMATION, "Variable state set to %f in reset.",this->state);
}



void FineNN::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{

    // --> Create output buffer and copy instrument reading
    FinePredictionMsgPayload fine_nn_out_msg_buffer; // --> CHANGE
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            fine_nn_out_msg_buffer.mask[x][y] = this->mask[x][y];
        }
    }

    // --> Write output buffer to output message
    this->fine_nn_out_msg.write(&fine_nn_out_msg_buffer, this->moduleID, CurrentSimNanos);


    // --> Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
