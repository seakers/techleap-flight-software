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
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"

#include "msgPayloadDefC/GeoTrackingMsgPayload.h"



/*! @brief basic Basilisk C++ module class */
class GeoTracking: public SysModel { // --> CHANGE
public:
    GeoTracking();
    ~GeoTracking();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void ReadMessages();

public:

    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    int fine_state;
    int fine_mask[20][20];


    // -----------------------
    // ----- MESSAGE OUT -----
    // -----------------------

    Message<GeoTrackingMsgPayload> geo_msg;
    int state;
    int lat;
    int lon;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif