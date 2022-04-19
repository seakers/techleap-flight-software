//
// Created by gabe :)
//

#include "ExternalModules/InertialMeasurementUnit/InertialMeasurementUnit.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"


#include "vn/sensors.h"


InertialMeasurementUnit::InertialMeasurementUnit() // --> CHANGE
{

}

InertialMeasurementUnit::~InertialMeasurementUnit() // --> CHANGE
{
    return;
}


void InertialMeasurementUnit::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    // --> 1. Init image with zeros
    std::cout << "--> RESETTING MODULE: InertialMeasurementUnit" << std::endl;
    this->state = 0;
    for(int x = 0; x < 3; x++){
        this->angles[x] = 0;
    }





    // --> 2. Log information
    bskLogger.bskLog(BSK_INFORMATION, "Variable state set to %f in reset.",this->state);
}



void InertialMeasurementUnit::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{

    // --> Create output buffer and copy instrument reading
    InertialMeasurementUnitOutMsgPayload imu_out_msg_buffer; // --> CHANGE
    for(int x = 0; x < 3; x++){
        imu_out_msg_buffer.angles[x] = this->angles[x];
    }

    // --> Write output buffer to output message
    this->imu_out_msg.write(&imu_out_msg_buffer, this->moduleID, CurrentSimNanos);


    // --> Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
