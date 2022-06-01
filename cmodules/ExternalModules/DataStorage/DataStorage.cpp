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

void DataStorage::ReadMessages(){
    // --> VNIR Reading
    if(this->vnir_msg.isLinked()){
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        for(int y = 0; y < 3200; y++){
            for(int z = 0; z < 3200; z++){
                this->red[y][z] = vnir_msg_payload.red[y][z];
                this->green[y][z] = vnir_msg_payload.green[y][z];
                this->blue[y][z] = vnir_msg_payload.blue[y][z];
            }
        }
    }

    // --> Thermal Reading
    if(this->thermal_msg.isLinked()){
        ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
        this->thermal_state = thermal_msg_payload.state;
        for(int y = 0; y < 3200; y++){
            for(int z = 0; z < 3200; z++){
                this->b1[y][z] = thermal_msg_payload.b1[y][z];
                this->b2[y][z] = thermal_msg_payload.b2[y][z];
                this->b3[y][z] = thermal_msg_payload.b3[y][z];
                this->b4[y][z] = thermal_msg_payload.b4[y][z];
            }
        }
    }

    // --> Fine Prediction
    if(this->fine_msg.isLinked()){
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        for(int x = 0; x < 20; x++){
            for(int y = 0; y < 20; y++){
                this->fine_mask[x][y] = fine_msg_payload.mask[x][y];
            }
        }
    }

    // --> GeoTracking data
    if(this->geo_msg.isLinked()){
        GeoTrackingMsgPayload geo_msg_payload = this->geo_msg();
        this->geo_state = geo_msg_payload.state;
        this->geo_lat = geo_msg_payload.lat;
        this->geo_lon = geo_msg_payload.lon;
    }
}

void DataStorage::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void DataStorage::UpdateState(uint64_t CurrentSimNanos)
{
    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------


    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
