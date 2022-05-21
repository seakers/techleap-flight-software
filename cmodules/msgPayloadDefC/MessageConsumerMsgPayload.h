//
// Created by gabe :)
//

#ifndef MESSAGE_CONSUMER_MSG_H
#define MESSAGE_CONSUMER_MSG_H

/*! @brief Brief description of what this message contains */
typedef struct {
    int state;
    int msg;                  // 0 if no plume detected | 1 if plume detected
}MessageConsumerMsgPayload;

#endif
