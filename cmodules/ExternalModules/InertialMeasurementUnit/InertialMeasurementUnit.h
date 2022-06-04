//
// Created by gabe :)
//

#ifndef INERTIAL_MEASUREMENT_UNIT_H  // -------------------------------------------------> CHANGE
#define INERTIAL_MEASUREMENT_UNIT_H  // -------------------------------------------------> CHANGE



#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/InertialMeasurementUnitOutMsgPayload.h" // --> CHANGE

#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"



#include "vn/sensors.h"



/*! @brief basic Basilisk C++ module class */
class InertialMeasurementUnit: public SysModel { // --> CHANGE
public:
    InertialMeasurementUnit();  // --> CHANGE
    ~InertialMeasurementUnit(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void ReadMessages();

public:
    // --------------------
    // ----- MESSAGES -----
    // --------------------
    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    Message<InertialMeasurementUnitOutMsgPayload> imu_msg;
    int state;
    double yaw;
    double pitch;
    double roll;
    double temperature;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
