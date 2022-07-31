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

#include <Eigen/Dense>

#include <torch/torch.h>
#include <torch/script.h>


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
    void PreProcessImages();
    Eigen::MatrixXd PerformInference(std::vector<torch::jit::IValue> inputs);
    std::vector<Eigen::MatrixXd> TileImages(Eigen::MatrixXd largeImage);
    Eigen::MatrixXd UnTileMask(std::vector<Eigen::MatrixXd> maskTiles);
    torch::Tensor eigenMatrixToTorchTensor(Eigen::MatrixXd e);
    Eigen::MatrixXd torchTensorToEigenMatrix(torch::Tensor T);
    std::vector<float> GetCentroid(Eigen::MatrixXd e);
    std::vector<float> CentroidToDegrees(std::vector<float> centroid);

public:

    std::string nn_model_path;
    torch::jit::script::Module nn_model;

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

    ReadFunctor<ImagerThermalOutMsgPayload> thermal_msg;
    Eigen::MatrixXd b1;
    Eigen::MatrixXd b2;
    Eigen::MatrixXd b3;
    Eigen::MatrixXd b4;
    int thermal_state;

    ReadFunctor<CoarsePredictionMsgPayload> coarse_msg;
    int coarse_prediction;
    int coarse_state;


    // -----------------------
    // ----- MESSAGE OUT -----
    // -----------------------

    Message<FinePredictionMsgPayload> fine_msg;
    int state;
    float pan;
    float tilt;
    Eigen::MatrixXd mask;


    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
