//
// Created by gabe :)
//

#ifndef GPS_MODULE_H  // -------------------------------------------------> CHANGE
#define GPS_MODULE_H  // -------------------------------------------------> CHANGE



#include "msgPayloadDefC/GpsOutMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"





/*! @brief basic Basilisk C++ module class */
class Gps: public SysModel { // --> CHANGE
public:
    Gps();  // --> CHANGE
    ~Gps(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();

public:

    // --> VARIABLES
    int state;
    int angles[3];

    // --> MESSAGE IN

    // --> MESSAGE OUT
    Message<GpsOutMsgPayload> gps_msg;

    // --> OUTPUT
    double lat;       // gps_msg
    double lon;       // gps_msg
    double altitude;  // gps_msg

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
