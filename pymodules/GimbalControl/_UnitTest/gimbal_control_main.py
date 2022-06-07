from IMU_setup import *
from tilt_motor_setup import *
from pan_motor_setup import *
from camera_setup import *
import time, math
from pid import PID

import sys
import cv2
import numpy as np
from multiprocessing import Manager
from multiprocessing import Process
from imutils.video import VideoStream
import argparse
import signal


def initialization():
    # STEP 0: INITIALIZE IMU MOTORS CAMERA and RETURN HANDLES OF EACH UNIT
    IMU = IMU_init()
    tilt_motor = tilt_init()
    pan_motor = pan_init()
    st_datastream, st_device = cam_init()
    # return the objects
    return IMU, tilt_motor, pan_motor, st_datastream, st_device

def getIMUreading(IMU):
    ypr = IMU_read(IMU)
    return ypr

def register_home(IMU):
    # STEP 1: KNOW OUR ORIENTATION AT INIT
    # register the pan tilt angles when the system is staged for launch
    ypr = getIMUreading(IMU)
    print("HOME COORDINATES: ", ypr)
    # Yaw: PAN axis, Pitch: Tilt axis
    return ypr.x, ypr.y


def limitPan(control_pan, acc_control_ang):
    if acc_control_ang > -40 and acc_control_ang < 40:
        control_pan = control_pan
    else:
        control_pan = 0
    return control_pan

def limitTilt(control_tilt, acc_control_ang):
    if acc_control_ang > -40 and acc_control_ang < 40:
        control_tilt = control_tilt
    else:
        control_tilt = 0
    return control_tilt

   

## CV2 functions - DEMO purposed only
def mark_rect(event, x, y, flags, param):
    """
    Mouse callback that marks a frame region for tracker initialization.
    param:  event type, mouse coordinates and event parameters
    return: nothing, marks a frame region for tracker initialization
    """

    global x0, y0, x1, y1, drawing, mode, frame, bbox, tracker, tracker_initialized
    global MODE_MARK

    if mode != MODE_MARK:
        return
        
    if event == cv2.EVENT_LBUTTONDOWN:
        drawing = True
        x0, y0 = x, y
        x1, y1 = x, y

    elif event == cv2.EVENT_MOUSEMOVE:
        if drawing == True:
            x1, y1 = x, y

    elif event == cv2.EVENT_LBUTTONUP:
        drawing = False

        #
        # Convert any start (x0,y0) and end (x1,y1) points to be
        # a top-left to bottom right pair.
        # Extract ROI and initialize the tracker
        #
        if x0 == x1 or y0 == y1 or x0 < 0 or x1 < 0 or y0 < 0 or y1 < 0:
            return
        if x0 > x1:
            x0, x1 = x1, x0
        if y0 > y1:
            y0, y1 = y1, y0

        bbox = (x0, y0, x1-x0, y1-y0)
       # print('tracker_init', 'bbox', tracker_initialized,bbox)
        a = tracker.init(frame, bbox)
        
        tracker_initialized, boxed = tracker.update(frame)
        #print('tracker.init()', tracker_initialized)

if __name__ == "__main__":
    
    IMU, tilt_motor, pan_motor, st_datastream, st_device = initialization()
    #IMU = initialization()
    IMU_read(IMU)
    pan, tilt = register_home(IMU)

    HOME = [pan, tilt] # {-155, -6} degrees - current IMU readings
    PAN_LIMITS = [HOME[0] - 60, HOME[0] + 60] # Relative Range: {30, 98} deg
    TILT_LIMITS = [HOME[1] - 60, HOME[1] + 60] # Relative Range: {-120, -50} deg

    ################################### LIMITING THE ANGLES #################
    acc_control_ang = 0 # accumulated pan angle
    control_ang_prev = 0
    IMU_read(IMU)
    """
    panBySteps(pan_motor, PAN_deg2steps(10))
    
    for ii in range(0, 15):
        control_ang = 3
        acc_control_ang += (control_ang)
        angle_ = limitPan(control_ang, acc_control_ang)
        panBySteps(pan_motor, PAN_deg2steps(angle_))
        control_ang_prev = control_ang
        print("angle_, acc_control_ang: ", angle_, acc_control_ang)
        print("IMU: ", IMU_read(IMU))

    print("test back")
    
    for ii in range(0, 30):
        control_ang = -(3)
        acc_control_ang += control_ang
        angle_ = limitPan(control_ang, acc_control_ang)
        panBySteps(pan_motor, PAN_deg2steps(angle_))
        print("angle_, acc_control_ang: ", angle_, acc_control_ang)
        print("IMU: ", IMU_read(IMU))

    print("test successful")
    
    # commence scan pattern
    #######################################################################
    
    """
    ####################### SET CONTROLLER GAINS ####################
    pan_Kp = 10
    pan_Kd = 0.1
    pan_Ki = 0.0
    panPID = PID(pan_Kp, pan_Kd, pan_Ki)

    tilt_Kp = 3
    tilt_Kd = 0.1
    tilt_Ki = 0
    tiltPID = PID(tilt_Kp, tilt_Kd, tilt_Ki)
    ##################################################################

    # COMMENCE TRACKING
    
    global x0, y0, x1, y1, drawing, mode, frame, bbox, tracker, tracker_initialized
    global MODE_MARK

    panAngle = pan
    tiltAngle = tilt
    bugFlag = 1
    panAcc = 0
    tiltAcc = 0
    px_err_log = np.array([])
    py_err_log = np.array([])
    pan_signal_log = np.array([])
    tilt_signal_log = np.array([])
    
    # CV2 object tracker
    MODE_TRACK = 0          # track an object
    MODE_SHOW = 1           # only show tracking markers on video
    MODE_MARK = 2           # mark region color to track
    MODE_TRACK_HOLD = 3     # temporarily suspend tracking until object is recaptured

    tracker_initialized = False
    bbox = (0, 0, 0, 0)
    last_good_bbox = bbox

    mode = MODE_SHOW
    mode_text = 'Show'
    fps_text = 'x Fps'
    cvs_title_printed = False
    drawing = False         # true if mouse is pressed
    x0, y0 = -1, -1
    x1, y1 = -1, -1

    print(' m - mark color region to track\n t - track\n s - display tracking marker only\n ESC - quit')

    cv2.namedWindow('image', cv2.WINDOW_GUI_NORMAL+cv2.WINDOW_AUTOSIZE)
    cv2.setMouseCallback('image', mark_rect)
    
    font = cv2.FONT_HERSHEY_SIMPLEX
    
    frameWidth = 614
    frameHeight = 614

    frameCenterX = 614/2
    frameCenterY = 614/2

    tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW', 'GOTURN']
    tracker_type = tracker_types[2]
 
    if tracker_type == 'BOOSTING':
        tracker = cv2.TrackerBoosting_create()
    if tracker_type == 'MIL':
        tracker = cv2.TrackerMIL_create()
    if tracker_type == 'KCF':
        tracker = cv2.TrackerKCF_create()
    if tracker_type == 'TLD':
        tracker = cv2.TrackerTLD_create()
    if tracker_type == 'MEDIANFLOW':
        tracker = cv2.TrackerMedianFlow_create()
    if tracker_type == 'GOTURN':
        tracker = cv2.TrackerGOTURN_create()
    
    while(True):
        frame = camRead(st_datastream, st_device)
        frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)
        if mode == MODE_MARK:
            cv2.rectangle(frame, (x0, y0), (x1, y1), (255, 255, 0), 1)
        elif tracker_initialized:
            # Update the tracker with the newly acquired frame.
            
            track_ok, bbox = tracker.update(frame)

            if track_ok:
                last_good_bbox = bbox
                
                p1 = (int(bbox[0]), int(bbox[1]))
                p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
                cv2.rectangle(frame, p1, p2, (255, 0, 0), 2)

                object_x = int(bbox[0] + bbox[2]/2)
                object_y = int(bbox[1] + bbox[3]/2)
                
                err_pan_i = frameCenterX - object_x
                err_tilt_i = frameCenterY - object_y

                cv2.arrowedLine(frame, (int(frameCenterX), int(frameCenterY)), (int(object_x), int(object_y)), (255, 0, 0), 2)

                if mode == MODE_TRACK_HOLD:
                    mode = MODE_TRACK
            
            # If tracking is lost for some reason then use the last location
            # of the bounding box to mark that last location with a red box
            
            else:
                p1 = (int(last_good_bbox[0]), int(last_good_bbox[1]))
                p2 = (int(last_good_bbox[0] + last_good_bbox[2]), int(last_good_bbox[1] + last_good_bbox[3]))
                cv2.rectangle(frame, p1, p2, (0, 0, 255), 1)

                if mode == MODE_TRACK:
                    mode = MODE_TRACK_HOLD

        if mode == MODE_TRACK and tracker_initialized:
            
            err_pan = err_pan_i/614
            err_tilt = err_tilt_i/614

            px_err_log = np.append(px_err_log, err_pan_i)
            py_err_log = np.append(py_err_log, err_tilt_i)

            
            control_pan = panPID.update(err_tilt, 0)
            control_tilt = tiltPID.update(err_pan, 0)

            
            #control_pan = np.arctan2(err_pan, 0.3*2940) * 180/np.pi
            #control_tilt = np.arctan2(err_tilt, 0.3*2940) * 180/np.pi

            # Because pixel errors in x -> tilt (camera is setup like that!)
            # inversion

            # Measure the angle to keep them within mechanical limits

            panAcc += control_pan
            tiltAcc += control_tilt

            #print("panAngle, tiltAngle accumulates:", panAcc, tiltAcc)
            # allow control_pan only if panAngle is in the pan limits
                
            control_pan = limitPan(control_pan, panAcc)
            control_tilt = limitTilt(control_tilt, tiltAcc)
            
            pan_signal_log = np.append(pan_signal_log, control_pan)
            tilt_signal_log = np.append(tilt_signal_log, control_tilt)

            
            panBySteps(pan_motor, PAN_deg2steps(control_pan))
            tiltBySteps(tilt_motor, TILT_deg2steps(control_tilt))
            
            #panAngle += control_pan
            #tiltAngle += -control_tilt
            
            print("Pixel errors: ", err_pan_i, err_tilt_i)
            print("Pan/Tilt changes: ", control_tilt, control_pan)
            #print("Pan, tilt requested: ", panAngle, tiltAngle)
             #control_pan = err_pan/float(frameWidth/2)
             #control_tilt = err_tilt/float(frameHeight/2)
        else:
            pass

        if mode == MODE_TRACK_HOLD:
            cv2.putText(frame, mode_text, (1, 20), font, 0.4, (0, 0, 255), 1, cv2.LINE_AA)
        else:
            cv2.putText(frame, mode_text, (1, 20), font, 0.4, (255, 0, 0), 1, cv2.LINE_AA)

        cv2.putText(frame, tracker_type, (1, 40), font, 0.4, (255, 0, 0), 1, cv2.LINE_AA)
        cv2.putText(frame, fps_text, (1, 60), font, 0.4, (255, 0, 0), 1, cv2.LINE_AA)

        cv2.imshow('image', frame)

        key = cv2.waitKey(1) & 0xFF
        if key == 27:
            break
        elif key == ord('m') and not tracker_initialized:
            x0,y0  = -1,-1
            x1,y1  = -1,-1
            mode_text = 'Mark'
            mode = MODE_MARK
        elif key == ord('t'):
            mode_text = 'Track'
            if tracker_initialized:
                mode = MODE_TRACK
            else:
                mode = MODE_TRACK_HOLD
        elif key == ord('s'):
            mode_text = 'Show'
            mode = MODE_SHOW
        else:
            pass

    cv2.destroyAllWindows()

    
    
    """
    frame = camRead(st_datastream, st_device)
    cv2.imshow('image', frame)
    cv2.waitKey(1)
    
    for jj in range(0, 5):
        tiltBySteps(tilt_motor, TILT_deg2steps(1))
        print(getIMUreading(IMU))
    """

