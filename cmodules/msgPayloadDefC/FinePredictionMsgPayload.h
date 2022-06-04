//
// Created by gabe :)
//

#ifndef FINE_PREDICTION_H
#define FINE_PREDICTION_H

#include <Eigen/Dense>

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    Eigen::MatrixXd mask;
}FinePredictionMsgPayload;

#endif
