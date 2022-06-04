//
// Created by gabe :)
//

#ifndef THERMAL_MSG_H
#define THERMAL_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;                  // --> State of sensor
    double temps[4];
}ThermalMsgPayload;

#endif
