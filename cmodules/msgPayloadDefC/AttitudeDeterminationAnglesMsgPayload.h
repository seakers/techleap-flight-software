//
// Created by gabe :)
//

#ifndef ATTITUDE_DETERMINATION_ANGLES_MSG_H
#define ATTITUDE_DETERMINATION_ANGLES_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    float yaw;
    float pitch;                  // 0 if no plume detected | 1 if plume detected
    float roll;
}AttitudeDeterminationAnglesMsgPayload;

#endif
