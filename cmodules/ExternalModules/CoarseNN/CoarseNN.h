//
// Created by gabe :)
//

#ifndef COARSE_NN_H
#define COARSE_NN_H


#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"

#include "msgPayloadDefC/CoarsePredictionMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class CoarseNN: public SysModel { // --> CHANGE
public:
    CoarseNN();  // --> CHANGE
    ~CoarseNN(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void LoadModel();
    void ZeroOutputVariables();
    void ReadMessages();

public:

    std::string nn_model_path;

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

    // -----------------------
    // ----- MESSAGE OUT -----
    // -----------------------

    Message<CoarsePredictionMsgPayload> coarse_msg;
    int prediction;
    int state;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
