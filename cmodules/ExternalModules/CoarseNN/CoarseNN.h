//
// Created by gabe :)
//

#ifndef COARSE_NN_H
#define COARSE_NN_H


//#include <torch/script.h>
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

#include <Eigen/Dense>



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
    //torch::jit::script::Module nn_model;

    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<ImagerVNIROutMsgPayload> vnir_msg;
    Eigen::MatrixXd red;
    Eigen::MatrixXd green;
    Eigen::MatrixXd blue;
    Eigen::MatrixXd nir;
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
