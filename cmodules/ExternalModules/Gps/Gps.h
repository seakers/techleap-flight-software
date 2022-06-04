//
// Created by gabe :)
//

#ifndef GPS_MODULE_H  // -------------------------------------------------> CHANGE
#define GPS_MODULE_H  // -------------------------------------------------> CHANGE





#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/GpsOutMsgPayload.h" // --> CHANGE





/*! @brief basic Basilisk C++ module class */
class Gps: public SysModel { // --> CHANGE
public:
    Gps();  // --> CHANGE
    ~Gps(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ZeroOutputVariables();
    void ReadMessages();

public:

    int angles[3];

    // --------------------
    // ----- MESSAGES -----
    // --------------------
    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    Message<GpsOutMsgPayload> gps_msg;
    int state;
    double lat;
    double lon;
    double altitude;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
