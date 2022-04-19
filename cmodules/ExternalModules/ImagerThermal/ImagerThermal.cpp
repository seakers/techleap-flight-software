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


void ImagerThermal::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    // --> 1. Init image with zeros
    std::cout << "--> RESETTING MODULE";
    /*! - reset any required variables */
    this->state = 0;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->imageTensor[x][y] = 0;
        }
    }

    // --> 2. Log information
    bskLogger.bskLog(BSK_INFORMATION, "Variable state set to %f in reset.",this->state);
}



void ImagerThermal::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{
    // --> 1. Get instrument reading
    int thermal_reading[20][20] = {0};
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            thermal_reading[x][y] = 0;
        }
    }


    // --> 2. Create output buffer and copy instrument reading
    ImagerThermalOutMsgPayload outMsgBuffer; // --> CHANGE
    outMsgBuffer.state = 0;
    memcpy(&outMsgBuffer.imageTensor, &thermal_reading, sizeof(thermal_reading));


    // --> 3. Write output buffer to output message
    this->dataOutMsg.write(&outMsgBuffer, this->moduleID, CurrentSimNanos);


    // --> 4. Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
