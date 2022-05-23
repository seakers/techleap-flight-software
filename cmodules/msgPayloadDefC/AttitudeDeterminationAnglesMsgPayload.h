//
// Created by gabe :)
//

#ifndef ATTITUDE_DETERMINATION_ANGLES_MSG_H
#define ATTITUDE_DETERMINATION_ANGLES_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    float yaw;
    float pitch;
    float roll;
}AttitudeDeterminationAnglesMsgPayload;

#endif
