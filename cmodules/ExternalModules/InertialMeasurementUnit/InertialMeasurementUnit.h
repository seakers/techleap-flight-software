//
// Created by gabe :)
//

#ifndef INERTIAL_MEASUREMENT_UNIT_H  // -------------------------------------------------> CHANGE
#define INERTIAL_MEASUREMENT_UNIT_H  // -------------------------------------------------> CHANGE



#include "msgPayloadDefC/InertialMeasurementUnitOutMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"





/*! @brief basic Basilisk C++ module class */
class InertialMeasurementUnit: public SysModel { // --> CHANGE
public:
    InertialMeasurementUnit();  // --> CHANGE
    ~InertialMeasurementUnit(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;
    int angles[3];

    // --> MESSAGE IN

    // --> MESSAGE OUT
    Message<InertialMeasurementUnitOutMsgPayload> imu_out_msg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
