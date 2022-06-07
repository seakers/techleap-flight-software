//
// Created by gabe :)
//

#include "ExternalModules/GimbalControl/GimbalControl.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



GimbalControl::GimbalControl()
{
    this->state = 0;
    this->desiredPanAngle = 27.5;
    this->desiredTiltAngle = -80;
}

GimbalControl::~GimbalControl()
{
    return;
}

void GimbalControl::ReadMessages(){
    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    if(this->adcs_angles_msg.isLinked()){
        AttitudeDeterminationAnglesMsgPayload adcs_angles_msg_payload = this->adcs_angles_msg();
        this->adcs_state = adcs_angles_msg_payload.state;
        this->adcs_yaw = adcs_angles_msg_payload.yaw;
        this->adcs_pitch = adcs_angles_msg_payload.pitch;
        this->adcs_roll = adcs_angles_msg_payload.roll;
    }

    if(this->cont_angles_msg.isLinked()){
        ControllerManualAnglesMsgPayload cont_angles_msg_payload = this->cont_angles_msg();
        this->cont_state = cont_angles_msg_payload.state;
        this->cont_pan = cont_angles_msg_payload.pan;
        this->cont_tilt = cont_angles_msg_payload.tilt;
    }
    if(this->imu_msg.isLinked()){
        std::cout << "IMU ANGLES LINKED!" << std::endl;
        IMUOutMsgPayload imu_msg_payload = this->imu_msg();
        this->imu_state = imu_msg_payload.state;
        this->imu_yaw = imu_msg_payload.yaw;
        this->imu_pitch = imu_msg_payload.pitch;
        this->imu_roll = imu_msg_payload.roll;
    }
}




void GimbalControl::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ (reset)");

    // --> 1. Reset module state
    this->state = 0;
    this->panPID.Kp = 0.9;
    this->panPID.Ki = 0;
    this->panPID.Kd = 0.1;
    this->panPID.Ci = 0;
    this->panPID.currTime = (double) CurrentSimNanos/(1e9);
    this->panPID.prevTime = this->panPID.currTime;
    this->panPID.prevErr = 0;

    this->tiltPID.Kp = 1;
    this->tiltPID.Ki = 0;
    this->tiltPID.Kd = 0;
    this->tiltPID.Ci = 0;
    this->tiltPID.currTime = (double) CurrentSimNanos/(1e9);
    this->tiltPID.prevTime = this->tiltPID.currTime;
    this->tiltPID.prevErr = 0;
}



void GimbalControl::UpdateState(uint64_t CurrentSimNanos)
{
    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages();

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    int pan = open("/dev/ttyClientP", O_RDWR);
    int tilt = open("/dev/ttyClientT", O_RDWR);
    this->InitializePort(pan,1);
    this->InitializePort(tilt,2);
    this->MotorSetup(pan,1);
    this->MotorSetup(tilt,2);
    
    if(this->imu_msg.isLinked()) {
        std::cout << "attempting PID" << std::endl;
        double tiltAngle = this->tiltPID.pidUpdate(this->desiredTiltAngle - this->imu_pitch, CurrentSimNanos);
        tiltAngle = this->LimitAngle(tiltAngle, -50, -110, this->imu_pitch);
        this->MoveBySteps(tilt,2,DegToSteps(tiltAngle));
        double panAngle = this->panPID.pidUpdate(this->desiredPanAngle - this->imu_yaw, CurrentSimNanos);
        panAngle = this->LimitAngle(panAngle, 42.5, 12.5, this->imu_yaw);
        this->MoveBySteps(pan,1,DegToSteps(panAngle));
    } else {
        std::cout << "moving manually" << std::endl;
        this->MoveBySteps(pan,1,DegToSteps(-10));
        this->MoveBySteps(tilt,2,DegToSteps(-10));
    }
    

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

void GimbalControl::MotorSetup(int port, int motorID) {
        char set_steps_msg[] = { "EG51200\r" };
        this->SendCommand(port, motorID, set_steps_msg);
        char stall_detect_msg[] = { "EF1\r" };
        this->SendCommand(port, motorID, stall_detect_msg);
        char stall_prevent_msg[] = { "EF2\r" };
        this->SendCommand(port, motorID, stall_prevent_msg);
        char drive_response_msg[] = { "IFD\r" };
        this->SendCommand(port, motorID, drive_response_msg);
        char start_pos_msg[] = { "SP0\r" };
        this->SendCommand(port, motorID, start_pos_msg);
        char alarm_reset_msg[] = { "AR\r" };
        this->SendCommand(port, motorID, alarm_reset_msg);
        char accel_msg[] = { "AC0.168\r" };
        this->SendCommand(port, motorID, accel_msg);
        char decel_msg[] = { "DE0.168\r" };
        this->SendCommand(port, motorID, decel_msg);
        char velocity_msg[] = { "VE0.01\r" };
        this->SendCommand(port, motorID, velocity_msg);
        char enable_msg[] = { "ME\r" };
        this->SendCommand(port, motorID, enable_msg);
        char immediate_pos_msg[] = { "IP\r" };
        this->SendCommand(port, motorID, immediate_pos_msg);
}

void GimbalControl::InitializePort(int serial_port, int motorID) {
    struct termios tty;
    if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
    
    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;
    
    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    
    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    char mt_msg[] = { "MT1\r" };
    this->SendCommand(serial_port,motorID,mt_msg);
}

void GimbalControl::SendCommand(int port, int motorID, char command[]) {
    char finalCommand[1+strlen(command)];
    std::stringstream ss;
    ss << motorID;
    std::string motorStr = ss.str();
    strcpy(finalCommand,motorStr.c_str());
    strcat(finalCommand,command);
    std::cout << finalCommand << std::endl;
    write(port, finalCommand, sizeof(finalCommand));
    usleep(100000);
}

void GimbalControl::MoveBySteps(int port, int motorID, int nSteps) {
    char moveCommand[10];
    char* fp = "FP";
    char* r = "\r";
    std::stringstream ss;
    ss << nSteps;
    std::string stepsStr = ss.str();
    strcpy(moveCommand,fp);
    strcat(moveCommand,stepsStr.c_str());
    strcat(moveCommand,r);
    this->SendCommand(port, motorID, moveCommand);
    //char read_buf [256];
    //memset(&read_buf, '\0', sizeof(read_buf));
    //char ip_msg[] = { "IP\r" };
    //this->SendCommand(port, motorID, ip_msg);
    //int num_bytes = read(port, &read_buf, sizeof(read_buf));
    //printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
}

int GimbalControl::DegToSteps(int degrees) {
    int stepsPerRev = 51200;
    int oneRev = 360;
    int gearRatio = 1;
    return degrees * gearRatio * stepsPerRev / oneRev;
}

double PID::pidUpdate(double error, uint64_t CurrentSimNanos) {
    this->currTime = (double) CurrentSimNanos/(1e9);
    double dt = this->currTime - this->prevTime;
    double de = error - this->prevErr;
    double Cp = error;
    this->Ci += error * dt;
    double Cd = 0;
    if(dt > 0) {
        Cd = de/dt;
    }
    this->prevTime = this->currTime;
    this->prevErr = error;
    return Cp * this->Kp + Ci * this->Ki + Cd * this->Kd;
}

double GimbalControl::LimitAngle(double angle, double upperLimit, double lowerLimit, double imuAngle) {
    if (angle+imuAngle > upperLimit) {
        angle = upperLimit-imuAngle;
    }
    if (angle+imuAngle < lowerLimit) {
        angle = lowerLimit-imuAngle;
    }
    return angle;
}