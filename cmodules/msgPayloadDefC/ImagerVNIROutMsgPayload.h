//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_OUT_H
#define IMAGER_VNIR_OUT_H

#include <Eigen/Dense>
#include <string>

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    std::string status_msg;
    Eigen::MatrixXd red;
    Eigen::MatrixXd green;
    Eigen::MatrixXd blue;
    Eigen::MatrixXd nir;
}ImagerVNIROutMsgPayload;

#endif
