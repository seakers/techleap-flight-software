//
// Created by gabe :)
//

#ifndef FINE_NN_H  // -------------------------------------------------> CHANGE
#define FINE_NN_H  // -------------------------------------------------> CHANGE
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/FinePredictionMsgPayload.h" // --> CHANGE

// #include "msgPayloadDefC/CoarsePredictionMsgPayload.h" // --> CHANGE
#include "cMsgCInterface/CoarsePredictionMsg_C.h"

//#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
//#include "msgPayloadDefCpp/ImagerVNIROutMsgPayload.h"
#include "cMsgCInterface/ImagerVNIROutMsg_C.h"


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

    void LoadModel();
    void InitializeTensors();

public:

    // --> VARIABLES
    int state;
    int thermal_tensor[20][20];
    int vnir_tensor[20][20];
    int mask[20][20];


    std::string nn_model_path;
    torch::jit::script::Module nn_model;

    // --> MESSAGE IN
    ImagerVNIROutMsg_C vnir_msg;
    // ReadFunctor<ImagerVNIROutMsgPayload> vnir_msg;


    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;

    CoarsePredictionMsg_C coarse_msg;
    // ReadFunctor<CoarsePredictionMsgPayload> coarse_msg;


    // --> MESSAGE OUT
    Message<FinePredictionMsgPayload> fine_msg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
