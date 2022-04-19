//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_OUT_H
#define IMAGER_VNIR_OUT_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;                  // --> State of sensor
    int imageTensor[20][20];    // --> Image captured
}ImagerVNIROutMsgPayload;

#endif
