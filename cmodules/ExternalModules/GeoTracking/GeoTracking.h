//
// Created by gabe :)
// GeoTracking

#ifndef GEOTRACKING_H
#define GEOTRACKING_H

#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"


// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/FinePredictionMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/GeoTrackingMsgPayload.h" // --> CHANGE



/*! @brief basic Basilisk C++ module class */
class GeoTracking: public SysModel { // --> CHANGE
public:
    GeoTracking();  // --> CHANGE
    ~GeoTracking(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void InitializeTensors();

public:

    // ---------------------
    // ----- VARIABLES -----
    // ---------------------
    int state;

    // --> MESSAGE IN
    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    int fine_state;
    int fine_mask[20][20];

    // --> MESSAGE OUT
    Message<GeoTrackingMsgPayload> geo_msg;
    int lat;
    int lon;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
