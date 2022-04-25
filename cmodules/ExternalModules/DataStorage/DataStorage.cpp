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


void DataStorage::InitializeTensors(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->fine_mask[x][y] = 0;
        }
    }
}

void DataStorage::ZeroOutputVariables(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->fine_mask[x][y] = 0;
        }
    }
}

void DataStorage::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();
}



void DataStorage::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO: Write correct image dimensions

    // --> VNIR Reading
    ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
    this->vnir_state = vnir_msg_payload.state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->vnir_tensor[x][y] = vnir_msg_payload.imageTensor[x][y];
        }
    }

    // --> Thermal Reading
    ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
    this->thermal_state = thermal_msg_payload.state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->thermal_tensor[x][y] = thermal_msg_payload.imageTensor[x][y];
        }
    }

    // --> Fine Prediction
    FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
    this->fine_state = fine_msg_payload.state;
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->fine_mask[x][y] = fine_msg_payload.mask[x][y];
        }
    }


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Image post-processing... Compression?




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write to some storage device



    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
