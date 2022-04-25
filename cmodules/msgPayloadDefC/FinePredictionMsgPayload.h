//
// Created by gabe :)
//

#ifndef FINE_PREDICTION_H
#define FINE_PREDICTION_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    int mask[20][20]; // 0 if pixel is not in plume | 1 if pixel is in plume
}FinePredictionMsgPayload;

#endif
