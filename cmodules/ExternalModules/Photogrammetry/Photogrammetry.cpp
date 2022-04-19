//
// Created by gabe :)
//

#include "ExternalModules/Photogrammetry/Photogrammetry.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



Photogrammetry::Photogrammetry() // --> CHANGE
{

}

Photogrammetry::~Photogrammetry() // --> CHANGE
{
    return;
}


void Photogrammetry::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    // --> 1. Init image with zeros
    std::cout << "--> RESETTING MODULE: FineNN" << std::endl;
    this->state = 0;
    this->depth = 5;





    // --> 2. Log information
    bskLogger.bskLog(BSK_INFORMATION, "Variable state set to %f in reset.",this->state);
}



void Photogrammetry::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{

    // --> Create output buffer and copy instrument reading
    PhotogrammetryMsgPayload photogrammetry_out_msg_buffer; // --> CHANGE
    photogrammetry_out_msg_buffer.depth = this->depth;

    // --> Write output buffer to output message
    this->photogrammetry_out_msg.write(&photogrammetry_out_msg_buffer, this->moduleID, CurrentSimNanos);


    // --> Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
