//
// Created by gabe :)
//

#include "ExternalModules/DataStorage/DataStorage.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



DataStorage::DataStorage() // --> CHANGE
{

}

DataStorage::~DataStorage() // --> CHANGE
{
    return;
}


void DataStorage::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    // --> 1. Init image with zeros
    std::cout << "--> RESETTING MODULE: DataStorage" << std::endl;
    this->state = 0;





    // --> 2. Log information
    bskLogger.bskLog(BSK_INFORMATION, "Variable state set to %f in reset.",this->state);
}



void DataStorage::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{



    // --> Log module run
    bskLogger.bskLog(BSK_INFORMATION, "C++ Module ID %lld ran Update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
