//
// Created by gabe :) and ben :P
//

#include "ExternalModules/Thermal/Thermal.h" // --> CHANGE
#include <iostream>
#include <memory>
#include <cstring>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

#include<fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include<fstream>
#include<string>
#include <stdio.h>
#include <string.h>
using namespace std;

void initialize_port(int serial_port);



Thermal::Thermal() {
    this->state = 0;
}

Thermal::~Thermal() {
    return;
}

void Thermal::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "Thermal -------- (reset)");

    // --> 1. Reset module state
    this->state = 0;
}

void Thermal::UpdateState(uint64_t CurrentSimNanos){
    // -----------------------
    // ----- Zero Output -----
    // -----------------------
    ThermalMsgPayload thermal_msg_buffer = this->thermal_msg.zeroMsgPayload;
    this->ZeroOutputVariables();

    // --------------------------
    // ----- Read Inputs -----
    // --------------------------
    // --> TODO: Manual heater control

    int teensy = open("/dev/ttyUSB0", O_RDWR); //TODO figure out actual port
    initialize_port(teensy);

    unsigned char read_teensy[] = { "GET TEMPS\r" };

    while(count<100) {
        write(teensy, read_teensy, sizeof(read_teensy));
        char read_buf [256];
        memset(&read_buf, '\0', sizeof(read_buf));
        int num_bytes = read(teensy, &read_buf, sizeof(read_buf));
        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes < 0) {
          printf("Error reading: %s", strerror(errno));
          return 1;
        }
        for(int i; i < 4; i++) {
          this->temps[i] = 0.0;
        }

        // TODO figure out read_buf to temps
        
        // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
        // print it to the screen like this!)
        printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
        cout << endl;
        count = count + 1;
    }

    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    thermal_msg_buffer.state = this->state;
    for(int i; i < 4; i++)
    {
      thermal_msg_buffer.temps[i] = this->temps[i];
    }
    
    this->thermal_msg.write(&thermal_msg_buffer, this->moduleID, CurrentSimNanos);

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "Thermal -------- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}


void initialize_port(int serial_port) {
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
    unsigned char mt_msg[] = { "MT1\r" };
    write(serial_port,mt_msg,sizeof(mt_msg));
}