//
// Created by gabe :)
// Photogrammetry

#ifndef PHOTOGRAMMETRY_H  // -------------------------------------------------> CHANGE
#define PHOTOGRAMMETRY_H  // -------------------------------------------------> CHANGE
#include "msgPayloadDefC/FinePredictionMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/PhotogrammetryMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"



/*! @brief basic Basilisk C++ module class */
class Photogrammetry: public SysModel { // --> CHANGE
public:
    Photogrammetry();  // --> CHANGE
    ~Photogrammetry(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;
    int depth;

    // --> MESSAGE IN
    ReadFunctor<FinePredictionMsgPayload> fine_nn_msg;

    // --> MESSAGE OUT
    Message<PhotogrammetryMsgPayload> photogrammetry_out_msg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
