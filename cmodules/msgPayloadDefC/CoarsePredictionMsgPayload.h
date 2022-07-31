//
// Created by gabe :)
//

#ifndef COARSE_PREDICTION_H
#define COARSE_PREDICTION_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    int prediction;                  // 0 if no plume detected | 1 if plume detected
}CoarsePredictionMsgPayload;

#endif
