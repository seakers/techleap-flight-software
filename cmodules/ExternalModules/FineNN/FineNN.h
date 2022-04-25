//
// Created by gabe :)
//

#ifndef FINE_NN_H
#define FINE_NN_H


#include <torch/script.h>
#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
#include "msgPayloadDefC/CoarsePredictionMsgPayload.h"
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"



/*! @brief basic Basilisk C++ module class */
class FineNN: public SysModel { // --> CHANGE
public:
    FineNN();  // --> CHANGE
    ~FineNN(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void LoadModel();
    void InitializeTensors();
    void ZeroOutputVariables();

public:


    // ---------------------
    // ----- VARIABLES -----
    // ---------------------

    // --> INTERNAL
    int state;
    std::string nn_model_path;
    torch::jit::script::Module nn_model;

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> vnir_msg;
    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;
    ReadFunctor<CoarsePredictionMsgPayload> coarse_msg;

    // --> INPUT
    int vnir_tensor[20][20];    // vnir_msg
    int vnir_state;             // vnir_msg
    int thermal_tensor[20][20]; // thermal_msg
    int thermal_state;          // thermal_msg
    int coarse_prediction;      // coarse_msg
    int coarse_state;           // coarse_msg

    // --> MESSAGE OUT
    Message<FinePredictionMsgPayload> fine_msg;

    // --> OUTPUT
    int mask[20][20]; // fine_msg

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
