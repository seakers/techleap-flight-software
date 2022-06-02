//
// Created by gabe :)
//

#ifndef FINE_NN_H
#define FINE_NN_H


//#include <torch/script.h>
#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
#include "msgPayloadDefC/CoarsePredictionMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"

#include "msgPayloadDefC/FinePredictionMsgPayload.h"



/*! @brief basic Basilisk C++ module class */
class FineNN: public SysModel { // --> CHANGE
public:
    FineNN();
    ~FineNN();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void LoadModel();
    void ZeroOutputVariables();
    void ReadMessages();

public:

    std::string nn_model_path;
    //torch::jit::script::Module nn_model;

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

    ReadFunctor<CoarsePredictionMsgPayload> coarse_msg;
    int coarse_prediction;
    int coarse_state;


    // -----------------------
    // ----- MESSAGE OUT -----
    // -----------------------

    Message<FinePredictionMsgPayload> fine_msg;
    int state;
    int mask[20][20];


    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
