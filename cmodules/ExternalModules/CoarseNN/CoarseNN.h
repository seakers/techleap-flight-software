//
// Created by gabe :)
//

#ifndef COARSE_NN_H
#define COARSE_NN_H


#include <torch/script.h>
#include <string>


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
#include "msgPayloadDefC/CoarsePredictionMsgPayload.h"


/*! @brief basic Basilisk C++ module class */
class CoarseNN: public SysModel { // --> CHANGE
public:
    CoarseNN();  // --> CHANGE
    ~CoarseNN(); // --> CHANGE

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

    // --> INPUT
    int vnir_tensor[20][20];    // vnir_msg
    int vnir_state;             // vnir_msg
    int thermal_tensor[20][20]; // thermal_msg
    int thermal_state;          // thermal_msg
    int coarse_prediction;      // coarse_msg
    int coarse_state;           // coarse_msg

    // --> MESSAGE OUT
    Message<CoarsePredictionMsgPayload> coarse_msg;

    // --> OUTPUT
    int prediction; // coarse_msg (0 | 1)

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
