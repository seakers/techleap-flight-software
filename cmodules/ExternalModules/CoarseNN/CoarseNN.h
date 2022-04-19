//
// Created by gabe :)
//

#ifndef COARSE_NN_H  // -------------------------------------------------> CHANGE
#define COARSE_NN_H  // -------------------------------------------------> CHANGE
#include "msgPayloadDefC/ImagerThermalOutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/ImagerVNIROutMsgPayload.h" // --> CHANGE
#include "msgPayloadDefC/CoarsePredictionMsgPayload.h" // --> CHANGE


#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"



/*! @brief basic Basilisk C++ module class */
class CoarseNN: public SysModel { // --> CHANGE
public:
    CoarseNN();  // --> CHANGE
    ~CoarseNN(); // --> CHANGE

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

public:

    // --> VARIABLES
    int state;
    int thermal_tensor[20][20];
    int vnir_tensor[20][20];

    // --> MESSAGE IN
    ReadFunctor<ImagerVNIROutMsgPayload> vnir_image_msg;  //!< translation navigation output msg
    ReadFunctor<ImagerThermalOutMsgPayload> thermal_image_msg;  //!< translation navigation output msg

    // --> MESSAGE OUT
    Message<CoarsePredictionMsgPayload> coarse_nn_out_msg;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
