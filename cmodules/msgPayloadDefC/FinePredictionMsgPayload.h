//
// Created by gabe :)
//

#ifndef FINE_PREDICTION_H
#define FINE_PREDICTION_H

#include <Eigen/Dense>

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    float pan;
    float tilt;
    Eigen::MatrixXd mask;
}FinePredictionMsgPayload;

#endif
