//
// Created by gabe :)
//

#include "ExternalModules/GeoTracking/GeoTracking.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



GeoTracking::GeoTracking() // --> CHANGE
{
    // --> Always start FSW on mode 0
    this->state = 0;
}

GeoTracking::~GeoTracking() // --> CHANGE
{
    return;
}

void GeoTracking::ZeroOutputVariables(){
    this->lat = 0.0;
    this->lon = 0.0;
}

void GeoTracking::InitializeTensors(){
    for(int x = 0; x < 20; x++){
        for(int y = 0; y < 20; y++){
            this->fine_mask[x][y] = 0;
        }
    }
}





void GeoTracking::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "GeoTracking ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->InitializeTensors();
}



void GeoTracking::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{

    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    GeoTrackingMsgPayload geo_msg_buffer = this->geo_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    if(this->fine_msg.isLinked()){
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        for(int x = 0; x < 20; x++){
            for(int y = 0; y < 20; y++){
                this->fine_mask[x][y] = fine_msg_payload.mask[x][y];
            }
        }
    }

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Determine lat / lon from mask


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    geo_msg_buffer.state = this->state;
    geo_msg_buffer.lat = this->lat;
    geo_msg_buffer.lon = this->lon;
    this->geo_msg.write(&geo_msg_buffer, this->moduleID, CurrentSimNanos);



    // -------------------
    // ----- Logging -----
    // -------------------

    bskLogger.bskLog(BSK_INFORMATION, "GeoTracking -------- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
