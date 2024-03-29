//
// Created by gabe :)
//

#include "ExternalModules/ImagerVNIR/ImagerVNIR.h" // --> CHANGE
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "architecture/utilities/avsEigenSupport.h"
#include "architecture/utilities/linearAlgebra.h"

//Namespace for using StApi.
using namespace StApi;

//Namespace for using cout
using namespace std;

// Count of images to be grabbed. We'll only get 1 image to demo how to save image file.
const uint64_t nCountOfImagesToGrab = 1;



ImagerVNIR::ImagerVNIR() // --> CHANGE
{
    this->state = 0;
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
}

ImagerVNIR::~ImagerVNIR() // --> CHANGE
{
    return;
}


void ImagerVNIR::ZeroOutputVariables(){
    this->red.setZero(512, 512);
    this->green.setZero(512, 512);
    this->blue.setZero(512, 512);
    this->nir.setZero(512, 512);
}



void ImagerVNIR::Reset(uint64_t CurrentSimNanos) {
    bskLogger.bskLog(BSK_INFORMATION, "ImagerVNIR ---- (reset)");

    // --> 1. Reset module state
    this->state = 0;

    // --> 2. Initialize tensors
    this->ZeroOutputVariables();
}

class Band{
    public:
        int ColAvg;
        int RowAvg;
        int Col;
        int Row;
        int ColInc;
        int RowInc;
};



void ImagerVNIR::UpdateState(uint64_t CurrentSimNanos) {


    // -----------------------
    // ----- Zero Output -----
    // -----------------------

    // --> Zero output messages
    ImagerVNIROutMsgPayload vnir_msg_buffer = this->vnir_msg.zeroMsgPayload;

    // --> Zero internal output variables
    this->ZeroOutputVariables();


    // --------------------------
    // ----- Process Inputs -----
    // --------------------------
    // --> TODO: Implement SDK reading vnir sensor and copy values over to image_tensor
    this->state = 0;
    try{
             // Initialize StApi before using.
        CStApiAutoInit objStApiAutoInit;

        // Create a system object for device scan and connection.
        CIStSystemPtr pIStSystem(CreateIStSystem());

        // Create a camera device object and connect to first detected device.
        CIStDevicePtr pIStDevice(pIStSystem->CreateFirstIStDevice());

        // Displays the DisplayName of the device.
        //cout << "Device=" << pIStDevice->GetIStDeviceInfo()->GetDisplayName() << endl;

        // Create a DataStream object for handling image stream data.
        CIStDataStreamPtr pIStDataStream(pIStDevice->CreateIStDataStream(0));

        // Start the image acquisition of the host (local machine) side.
        pIStDataStream->StartAcquisition(nCountOfImagesToGrab);

        // Start the image acquisition of the camera side.
        pIStDevice->AcquisitionStart();

        // Retrieve the buffer pointer of image data with a timeout of 5000ms.
        // Note that we don't use a while loop here because we only retrieve one image for saving.
        CIStStreamBufferPtr pIStStreamBuffer(pIStDataStream->RetrieveBuffer(500));

        // Check if the acquired data contains image data.
        if(pIStStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent())
        {
            // If yes, we create a IStImage object for further image handling.
            IStImage *pIStImage = pIStStreamBuffer->GetIStImage();

            // Display the information of the acquired image data.
            cout << "\r BlockId=" << pIStStreamBuffer->GetIStStreamBufferInfo()->GetFrameID()
                << " Size:" << pIStImage->GetImageWidth() << " x " << pIStImage->GetImageHeight()
                << " First byte =" << (uint32_t)*(uint8_t*)pIStImage->GetImageBuffer() << endl;

            // demosaicing code
            Band redBand;
            redBand.ColAvg = 1;
            redBand.RowAvg = 1;
            redBand.Col = 0;
            redBand.Row = 0;
            redBand.ColInc = 4;
            redBand.RowInc = 4;
            uint8_t* ptr = (uint8_t*)pIStImage->GetImageBuffer();
            //byte* ptr = (byte*)aBuffer.DataPointer;
            int imageRows = 2048;
            int imageCols = 2048;
            int s = 2048*2048;
            // demosaic the image - image will be smaller by a factor related to dr and dc
            int sum;
            int n;
            int r;
            int tr;
            int c;
            int tc;
            int kr;
            int kc;
            int kci;
            int kri;
            int avg;
            int sindex;

            for (r = redBand.Row, tr = 0; r < imageRows; r += redBand.RowInc, tr += 1)
            {
                for (c = redBand.Col, tc = 0; c < imageCols; c += redBand.ColInc, tc += 1)
                {
                    // if more than one source pixel used, compute average
                    sum = 0;
                    n = 0;
                    for (kr = r, kri = 0; kri < redBand.RowAvg; kr += 1, kri += 1)
                    {
                        for (kc = c, kci = 0; kci < redBand.ColAvg; kc += 1, kci += 1)
                        {
                            sindex = imageCols * kr + kc;
                            if (sindex > s) break;
                            IStPixelComponentValue *pIStPixelComponentValue = pIStImage->GetIStPixelComponentValue(kr,kc);
                            sum+=pIStPixelComponentValue->GetValue(0);
                            n += 1;
                        }
                    }
                    if (n != 0)
                    {
                        avg = sum / n;
                    }
                    else
                    {
                        avg = 0;
                    }
                    if (avg > 255) { avg = 255; }
                    //this->red[tr][tc] = (uint8_t)avg;
                    this->red(tr,tc) = (uint8_t)avg;
                }
            }
            Band greenBand;
            greenBand.ColAvg = 1;
            greenBand.RowAvg = 1;
            greenBand.Col = 0;
            greenBand.Row = 2;
            greenBand.ColInc = 4;
            greenBand.RowInc = 4;
            // demosaic the image - image will be smaller by a factor related to dr and dc
            
            for (r = greenBand.Row, tr = 0; r < imageRows; r += greenBand.RowInc, tr += 1)
            {
                for (c = greenBand.Col, tc = 0; c < imageCols; c += greenBand.ColInc, tc += 1)
                {
                    // if more than one source pixel used, compute average
                    sum = 0;
                    n = 0;
                    for (kr = r, kri = 0; kri < greenBand.RowAvg; kr += 1, kri += 1)
                    {
                        for (kc = c, kci = 0; kci < greenBand.ColAvg; kc += 1, kci += 1)
                        {
                            sindex = imageCols * kr + kc;
                            if (sindex > s) break;
                            IStPixelComponentValue *pIStPixelComponentValue = pIStImage->GetIStPixelComponentValue(kr,kc);
                            sum+=pIStPixelComponentValue->GetValue(0);
                            n += 1;
                        }
                    }
                    if (n != 0)
                    {
                        avg = sum / n;
                    }
                    else
                    {
                        avg = 0;
                    }
                    if (avg > 255) { avg = 255; }
                    this->green(tr,tc) = (uint8_t)avg;
                }
            }
            Band blueBand;
            blueBand.ColAvg = 1;
            blueBand.RowAvg = 1;
            blueBand.Col = 2;
            blueBand.Row = 0;
            blueBand.ColInc = 4;
            blueBand.RowInc = 4;
            // demosaic the image - image will be smaller by a factor related to dr and dc
            
            for (r = blueBand.Row, tr = 0; r < imageRows; r += blueBand.RowInc, tr += 1)
            {
                for (c = blueBand.Col, tc = 0; c < imageCols; c += blueBand.ColInc, tc += 1)
                {
                    // if more than one source pixel used, compute average
                    sum = 0;
                    n = 0;
                    for (kr = r, kri = 0; kri < blueBand.RowAvg; kr += 1, kri += 1)
                    {
                        for (kc = c, kci = 0; kci < blueBand.ColAvg; kc += 1, kci += 1)
                        {
                            sindex = imageCols * kr + kc;
                            if (sindex > s) break;
                            IStPixelComponentValue *pIStPixelComponentValue = pIStImage->GetIStPixelComponentValue(kr,kc);
                            sum+=pIStPixelComponentValue->GetValue(0);
                            n += 1;
                        }
                    }
                    if (n != 0)
                    {
                        avg = sum / n;
                    }
                    else
                    {
                        avg = 0;
                    }
                    if (avg > 255) { avg = 255; }
                    this->blue(tr,tc) = (uint8_t)avg;
                }
            }
            Band nirBand;
            nirBand.ColAvg = 1;
            nirBand.RowAvg = 1;
            nirBand.Col = 2;
            nirBand.Row = 2;
            nirBand.ColInc = 4;
            nirBand.RowInc = 4;
            // demosaic the image - image will be smaller by a factor related to dr and dc
            
            for (r = nirBand.Row, tr = 0; r < imageRows; r += nirBand.RowInc, tr += 1)
            {
                for (c = nirBand.Col, tc = 0; c < imageCols; c += nirBand.ColInc, tc += 1)
                {
                    // if more than one source pixel used, compute average
                    sum = 0;
                    n = 0;
                    for (kr = r, kri = 0; kri < nirBand.RowAvg; kr += 1, kri += 1)
                    {
                        for (kc = c, kci = 0; kci < nirBand.ColAvg; kc += 1, kci += 1)
                        {
                            sindex = imageCols * kr + kc;
                            if (sindex > s) break;
                            IStPixelComponentValue *pIStPixelComponentValue = pIStImage->GetIStPixelComponentValue(kr,kc);
                            sum+=pIStPixelComponentValue->GetValue(0);
                            n += 1;
                        }
                    }
                    if (n != 0)
                    {
                        avg = sum / n;
                    }
                    else
                    {
                        avg = 0;
                    }
                    if (avg > 255) { avg = 255; }
                    this->nir(tr,tc) = (uint8_t)avg;
                }
            }
            this->status_msg = "Image captured.";
        }
        else
        {
            // If the acquired data contains no image data...
            cout << "Image data does not exist." << endl;
            this->state = 1;
            this->status_msg = "Image data does not exist.";
        }

        // Stop the image acquisition of the camera side.
        pIStDevice->AcquisitionStop();

        // Stop the image acquisition of the host side.
        pIStDataStream->StopAcquisition();
    }
    catch (const GenICam::GenericException &e)
    {
        // Display a description of the error.
        cerr << endl << "An exception occurred." << endl << e.GetDescription() << endl;
        this->state = 1;
        this->status_msg = "Camera exception occurred.";
    }
       
    
//    if(this->mock_msg.isLinked()){
//        ImagerVNIROutMsgPayload mock_msg_payload = this->mock_msg();
//        for(int x = 0; x < 20; x++){
//            for(int y = 0; y < 20; y++){
//                this->image_tensor[x][y] = mock_msg_payload.imageTensor[x][y];
//            }
//        }
//    }
//    else{
//        for(int x = 0; x < 20; x++){
//            for(int y = 0; y < 20; y++){
//                // this->image_tensor[x][y] = this->image_tensor[x][y];
//                this->image_tensor[x][y] = this->state;
//            }
//        }
//    }




    // -------------------------
    // ----- Write Outputs -----
    // -------------------------
    // --> TODO: Write correct image dimensions
    vnir_msg_buffer.state = this->state;
    vnir_msg_buffer.status_msg = this->status_msg;
    vnir_msg_buffer.red = this->red;
    vnir_msg_buffer.green = this->green;
    vnir_msg_buffer.blue = this->blue;
    vnir_msg_buffer.nir = this->nir;
    this->vnir_msg.write(&vnir_msg_buffer, this->moduleID, CurrentSimNanos);


    // -------------------
    // ----- Logging -----
    // -------------------
    bskLogger.bskLog(BSK_INFORMATION, "ImagerVNIR ---- ran update at %fs", this->moduleID, (double) CurrentSimNanos/(1e9));
}

void ImagerVNIR::readInputMessages(){
    ControllerModeMsgPayload mode_msg_payload = this->mode_msg();
    this->captureMode = mode_msg_payload.mode;
}