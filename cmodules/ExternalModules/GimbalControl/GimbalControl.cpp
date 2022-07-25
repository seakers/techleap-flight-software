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
    this->fine_state = -1;
    this->fine_pan = 0;
    this->fine_tilt = 0;
    this->mode = -1;
    this->count = 0;
}

GimbalControl::~GimbalControl()
{
    return;
}

void GimbalControl::ReadMessages(){
    if(this->mode_msg.isLinked()){
        std::cout << "Mode message linked!" << std::endl;
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
        std::cout << this->mode << std::endl;
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
    if(this->fine_msg.isLinked()){
        std::cout << "Fine angles LINKED!" << std::endl;
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        this->fine_pan = fine_msg_payload.pan;
        this->fine_tilt = fine_msg_payload.tilt;
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
    this->panPID.currTime = (float) CurrentSimNanos/(1e9);
    this->panPID.prevTime = this->panPID.currTime;
    this->panPID.prevErr = 0;

    this->tiltPID.Kp = 1;
    this->tiltPID.Ki = 0;
    this->tiltPID.Kd = 0;
    this->tiltPID.Ci = 0;
    this->tiltPID.currTime = (float) CurrentSimNanos/(1e9);
    this->tiltPID.prevTime = this->tiltPID.currTime;
    this->tiltPID.prevErr = 0;

    int pan = open("/dev/ttyClientP", O_RDWR);
    int tilt = open("/dev/ttyClientT", O_RDWR);

    this->count = 0;
    this->InitializePort(pan,1);
    this->InitializePort(tilt,2);
    this->PanMotorSetup(pan,1);
    this->TiltMotorSetup(tilt,2);
    this->ScanPattern();
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

    if(this->mode == 2) {
        std::cout << "Scanning!" << std::endl;
        if(motorVector.at(count) == "pan") {
            this->MoveBySteps(pan,1,degreeVector.at(count));
        }
        if(motorVector.at(count) == "tilt") {
            this->MoveBySteps(tilt,2,degreeVector.at(count));
        }
        this->count = count + 1;
        if(this->count > 90) {
            this->count = 0;
        }
    }
    if(this->mode == 1 && this->fine_msg.isLinked()) {
        std::cout << "Tracking plume!" << std::endl;
        float panAngle = this->tiltPID.pidUpdate(this->fine_pan, CurrentSimNanos);
        panAngle = this->LimitAngle(panAngle, 42.5, 12.5, this->imu_pitch);
        this->MoveBySteps(pan,1,this->DegToSteps(panAngle,1));
        float tiltAngle = this->tiltPID.pidUpdate(this->fine_tilt, CurrentSimNanos);
        tiltAngle = this->LimitAngle(tiltAngle, -50, -110, this->imu_pitch);
        this->MoveBySteps(tilt,2,this->DegToSteps(tiltAngle,3.5866));
    }
    if(this->mode == 3 && this->fine_msg.isLinked()) {
        std::cout << "Moving manually!" << std::endl;
        float panAngle = this->tiltPID.pidUpdate(this->cont_pan, CurrentSimNanos);
        panAngle = this->LimitAngle(panAngle, 42.5, 12.5, this->imu_pitch);
        this->MoveBySteps(pan,1,this->DegToSteps(panAngle,1));
        float tiltAngle = this->tiltPID.pidUpdate(this->cont_tilt, CurrentSimNanos);
        tiltAngle = this->LimitAngle(tiltAngle, -50, -110, this->imu_pitch);
        this->MoveBySteps(tilt,2,this->DegToSteps(tiltAngle,3.5866));
    }

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ ran update at %fs", this->moduleID, (float) CurrentSimNanos/(1e9));
}

void GimbalControl::PanMotorSetup(int port, int motorID) {
    char set_reset_msg[] = {"RE\r"};
    this->SendCommand(port, motorID, set_reset_msg);
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

void GimbalControl::TiltMotorSetup(int port, int motorID) {
    char set_reset_msg[] = {"RE\r"};
    this->SendCommand(port, motorID, set_reset_msg);
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
    char velocity_msg[] = { "VE0.2\r" };
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

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
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
    //std::cout << finalCommand << std::endl;
    write(port, finalCommand, sizeof(finalCommand));
    usleep(100000);
}

void GimbalControl::MoveBySteps(int port, int motorID, int nSteps) {
    int length = 0;
    if(nSteps < 0) {
        length = 4;
    } else {
        length = 3;
    }
    char moveCommand[4+length];
    char* fp = "FL";
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

void GimbalControl::ScanPattern() {
    this->TiltScan();

    for (int i = 0; i < 15; i++) {
        this->motorVector.push_back("pan");
        this->degreeVector.push_back(this->DegToSteps(-1,1));
    }

    this->TiltScan();

    for (int i = 0; i < 30; i++) {
        this->motorVector.push_back("pan");
        this->degreeVector.push_back(this->DegToSteps(1,1));
    }

    for (int i = 0; i < 15; i++) {
        this->motorVector.push_back("pan");
        this->degreeVector.push_back(this->DegToSteps(-1,1));
    }
}

void GimbalControl::TiltScan() {
    for (int i = 0; i < 15; i++) {
        this->motorVector.push_back("tilt");
        this->degreeVector.push_back(this->DegToSteps(1,3.5866));
    }
    for (int i = 0; i < 30; i++) {
        this->motorVector.push_back("tilt");
        this->degreeVector.push_back(this->DegToSteps(-1,3.5866));
    }
    for (int i = 0; i < 15; i++) {
        this->motorVector.push_back("tilt");
        this->degreeVector.push_back(this->DegToSteps(1,3.5866));
    }
}

/*void GimbalControl::ReadIMU() {
 VnSensor vs;
 const std::string SensorPort = "/dev/ttyUSB1";
 const uint32_t SensorBaudrate = 115200;
 vs.connect(SensorPort, SensorBaudrate);
 // --> Read yaw pitch roll
 vec3f ypr = vs.readYawPitchRoll();
 std::cout << ypr << std::endl;
}*/

int GimbalControl::DegToSteps(int degrees, float gearRatio) {
    int stepsPerRev = 51200;
    int oneRev = 360;
    float prod = degrees * gearRatio * stepsPerRev / oneRev;
    return (int)prod;
}

float PID::pidUpdate(float error, uint64_t CurrentSimNanos) {
    this->currTime = (float) CurrentSimNanos/(1e9);
    float dt = this->currTime - this->prevTime;
    float de = error - this->prevErr;
    float Cp = error;
    this->Ci += error * dt;
    float Cd = 0;
    if(dt > 0) {
        Cd = de/dt;
    }
    this->prevTime = this->currTime;
    this->prevErr = error;
    return Cp * this->Kp + Ci * this->Ki + Cd * this->Kd;
}

float GimbalControl::LimitAngle(float angle, float upperLimit, float lowerLimit, float imuAngle) {
    if (angle+imuAngle > upperLimit) {
        angle = upperLimit-imuAngle;
    }
    if (angle+imuAngle < lowerLimit) {
        angle = lowerLimit-imuAngle;
    }
    return angle;
}