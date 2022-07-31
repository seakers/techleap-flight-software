//
// Created by gabe :)
//

#ifndef CONTROLLER_MANUAL_MSG_H
#define CONTROLLER_MANUAL_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    double manual_lat;
    double manual_lon;
    double manual_alt;                  // 0 if no plume detected | 1 if plume detected
}ControllerManualMsgPayload;

#endif
