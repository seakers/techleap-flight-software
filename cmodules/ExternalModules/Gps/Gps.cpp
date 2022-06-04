//
// Created by gabe :)
//

#include "ExternalModules/Gps/Gps.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"


#include "vn/sensors.h"


Gps::Gps() // --> CHANGE
{

}

Gps::~Gps() // --> CHANGE
{
    return;
}


void Gps::ZeroOutputVariables(){
    this->lat = 0.0;
    this->lon = 0.0;
    this->altitude = 0.0;
}

void Gps::ReadMessages(){
    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }
}

void Gps::Reset(uint64_t CurrentSimNanos) // --> CHANGE
{
    bskLogger.bskLog(BSK_INFORMATION, "Gps ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void Gps::UpdateState(uint64_t CurrentSimNanos) // --> CHNAGE
{


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    GpsOutMsgPayload gps_msg_buffer = this->gps_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    // --> TODO: read gps sensor to get location

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO:


    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    gps_msg_buffer.state    = this->state;
    gps_msg_buffer.lat      = this->lat;
    gps_msg_buffer.lon      = this->lon;
    gps_msg_buffer.altitude = this->altitude;
    this->gps_msg.write(&gps_msg_buffer, this->moduleID, CurrentSimNanos);






    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "Gps ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}
