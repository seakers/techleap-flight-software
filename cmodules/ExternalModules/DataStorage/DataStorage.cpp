//
// Created by gabe :)
//

#include "ExternalModules/DataStorage/DataStorage.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include <fstream>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"



DataStorage::DataStorage() // --> CHANGE
{
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
    //this->b1.setZero(512, 512);
    this->fine_mask.setZero(512, 512);
}

DataStorage::~DataStorage() // --> CHANGE
{
    return;
}

void DataStorage::ReadMessages(uint64_t SimNanos){
    std::ofstream telemetryFile;
    telemetryFile.open("/media/ben/USB/telemetry"+std::to_string((double) SimNanos)+".txt");

    // --> VNIR Reading
    if(this->vnir_msg.isLinked()){
        std::cout << "VNIR MSG IS LINKED!" << std::endl;
        ImagerVNIROutMsgPayload vnir_msg_payload = this->vnir_msg();
        this->vnir_state = vnir_msg_payload.state;
        this->red = vnir_msg_payload.red;
        saveData("/media/ben/USB/red"+std::to_string((double) SimNanos)+".csv", this->red);
        this->green = vnir_msg_payload.green;
        saveData("/media/ben/USB/green"+std::to_string((double) SimNanos)+".csv", this->green);
        this->blue = vnir_msg_payload.blue;
        saveData("/media/ben/USB/blue"+std::to_string((double) SimNanos)+".csv", this->blue);
        this->nir = vnir_msg_payload.nir;
        saveData("/media/ben/USB/nir"+std::to_string((double) SimNanos)+".csv", this->nir);
    }
    
    // --> IMU Reading
    if(this->imu_msg.isLinked()){
        std::cout << "IMU MSG IS LINKED!" << std::endl;
        IMUOutMsgPayload imu_msg_payload = this->imu_msg();
        this->imu_state = imu_msg_payload.state;
        telemetryFile << "Yaw: " << imu_msg_payload.yaw << ", Pitch: " << imu_msg_payload.pitch << ", Roll: " << imu_msg_payload.roll << "\n";
    }

    // // --> Thermal Reading
    // if(this->thermal_msg.isLinked()){
    //     ImagerThermalOutMsgPayload thermal_msg_payload = this->thermal_msg();
    //     this->thermal_state = thermal_msg_payload.state;
    //     this->b1 = thermal_msg_payload.b1;
    //     this->b2 = thermal_msg_payload.b2;
    //     this->b3 = thermal_msg_payload.b3;
    //     this->b4 = thermal_msg_payload.b4;
    // }

    // --> Fine Prediction
    if(this->fine_msg.isLinked()){
        std::cout << "FINENN MSG IS LINKED!" << std::endl;
        FinePredictionMsgPayload fine_msg_payload = this->fine_msg();
        this->fine_state = fine_msg_payload.state;
        this->fine_mask = fine_msg_payload.mask;
        saveData("/media/ben/USB/mask"+std::to_string((double) SimNanos)+".csv", this->fine_mask);
        telemetryFile << "Pan: " << fine_msg_payload.pan << ", Tilt: " << fine_msg_payload.tilt << "\n";
    }

    // --> GeoTracking data
    if(this->geo_msg.isLinked()){
        GeoTrackingMsgPayload geo_msg_payload = this->geo_msg();
        this->geo_state = geo_msg_payload.state;
        this->geo_lat = geo_msg_payload.lat;
        this->geo_lon = geo_msg_payload.lon;
    }

    // --> Balloon gps data
    if(this->gps_msg.isLinked()){
        std::cout << "BALLOON MSG IS LINKED!" << std::endl;
        MessageConsumerMsgPayload gps_msg_payload = this->gps_msg();
        this->gps_state = gps_msg_payload.state;
        this->gps_lat = gps_msg_payload.lat;
        std::cout << this->gps_lat << std::endl;
        this->gps_lon = gps_msg_payload.lon;
        this->gps_alt = gps_msg_payload.alt;
        telemetryFile << "Lat: " << this->gps_lat << ", Lon: " << this->gps_lon << ", Alt: " << this->gps_alt << "\n";
    }
    telemetryFile.close();
}

void DataStorage::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- (reset)");

    // --> 1. Reset module state
    this->state = 0;
}



void DataStorage::UpdateState(uint64_t CurrentSimNanos)
{
    // -----------------------
    // ----- Read Inputs -----
    // -----------------------
    this->ReadMessages(CurrentSimNanos);

    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    

    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "DataStorage --- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

void DataStorage::saveData(std::string fileName, Eigen::MatrixXd matrix)
{
    std::cout << "trying to save!" << std::endl;
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