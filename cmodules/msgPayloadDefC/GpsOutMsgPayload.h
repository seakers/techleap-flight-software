//
// Created by gabe :)
//

#ifndef GPS_OUT_MSG_H
#define GPS_OUT_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    float lat;
    float lon;                  // 0 if no plume detected | 1 if plume detected
    float altitude;
}GpsOutMsgPayload;

#endif
