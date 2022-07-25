//
// Created by gabe :)
//

#include "ExternalModules/FineNN/FineNN.h" // --> CHANGE
#include <iostream>
#include <memory>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

using namespace torch::indexing;


FineNN::FineNN() {
    this->nn_model_path = "/home/ben/repos/techleap-flight-software/neuralnet/trainednet.pt";
    this->state = 0;
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
    this->b1.setZero(512, 512);
    this->b2.setZero(512, 512);
    this->b3.setZero(512, 512);
    this->b4.setZero(512, 512);
    this->mask.setZero(512, 512);
}

FineNN::~FineNN() {
    return;
}

void FineNN::LoadModel(){
    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        this->nn_model = torch::jit::load(this->nn_model_path);
        bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- loaded nn model");
    }
    catch (const c10::Error& e) {
        bskLogger.bskLog(BSK_ERROR, "FineNN -------- failed to load nn model");
    }
}

Eigen::MatrixXd FineNN::PerformInference(std::vector<torch::jit::IValue> inputs){
    at::Tensor output = this->nn_model.forward(inputs).toTensor();
    at::Tensor outputSigmoid = at::sigmoid(output);
    at::Tensor outputThreshold = at::gt(outputSigmoid,0.666);
    at::Tensor outputGetMask = outputThreshold.index({0});
    outputGetMask = outputGetMask.index({1});
    // std::cout << outputGetMask << std::endl;
    outputGetMask = outputGetMask.to(torch::kFloat);
    return this->torchTensorToEigenMatrix(outputGetMask);
}

void FineNN::ZeroOutputVariables(){
    this->mask.setZero(512, 512);
}

void FineNN::ReadMessages(){

    if(this->mode_msg.isLinked()){
        std::cout << "MODE MSG IS LINKED!" << std::endl;
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    // --> VNIR Reading
    if(this->vnir_msg.isLinked()){
        std::cout << "VNIR MSG IS LINKED!" << std::endl;
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->red = vnir_msg_payload.red;
        this->green = vnir_msg_payload.green;
        this->blue = vnir_msg_payload.blue;
        this->nir = vnir_msg_payload.nir;
    }

    // --> Thermal Reading
    // if(this->thermal_msg.isLinked()){
    //     ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
    //     this->thermal_state = thermal_msg_payload.state;
    //     this->b1 = thermal_msg_payload.b1;
    //     this->b2 = thermal_msg_payload.b2;
    //     this->b3 = thermal_msg_payload.b3;
    //     this->b4 = thermal_msg_payload.b4;
    // }

    // if(this->coarse_msg.isLinked()){
    //     CoarsePredictionMsgPayload coarse_msg_payload = this->coarse_msg();
    //     this->coarse_state = coarse_msg_payload.state;
    //     this->coarse_prediction = coarse_msg_payload.prediction;
    // }
}

std::vector<Eigen::MatrixXd> FineNN::TileImages(Eigen::MatrixXd largeImage) {
    std::vector<Eigen::MatrixXd> smallImages;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            smallImages.push_back(largeImage.block<161,105>(j*161,i*105));
        }
        smallImages.push_back(largeImage.block<161,105>(512-161,i*105));
    }
    smallImages.push_back(largeImage.block<161,105>(0,512-105));
    smallImages.push_back(largeImage.block<161,105>(161,512-105));
    smallImages.push_back(largeImage.block<161,105>(322,512-105));
    smallImages.push_back(largeImage.block<161,105>(512-161,512-105));
    return smallImages;
}

Eigen::MatrixXd FineNN::UnTileMask(std::vector<Eigen::MatrixXd> maskTiles) {
    Eigen::MatrixXd fullMask = Eigen::MatrixXd::Zero(512,512);
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            fullMask.block<161,105>(j*161,i*105) = maskTiles[3*i+j];
        }
        fullMask.block<29,105>(483,i*105) = maskTiles[3*i+3].block<29,105>(132,0);
    }
    fullMask.block<161,92>(0,420) = maskTiles[16].block<161,92>(0,13);
    fullMask.block<161,92>(161,420) = maskTiles[17].block<161,92>(0,13);
    fullMask.block<161,92>(322,420) = maskTiles[18].block<161,92>(0,13);
    fullMask.block<29,92>(483,420) = maskTiles[19].block<29,92>(132,13);
    return fullMask;
}

void FineNN::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Load nn model
    this->LoadModel();
}

void FineNN::UpdateState(uint64_t CurrentSimNanos){
    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    FinePredictionMsgPayload fine_msg_buffer = this->fine_msg.zeroMsgPayload;
    //this->ZeroOutputVariables();

    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    

    std::vector<Eigen::MatrixXd> redTiles = this->TileImages(this->red);
    std::vector<Eigen::MatrixXd> greenTiles = this->TileImages(this->green);
    std::vector<Eigen::MatrixXd> blueTiles = this->TileImages(this->blue);
    std::vector<Eigen::MatrixXd> nirTiles = this->TileImages(this->nir);
    std::vector<Eigen::MatrixXd> maskTiles;
    //std::cout << redTiles.size() << std::endl;
    for(int i = 0; i < 20; i++) {
        torch::Tensor redTensor = this->eigenMatrixToTorchTensor(redTiles[i]);
        torch::Tensor greenTensor = this->eigenMatrixToTorchTensor(greenTiles[i]);
        torch::Tensor blueTensor = this->eigenMatrixToTorchTensor(blueTiles[i]);
        torch::Tensor nirTensor = this->eigenMatrixToTorchTensor(nirTiles[i]);
        torch::Tensor stackedTensor = torch::stack({redTensor,greenTensor,blueTensor,nirTensor});
        torch::Tensor finalTensor = torch::stack({stackedTensor});
        // std::cout << "dim 0: " << finalTensor.sizes()[0] << std::endl;
        // std::cout << "dim 1: " << finalTensor.sizes()[1] << std::endl;
        // std::cout << "dim 2: " << finalTensor.sizes()[2] << std::endl;
        // std::cout << "dim 3: " << finalTensor.sizes()[3] << std::endl;
        std::vector<torch::jit::IValue> input_image;
        input_image.push_back(finalTensor);
        maskTiles.push_back(this->PerformInference(input_image));
    }
    this->mask = this->UnTileMask(maskTiles);

    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    fine_msg_buffer.state = this->state;
    fine_msg_buffer.mask  = this->mask;
    std::vector<float> controlDegrees = this->CentroidToDegrees(this->GetCentroid(this->mask));
    fine_msg_buffer.pan = controlDegrees.at(0);
    fine_msg_buffer.tilt = controlDegrees.at(1);
    this->fine_msg.write(&fine_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "FineNN -------- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

std::vector<float> FineNN::GetCentroid(Eigen::MatrixXd e) {
    int xSum = 0;
    int ySum = 0;
    int count = 0;
    for(int i = 0; i < e.rows(); i++) {
        for(int j = 0; j < e.cols(); j++) {
            if(e(i,j) > 0.5) {
                //std::cout << i << "," << j << std::endl;
                xSum = xSum + j;
                ySum = ySum + i;
                count = count + 1;
            }
        }
    }
    if(count > 0){
        std::cout << "PLUME DETECTED!" << std::endl;
        this->state = 1; // plume detected
    }
    std::vector<float> centroidLocation;
    centroidLocation.push_back((float)xSum/(float)count);
    centroidLocation.push_back((float)ySum/(float)count);
    return centroidLocation;
}

std::vector<float> FineNN::CentroidToDegrees(std::vector<float> centroid) {
    float pan = (centroid.at(0) - 256.0) * 0.0167;
    float tilt = (centroid.at(1) - 256.0) * 0.0167;
    std::vector<float> controlDegrees;
    controlDegrees.push_back(pan);
    controlDegrees.push_back(tilt);
    return controlDegrees;
}

torch::Tensor FineNN::eigenMatrixToTorchTensor(Eigen::MatrixXd e){
    auto t = torch::empty({e.cols(),e.rows()});
    float* data = t.data_ptr<float>();

    Eigen::Map<Eigen::MatrixXf> ef(data,t.size(1),t.size(0));
    ef = e.cast<float>();
    t.requires_grad_(true);
    return t.transpose(0,1);
}

Eigen::MatrixXd FineNN::torchTensorToEigenMatrix(torch::Tensor T){
    float* data = T.data_ptr<float>();
    Eigen::Map<Eigen::MatrixXf> e(data, T.size(0), T.size(1));
    return e.cast <double> ();
}

