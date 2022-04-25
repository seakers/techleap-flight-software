//
// Created by gabe :)
//

#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"



/*! @brief basic Basilisk C++ module class */
class DataStorage: public SysModel { // --> CHANGE
public:
    DataStorage();  // --> CHANGE
    ~DataStorage(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void InitializeTensors();
    void ZeroOutputVariables();

public:


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> vnir_msg;
    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;
    ReadFunctor<FinePredictionMsgPayload> fine_msg;

    // --> INPUT
    int vnir_tensor[20][20];    // vnir_msg
    int vnir_state;             // vnir_msg
    int thermal_tensor[20][20]; // thermal_msg
    int thermal_state;          // thermal_msg
    int fine_mask[20][20];      // fine_msg
    int fine_state;             // fine_msg

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
