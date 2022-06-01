//
// Created by gabe :)
//

#ifndef IMAGER_VNIR_OUT_H
#define IMAGER_VNIR_OUT_H



/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    double red[3200][3200];
    double green[3200][3200];
    double blue[3200][3200];
}ImagerVNIROutMsgPayload;

#endif
