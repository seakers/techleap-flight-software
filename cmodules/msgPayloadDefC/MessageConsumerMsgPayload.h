//
// Created by gabe :)
//

#ifndef MESSAGE_CONSUMER_MSG_H
#define MESSAGE_CONSUMER_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int ins_state;
    double lat;
    double lon;
    double alt;
    double yaw;
    double pitch;
    double roll;
}MessageConsumerMsgPayload;

#endif
