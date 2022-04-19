//
// Created by gabe :)
//

#ifndef IMAGER_THERMAL_OUT_H
#define IMAGER_THERMAL_OUT_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;                  // --> State of sensor
    int imageTensor[20][20];    // --> Image captured
}ImagerThermalOutMsgPayload;

#endif
