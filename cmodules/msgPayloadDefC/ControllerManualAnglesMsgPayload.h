//
// Created by gabe :)
//

#ifndef CONTROLLER_MANUAL_ANGLES_MSG_H
#define CONTROLLER_MANUAL_ANGLES_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    double yaw;
    double pitch;                  // 0 if no plume detected | 1 if plume detected
    double roll;
}ControllerManualAnglesMsgPayload;

#endif
