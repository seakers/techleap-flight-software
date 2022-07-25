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

#include <Eigen/Dense>

#include "architecture/_GeneralModuleFiles/sys_model.h"
#include "architecture/utilities/bskLogging.h"
#include "architecture/messaging/messaging.h"

// ---------------------------
// ----- MESSAGE IMPORTS -----
// ---------------------------
#include "msgPayloadDefC/ControllerModeMsgPayload.h"
#include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"
#include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"
#include "msgPayloadDefC/FinePredictionMsgPayload.h"
#include "msgPayloadDefC/IMUOutMsgPayload.h"


class PID {
    public:
        int Kp;
        int Ki;
        int Kd;
        float currTime;
        float prevTime;
        float prevErr;
        float Ci;
    float pidUpdate(float error, uint64_t CurrentSimNanos);
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
    void PanMotorSetup(int port, int motorID);
    void TiltMotorSetup(int port, int motorID);
    void MoveBySteps(int port, int motorID, int nSteps);
    int DegToSteps(int degrees, float gearRatio);
    float LimitAngle(float angle, float upperLimit, float lowerLimit, float imuAngle);
    void ScanPattern();
    void TiltScan();

public:

    int state;
    float desiredPanAngle;
    float desiredTiltAngle;
    PID panPID;
    PID tiltPID;
    std::vector<std::string> motorVector;
    std::vector<int> degreeVector;
    int count;

    // ----------------------
    // ----- MESSAGE IN -----
    // ----------------------

    ReadFunctor<ControllerModeMsgPayload> mode_msg;
    int mode;

    ReadFunctor<AttitudeDeterminationAnglesMsgPayload> adcs_angles_msg;
    int   adcs_state;
    float adcs_yaw;
    float adcs_pitch;
    float adcs_roll;

    ReadFunctor<ControllerManualAnglesMsgPayload> cont_angles_msg;
    int   cont_state;
    float cont_pan;
    float cont_tilt;

    ReadFunctor<IMUOutMsgPayload> imu_msg;
    int   imu_state;
    float imu_yaw;
    float imu_pitch;
    float imu_roll;

    ReadFunctor<FinePredictionMsgPayload> fine_msg;
    int fine_state;
    float fine_pan;
    float fine_tilt;
    Eigen::MatrixXd fine_mask;

    // --> LOGGING
    BSKLogger bskLogger;
};


#endif
