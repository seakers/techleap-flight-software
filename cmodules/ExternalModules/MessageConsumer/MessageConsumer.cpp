//
// Created by gabe :)
//

#include "ExternalModules/MessageConsumer/MessageConsumer.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



MessageConsumer::MessageConsumer() // --> CHANGE
{

}

MessageConsumer::~MessageConsumer() // --> CHANGE
{
    return;
}



void MessageConsumer::ZeroOutputVariables(){
    return;
}




void MessageConsumer::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void MessageConsumer::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO: read balloon messaging system here

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: determine which module to send the message to



    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: create outputs for each module


    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "MessageConsumer ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}