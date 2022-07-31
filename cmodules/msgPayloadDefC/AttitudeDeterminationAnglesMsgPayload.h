//
// Created by gabe :)
//

#ifndef ATTITUDE_DETERMINATION_ANGLES_MSG_H
#define ATTITUDE_DETERMINATION_ANGLES_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    double yaw;
    double pitch;
    double roll;
}AttitudeDeterminationAnglesMsgPayload;

#endif
