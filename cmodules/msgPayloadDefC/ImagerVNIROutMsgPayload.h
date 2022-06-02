//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_OUT_H
#define IMAGER_VNIR_OUT_H



/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    double red[512][512];
    double green[512][512];
    double blue[512][512];
}ImagerVNIROutMsgPayload;

#endif
