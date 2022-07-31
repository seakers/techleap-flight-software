//
// Created by gabe :)
//

#include "ExternalModules/DataStorage/DataStorage.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include <fstream>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"
#include <memory>
#include<fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include<fstream>
#include<string>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <bits/stdc++.h>

DataStorage::DataStorage() // --> CHANGE
{
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
    this->fine_mask.setZero(512, 512);
}

DataStorage::~DataStorage() // --> CHANGE
{
    return;
}

void DataStorage::ReadMessages(uint64_t SimNanos){
    std::ofstream telemetryFile;
    telemetryFile.open("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/telemetry"+std::to_string((double) SimNanos/(1e9))+".txt");

    // --> VNIR Reading
    if(this->vnir_msg.isLinked()){
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->red = vnir_msg_payload.red;
        saveData("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/red"+std::to_string((double) SimNanos/(1e9))+".csv", this->red);
        this->green = vnir_msg_payload.green;
        saveData("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/green"+std::to_string((double) SimNanos/(1e9))+".csv", this->green);
        this->blue = vnir_msg_payload.blue;
        saveData("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/blue"+std::to_string((double) SimNanos/(1e9))+".csv", this->blue);
        this->nir = vnir_msg_payload.nir;
        saveData("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/nir"+std::to_string((double) SimNanos/(1e9))+".csv", this->nir);
    }
    
    // --> IMU Reading
    if(this->imu_msg.isLinked()){
        //std::cout << "IMU MSG IS LINKED!" << std::endl;
        IMUOutMsgPayload imu_msg_payload = this->imu_msg();
        this->imu_state = imu_msg_payload.state;
        telemetryFile << "Yaw: " << imu_msg_payload.yaw << ", Pitch: " << imu_msg_payload.pitch << ", Roll: " << imu_msg_payload.roll << "\n";
    }

    // --> Fine Prediction
    if(this->fine_msg.isLinked()){
        //std::cout << "FINENN MSG IS LINKED!" << std::endl;
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        this->fine_mask = fine_msg_payload.mask;
        saveData("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/mask"+std::to_string((double) SimNanos/(1e9))+".csv", this->fine_mask);
        telemetryFile << "Pan: " << fine_msg_payload.pan << ", Tilt: " << fine_msg_payload.tilt << "\n";
    }

    // --> GeoTracking data
    if(this->geo_msg.isLinked()){
        GeoTrackingMsgPayload geo_msg_payload = this->geo_msg();
        this->geo_state = geo_msg_payload.state;
        this->geo_lat = geo_msg_payload.lat;
        this->geo_lon = geo_msg_payload.lon;
    }

    // --> controller data
    if(this->controller_msg.isLinked()){
        ControllerOutMsgPayload controller_msg_payload = this->controller_msg();
        this->cont_state = controller_msg_payload.state;
        this->cont_msg = controller_msg_payload.msg;
    }

    if(this->mode_msg.isLinked()){
        ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
        this->mode = mode_msg_payload.mode;
    }

    // --> Balloon gps data
    if(this->ins_msg.isLinked()){
        //std::cout << "BALLOON MSG IS LINKED!" << std::endl;
        MessageConsumerMsgPayload ins_msg_payload = this->ins_msg();
        this->ins_state = ins_msg_payload.ins_state;
        this->ins_lat = ins_msg_payload.lat;
        //std::cout << this->ins_lat << std::endl;
        this->ins_lon = ins_msg_payload.lon;
        this->ins_alt = ins_msg_payload.alt;
        this->ins_yaw = ins_msg_payload.yaw;
        this->ins_pitch = ins_msg_payload.pitch;
        this->ins_roll = ins_msg_payload.roll;
        telemetryFile << "Lat: " << this->ins_lat << ", Lon: " << this->ins_lon << ", Alt: " << this->ins_alt << "\n";
        telemetryFile << "Yaw: " << this->ins_yaw << ", Pitch: " << this->ins_pitch << ", Roll: " << this->ins_roll << "\n";
        
    }
    //telemetryFile << "Teensy: " << this->temps << "\n";
    telemetryFile << "System time: " << time(NULL) << "\n";
    telemetryFile << "Controller message: " << this->cont_msg << "\n";
    telemetryFile.close();
    std::ifstream input_file("/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount)+"/telemetry"+std::to_string((double) SimNanos/(1e9))+".txt");
    this->telemetry = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

void DataStorage::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- (reset)");
    this->directoryCount = 0;
    bool directoryCreated = false;
    while(directoryCreated == false) {
        std::string dirPath = "/home/ben/nvme/storage/experiment"+std::to_string(this->directoryCount);
        const char * dirPathC = dirPath.c_str();
        if(mkdir(dirPathC,0777) == -1) {
            std::cerr << "Error creating directory." << std::endl;
            this->directoryCount +=1;
        } else {
            directoryCreated = true;
        }
    }

    // --> 1. Reset module state
    this->state = 0;
}



void DataStorage::UpdateState(uint64_t CurrentSimNanos)
{
    TelemetryMsgPayload telemetry_msg_buffer = this->telemetry_msg.zeroMsgPayload;
    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    if(this->mode != 0) {
        this->ReadMessages(CurrentSimNanos);
    }


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    int teensy = open("/dev/ttyACM0", O_RDWR); //TODO figure out actual port
    this->InitializePort(teensy);

    //write(teensy, read_teensy, sizeof(read_teensy));
    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));
    int num_bytes = read(teensy, &read_buf, sizeof(read_buf));
    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
    }
    //std::cout << read_buf << std::endl;
    this->temps = read_buf;
    telemetry_msg_buffer.tel_msg = this->telemetry;
    this->telemetry_msg.write(&telemetry_msg_buffer, this->moduleID, CurrentSimNanos);
    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

void DataStorage::saveData(std::string fileName, Eigen::MatrixXd matrix)
{
    //std::cout << "trying to save!" << std::endl;
	//https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
	Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");

	std::ofstream file(fileName);
	if (file.is_open())
	{
		file << matrix.format(CSVFormat);
		file.close();
	}
}

/*Eigen::MatrixXd DataStorage::openData(std::string fileToOpen)
{

	// the inspiration for creating this function was drawn from here (I did NOT copy and paste the code)
	// https://stackoverflow.com/questions/34247057/how-to-read-csv-file-and-assign-to-eigen-matrix
	
	// the input is the file: "fileToOpen.csv":
	// a,b,c
	// d,e,f
	// This function converts input file data into the Eigen matrix format



	// the matrix entries are stored in this variable row-wise. For example if we have the matrix:
	// M=[a b c 
	//	  d e f]
	// the entries are stored as matrixEntries=[a,b,c,d,e,f], that is the variable "matrixEntries" is a row vector
	// later on, this vector is mapped into the Eigen matrix format
	vector<double> matrixEntries;

	// in this object we store the data from the matrix
	ifstream matrixDataFile(fileToOpen);

	// this variable is used to store the row of the matrix that contains commas 
	std::string matrixRowString;

	// this variable is used to store the matrix entry;
	std::string matrixEntry;

	// this variable is used to track the number of rows
	int matrixRowNumber = 0;


	while (getline(matrixDataFile, matrixRowString)) // here we read a row by row of matrixDataFile and store every line into the string variable matrixRowString
	{
		std::stringstream matrixRowStringStream(matrixRowString); //convert matrixRowString that is a string to a stream variable.

		while (getline(matrixRowStringStream, matrixEntry, ',')) // here we read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry
		{
			matrixEntries.push_back(stod(matrixEntry));   //here we convert the string to double and fill in the row vector storing all the matrix entries
		}
		matrixRowNumber++; //update the column numbers
	}

	// here we convet the vector variable into the matrix and return the resulting object, 
	// note that matrixEntries.data() is the pointer to the first memory location at which the entries of the vector matrixEntries are stored;
	return Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);

}*/

void DataStorage::InitializePort(int serial_port) {
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
}