//
// Created by gabe :)
//

#ifndef GIMBAL_CONTROL_H
#define GIMBAL_CONTROL_H

#include <string>

#include<iostream>
#include<fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include<fstream>
#include<string>
#include <stdio.h>
#include <string.h>
#include <sstream>

#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"
#include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"
#include "msgPayloadDefC/IMUOutMsgPayload.h"

class PID {
    public:
        int Kp;
        int Ki;
        int Kd;
        double currTime;
        double prevTime;
        double prevErr;
        double Ci;
    double pidUpdate(double error, uint64_t CurrentSimNanos);
};

/*! @brief basic Basilisk C++ module class */
class GimbalControl: public SysModel {
public:
    GimbalControl();
    ~GimbalControl();

    void Reset(uint64_t CurrentSimNanos);
    void UpdateState(uint64_t CurrentSimNanos);

    void ReadMessages();
    void InitializePort(int serial_port, int motorID);
    void SendCommand(int port, int motorID, char command[]);
    void MotorSetup(int port, int motorID);
    void MoveBySteps(int port, int motorID, int nSteps);
    int DegToSteps(int degrees);
    double LimitAngle(double angle, double upperLimit, double lowerLimit, double imuAngle);

public:

    int state;
    double desiredPanAngle;
    double desiredTiltAngle;
    PID panPID;
    PID tiltPID;

    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<AttitudeDeterminationAnglesMsgPayload> adcs_angles_msg;
    int   adcs_state;
    double adcs_yaw;
    double adcs_pitch;
    double adcs_roll;

    ReadFunctor<ControllerManualAnglesMsgPayload> cont_angles_msg;
    int   cont_state;
    double cont_pan;
    double cont_tilt;

    ReadFunctor<IMUOutMsgPayload> imu_msg;
    int   imu_state;
    double imu_yaw;
    double imu_pitch;
    double imu_roll;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
