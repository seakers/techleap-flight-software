//
// Created by gabe :)
//

#ifndef INERTIAL_MEASUREMENT_UNIT_OUT_H
#define INERTIAL_MEASUREMENT_UNIT_OUT_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;                  // --> State of sensor
    int angles[3];
}InertialMeasurementUnitOutMsgPayload;

#endif
