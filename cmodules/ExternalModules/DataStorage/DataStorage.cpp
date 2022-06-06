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
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
    //this->b1.setZero(512, 512);
    this->fine_mask.setZero(512, 512);
}

DataStorage::~DataStorage() // --> CHANGE
{
    return;
}

void DataStorage::ReadMessages(){
    // --> VNIR Reading
    if(this->vnir_msg.isLinked()){
        std::cout << "VNIR MSG IS LINKED!" << std::endl;
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->red = vnir_msg_payload.red;
        this->green = vnir_msg_payload.green;
        this->blue = vnir_msg_payload.blue;
        this->nir = vnir_msg_payload.nir;
    }

    // // --> Thermal Reading
    // if(this->thermal_msg.isLinked()){
    //     ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
    //     this->thermal_state = thermal_msg_payload.state;
    //     this->b1 = thermal_msg_payload.b1;
    //     this->b2 = thermal_msg_payload.b2;
    //     this->b3 = thermal_msg_payload.b3;
    //     this->b4 = thermal_msg_payload.b4;
    // }

    // --> Fine Prediction
    if(this->fine_msg.isLinked()){
        std::cout << "FINENN MSG IS LINKED!" << std::endl;
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        this->fine_mask = fine_msg_payload.mask;
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
