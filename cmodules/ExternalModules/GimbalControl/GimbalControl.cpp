//
// Created by gabe :)
//

#include "ExternalModules/GimbalControl/GimbalControl.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

#define PI 3.14159265


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
        //std::cout << "Mode message linked!" << std::endl;
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
        //std::cout << this->mode << std::endl;
    }
    if(this->ins_msg.isLinked()){
        MessageConsumerMsgPayload ins_msg_payload = this->ins_msg();
        this->ins_state = ins_msg_payload.ins_state;
        this->ins_yaw = ins_msg_payload.yaw;
        this->ins_pitch = ins_msg_payload.pitch;
        this->ins_roll = ins_msg_payload.roll;
        this->ins_lat = ins_msg_payload.lat;
        this->ins_lon = ins_msg_payload.lon;
        this->ins_alt = ins_msg_payload.alt;
    }
    if(this->manual_msg.isLinked()){
        MessageConsumerManualMsgPayload manual_msg_payload = this->manual_msg();
        this->manual_plume = manual_msg_payload.manual_plume;
        this->manual_lat = manual_msg_payload.manual_lat;
        this->manual_lon = manual_msg_payload.manual_lon;
        this->manual_alt = manual_msg_payload.manual_alt;
    }
    if(this->imu_msg.isLinked()){
        //std::cout << "IMU ANGLES LINKED!" << std::endl;
        IMUOutMsgPayload imu_msg_payload = this->imu_msg();
        this->imu_state = imu_msg_payload.state;
        this->imu_yaw = imu_msg_payload.yaw;
        this->imu_pitch = imu_msg_payload.pitch;
        this->imu_roll = imu_msg_payload.roll;
    }
    if(this->fine_msg.isLinked()){
        //std::cout << "Fine angles LINKED!" << std::endl;
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
    this->panPID.Kp = 0.1;
    this->panPID.Ki = 0;
    this->panPID.Kd = 0.1;
    this->panPID.Ci = 0;
    this->panPID.currTime = (float) CurrentSimNanos/(1e9);
    this->panPID.prevTime = this->panPID.currTime;
    this->panPID.prevErr = 0;

    this->tiltPID.Kp = 0.1;
    this->tiltPID.Ki = 0;
    this->tiltPID.Kd = 0;
    this->tiltPID.Ci = 0;
    this->tiltPID.currTime = (float) CurrentSimNanos/(1e9);
    this->tiltPID.prevTime = this->tiltPID.currTime;
    this->tiltPID.prevErr = 0;

    int pan = open("/dev/ttyClientP", O_RDWR);
    int tilt = open("/dev/ttyClientT", O_RDWR);

    this->count = 0;
    this->lastMode = 0;
    this->InitializePort(pan,1);
    this->InitializePort(tilt,2);
    this->PanMotorSetup(pan,1);
    this->TiltMotorSetup(tilt,2);
    this->ScanPattern();
    this->resetAfterPlume = false;
    this->movedToPlume = false;
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
    if(this->lastMode == 1 && this->mode == 2) {
        std::cout << "Resetting after track" << std::endl;
        this->ResetPosition(pan,tilt);
    }
    if(this->mode == 2) {
        std::cout << "Scanning!" << std::endl;
        if(motorVector.at(count) == "pan") {
            this->MoveBySteps(pan,1,degreeVector.at(count));
        }
        if(motorVector.at(count) == "tilt") {
            this->MoveBySteps(tilt,2,degreeVector.at(count));
        }
        this->count = count + 1;
        if(this->count > degreeVector.size()) {
            this->ResetPosition(pan,tilt);
            this->count = 0;
        }
        this->lastMode = 2;
    }
    if(this->mode == 1) {
        std::cout << "Tracking plume!" << std::endl;
        std::cout << "Fine pan: " << this->fine_pan << std::endl;
        float panAngle = this->panPID.pidUpdate(this->fine_pan, CurrentSimNanos);
        //panAngle = this->LimitAngle(panAngle, 42.5, 12.5, this->imu_pitch);
        std::cout << "panPID output: " << panAngle << std::endl;
        this->MoveBySteps(pan,1,this->DegToSteps(panAngle,1));
        std::cout << "Fine tilt: " << this->fine_tilt << std::endl;
        float tiltAngle = this->tiltPID.pidUpdate(this->fine_tilt, CurrentSimNanos);
        std::cout << "tiltPID output: " << tiltAngle << std::endl;
        //tiltAngle = this->LimitAngle(tiltAngle, -50, -110, this->imu_pitch);
        this->MoveBySteps(tilt,2,this->DegToSteps(tiltAngle,1.5));
        this->lastMode = 1;
    }
    if(this->mode == 3) {
        if(this->movedToPlume == false) {
            std::cout << "Moving manually!" << std::endl;
            std::vector<double> angles = this->GetManualAngles();
            //float panAngle = this->LimitAngle((float)angles.at(0), 42.5, 12.5, this->imu_pitch);
            this->MoveBySteps(pan,1,this->DegToSteps((float)angles.at(0)-112.0,1));
            //float tiltAngle = this->tiltPID.pidUpdate((float)angles.at(1), CurrentSimNanos);
            //tiltAngle = this->LimitAngle(tiltAngle, -50, -110, this->imu_pitch);
            this->MoveBySteps(tilt,2,this->DegToSteps((float)angles.at(1),1.5));
            this->movedToPlume = true;
        }
        this->lastMode = 3;
    }
    if(this->mode == 0) {
        std::cout << "Not moving!" << std::endl;
        this->lastMode = 0;
    }
    std::cout << "Mode in GC: " << this->mode << std::endl;
    std::cout << "resetAfterPlume in GC: " << this->resetAfterPlume << std::endl;
    if(this->mode != 3 && this->resetAfterPlume == false && this->movedToPlume == true) {
        std::cout << "Resetting after plume" << std::endl;
        this->ResetPosition(pan,tilt);
        this->resetAfterPlume = true;
    }


    this->CheckTiltLimits(tilt);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "GimbalControl ------ ran update at %fs", this->moduleID, (float) CurrentSimNanos/(1e9));
}

void GimbalControl::CheckTiltLimits(int tiltPort) {
    if(abs(this->imu_pitch) > 45.0) {
        this->MoveBySteps(tiltPort,2,this->DegToSteps(-this->imu_pitch,2.0));
    }
}

void GimbalControl::ResetPosition(int panPort, int tiltPort) {
    std::cout << "Resetting position!" << std::endl;
    this->MoveBySteps(tiltPort,2,this->DegToSteps(-this->imu_pitch,2.0));
    this->ResetPanPosition(panPort,1);
    //this->MoveBySteps(panPort,1,this->DegToSteps(-(this->imu_yaw-112.0),1.0));
}

std::vector<double> GimbalControl::GetManualAngles(){
    std::vector<double> angles;
    std::vector<double> plume_geo;
    std::vector<double> balloon_geo;
    plume_geo.push_back(this->manual_lat*PI/180);
    plume_geo.push_back(this->manual_lon*PI/180);
    plume_geo.push_back(this->manual_alt);
    std::cout << "Starting geo to ecef" << std::endl;
    std::vector<double> plume_ecef = this->geo_to_ecef(plume_geo);
    balloon_geo.push_back(this->ins_lat*PI/180);
    balloon_geo.push_back(this->ins_lon*PI/180);
    balloon_geo.push_back(this->ins_alt);
    std::vector<double> balloon_ecef = this->geo_to_ecef(balloon_geo);
    std::cout << "Starting ecef to ned" << std::endl;
    std::vector<double> plume_ned = this->ecef_to_ned(plume_ecef,balloon_ecef,balloon_geo);
    std::cout << "Starting ned to pan/tilt" << std::endl;
    double phi = atan2(plume_ned.at(1),plume_ned.at(0)) * 180 / PI;
    double theta = atan2(hypot(plume_ned.at(0),plume_ned.at(1)),plume_ned.at(2)) * 180 / PI;
    angles.push_back(phi);
    angles.push_back(theta);
    std::cout << "Manual angles: " << phi << ", " << theta << std::endl;
    return angles;
}

std::vector<double> GimbalControl::geo_to_ecef( std::vector<double> geo ) {
    std::vector<double> ecef;  //Results go here (x, y, z)
    double lat = geo[0];
    double lon = geo[1];
    double alt = geo[2];
    double a = 6378137.0;
    double e2 = 6.6943799901377997e-3;
    double n = a/sqrt( 1 - e2*sin( lat )*sin( lat ) );
    ecef.push_back(( n + alt )*cos( lat )*cos( lon ));    //ECEF x
    ecef.push_back(( n + alt )*cos( lat )*sin( lon ));    //ECEF y
    ecef.push_back(( n*(1 - e2 ) + alt )*sin( lat ));          //ECEF z
    return( ecef );     //Return x, y, z in ECEF
}

 std::vector<double> GimbalControl::ecef_to_ned( std::vector<double> point,  std::vector<double> ref,  std::vector<double> ref_geo ) {
    std::vector<double> enu;  //Results go here (x, y, z)
    std::vector<double> ned;
    double x_diff = 0.0;
    double y_diff = 0.0;
    double z_diff = 0.0;
    x_diff = point.at(0) - ref.at(0);
    y_diff = point.at(1) - ref.at(1);
    z_diff = point.at(2) - ref.at(2);
    enu.push_back(-sin(ref_geo.at(1))*x_diff + cos(ref_geo.at(1))*y_diff);
    enu.push_back(sin(ref_geo.at(0))*cos(ref_geo.at(1))*x_diff + -sin(ref_geo.at(0))*sin(ref_geo.at(1))*y_diff + cos(ref_geo.at(0))*z_diff);
    enu.push_back(cos(ref_geo.at(0))*cos(ref_geo.at(1))*x_diff + cos(ref_geo.at(0))*sin(ref_geo.at(1))*y_diff + sin(ref_geo.at(0))*z_diff);
    ned.push_back(enu.at(1));
    ned.push_back(enu.at(0));
    ned.push_back(-enu.at(2));
    return( ned );     //Return x, y, z in NED
}

void GimbalControl::PanMotorSetup(int port, int motorID) {
    char set_reset_msg[] = {"RE\r"};
    this->SendCommand(port, motorID, set_reset_msg);
    usleep(1000000);
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
    usleep(1000000);
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
    std::cout << finalCommand << std::endl;
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
}

void GimbalControl::ResetPanPosition(int port, int motorID) {
    int length = 0;
    int nSteps = 0;
    if(nSteps < 0) {
        length = 4;
    } else {
        length = 3;
    }
    char moveCommand[4+length];
    char* fp = "FP";
    char* r = "\r";
    std::stringstream ss;
    ss << nSteps;
    std::string stepsStr = ss.str();
    strcpy(moveCommand,fp);
    strcat(moveCommand,stepsStr.c_str());
    strcat(moveCommand,r);
    this->SendCommand(port, motorID, moveCommand);
}

void GimbalControl::ScanPattern() {
    this->TiltScan();

    for (int i = 0; i < 15; i=i+5) {
        this->motorVector.push_back("pan");
        this->degreeVector.push_back(this->DegToSteps(-5,1));
    }

    this->TiltScan();

    for (int i = 0; i < 30; i=i+5) {
        this->motorVector.push_back("pan");
        this->degreeVector.push_back(this->DegToSteps(5,1));
    }

    this->TiltScan();

    for (int i = 0; i < 15; i=i+5) {
        this->motorVector.push_back("pan");
        this->degreeVector.push_back(this->DegToSteps(-5,1));
    }
}

void GimbalControl::TiltScan() {
    for (int i = 0; i < 15; i++) {
        this->motorVector.push_back("tilt");
        this->degreeVector.push_back(this->DegToSteps(1,1.5));
    }
    for (int i = 0; i < 30; i++) {
        this->motorVector.push_back("tilt");
        this->degreeVector.push_back(this->DegToSteps(-1,1.5));
    }
    for (int i = 0; i < 15; i++) {
        this->motorVector.push_back("tilt");
        this->degreeVector.push_back(this->DegToSteps(1,1.5));
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

int GimbalControl::DegToSteps(float degrees, float gearRatio) {
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