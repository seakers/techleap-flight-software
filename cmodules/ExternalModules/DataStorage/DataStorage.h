//
// Created by gabe :)
//

#ifndef DATA_STORAGE_H  // -------------------------------------------------> CHANGE
#define DATA_STORAGE_H  // -------------------------------------------------> CHANGE
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/FinePredictionMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"



/*! @brief basic Basilisk C++ module class */
class DataStorage: public SysModel { // --> CHANGE
public:
    DataStorage();  // --> CHANGE
    ~DataStorage(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> vnir_image_msg;
    ReadFunctor<ImagerThermalOutMsgPayload> thermal_image_msg;
    ReadFunctor<FinePredictionMsgPayload> fine_nn_msg;

    // --> MESSAGE OUT


    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
