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
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"
#include "msgPayloadDefC/GeoTrackingMsgPayload.h"



/*! @brief basic Basilisk C++ module class */
class DataStorage: public SysModel { // --> CHANGE
public:
    DataStorage();
    ~DataStorage();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ReadMessages();

public:

    int state;

    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<ImagerVNIROutMsgPayload> vnir_msg;
    double red[3200][3200];
    double green[3200][3200];
    double blue[3200][3200];
    int vnir_state;

    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;
    double b1[3200][3200];
    double b2[3200][3200];
    double b3[3200][3200];
    double b4[3200][3200];
    int thermal_state;

    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    int fine_mask[20][20];
    int fine_state;

    ReadFunctor<GeoTrackingMsgPayload> geo_msg;
    double geo_lat;
    double geo_lon;
    int geo_state;



    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
