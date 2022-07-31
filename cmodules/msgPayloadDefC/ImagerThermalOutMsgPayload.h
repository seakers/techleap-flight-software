//
// Created by gabe :)
//

#ifndef IMAGER_THERMAL_OUT_H
#define IMAGER_THERMAL_OUT_H

#include <Eigen/Dense>

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    Eigen::MatrixXd b1;
    Eigen::MatrixXd b2;
    Eigen::MatrixXd b3;
    Eigen::MatrixXd b4;
}ImagerThermalOutMsgPayload;

#endif
