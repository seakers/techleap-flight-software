//
// Created by gabe :)
//

#ifndef INERTIAL_MEASUREMENT_UNIT_OUT_H
#define INERTIAL_MEASUREMENT_UNIT_OUT_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;                  // --> State of sensor
    float yaw;
    float pitch;                  // 0 if no plume detected | 1 if plume detected
    float roll;
}InertialMeasurementUnitOutMsgPayload;

#endif
