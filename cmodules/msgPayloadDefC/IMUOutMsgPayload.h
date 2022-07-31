//
// Created by gabe :)
//

#ifndef IMU_OUT_H
#define IMU_OUT_H

#include <string>

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;                  // --> State of sensor
    std::string status_msg;
    double yaw;
    double pitch;
    double roll;
    double temperature;
}IMUOutMsgPayload;

#endif
