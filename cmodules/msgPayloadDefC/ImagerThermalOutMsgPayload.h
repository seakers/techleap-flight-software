//
// Created by gabe :)
//

#ifndef IMAGER_THERMAL_OUT_H
#define IMAGER_THERMAL_OUT_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    double b1[3200][3200];
    double b2[3200][3200];
    double b3[3200][3200];
    double b4[3200][3200];
}ImagerThermalOutMsgPayload;

#endif
