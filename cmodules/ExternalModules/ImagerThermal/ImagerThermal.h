//
// Created by gabe :)
//

#ifndef IMAGER_THERMAL_H  // --------------------------------------------> CHANGE
#define IMAGER_THERMAL_H  // --------------------------------------------> CHANGE
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"



/*! @brief basic Basilisk C++ module class */
class ImagerThermal: public SysModel { // --> CHANGE
public:
    ImagerThermal();  // --> CHANGE
    ~ImagerThermal(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;
    int imageTensor[20][20];

    // --> MESSAGES
    Message<ImagerThermalOutMsgPayload> dataOutMsg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
