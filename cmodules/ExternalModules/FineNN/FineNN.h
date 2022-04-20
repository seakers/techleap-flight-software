//
// Created by gabe :)
//

#ifndef FINE_NN_H  // -------------------------------------------------> CHANGE
#define FINE_NN_H  // -------------------------------------------------> CHANGE
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/CoarsePredictionMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/FinePredictionMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"


#include <torch/script.h>
#include <string>


/*! @brief basic Basilisk C++ module class */
class FineNN: public SysModel { // --> CHANGE
public:
    FineNN();  // --> CHANGE
    ~FineNN(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;
    int thermal_tensor[20][20];
    int vnir_tensor[20][20];
    int mask[20][20];


    std::string nn_model_path;
    torch::jit::script::Module nn_model;

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> vnir_image_msg;
    ReadFunctor<ImagerThermalOutMsgPayload> thermal_image_msg;
    ReadFunctor<CoarsePredictionMsgPayload> coarse_nn_out_msg;

    // --> MESSAGE OUT
    Message<FinePredictionMsgPayload> fine_nn_out_msg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
