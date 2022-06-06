import math
from IMU_setup import *
from tilt_motor_setup import *
from pan_motor_setup import *
from camera_setup import *
import time
from pid import PID

import sys
import cv2
import numpy as np
from multiprocessing import Manager
from multiprocessing import Process
from imutils.video import VideoStream
#from controller import PID
import argparse
import signal

Kp = 0.9
Ki = 0
Kd = 0.1

# Hard code a scan pattern for system init and reset

# prior: home position (from IMU - pre-calibrated)
# once home is identified -> enable scan until object detection

# scan pattern - slowly cover the whole hemi-sphere not hitting the
# gimbal pan/tilt limits
# home coordinates: (pan, tilt)

###################### Sequence: ###############################
# Get to home
# 1) tilt -> home_tilt + 45; 2) tilt -> home_tilt - 45
# 2) get to home_tilt
# 3) Yaw -> home_yaw + 45;
# 4) tilt -> home_tilt + 45; 2) tilt -> home_tilt - 45
# 5) get to home_yaw

# breakpoint: object identified = True
# return: current yaw and tilt angles
################################################################


def initialization():
    # should go into setup
    IMU = IMU_init()
    tilt_motor = tilt_init()
    pan_motor = pan_init()
    st_datastream, st_device = cam_init()
    # return the objects
    return IMU, tilt_motor, pan_motor, st_datastream, st_device



def getIMUreading(IMU):
    ypr = IMU_read(IMU)
    return ypr

def getHomeLocation(IMU):
    # should go into setup
    ypr = getIMUreading(IMU)
    # Yaw: PAN axis, Pitch: Tilt axis
    return ypr.x, ypr.y
    
def TILT_deg2steps(ndegrees):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 3.5866
    steps_per_deg = gear_ratio * steps_per_rev / one_rev
    return math.ceil(ndegrees * steps_per_deg)

def TILT_steps2deg(nsteps):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 3.5866
    deg_per_steps = one_rev / steps_per_rev / gear_ratio
    return nsteps * deg_per_steps

def PAN_deg2steps(ndegrees):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 1
    steps_per_deg = gear_ratio * steps_per_rev / one_rev
    return math.ceil(ndegrees * steps_per_deg)

def PAN_steps2deg(nsteps):
    steps_per_rev = 51200
    one_rev = 360
    gear_ratio = 1
    deg_per_steps = one_rev / steps_per_rev / gear_ratio
    return nsteps * deg_per_steps

def TILTSCAN(tilt_motor, IMU):
    for jj in range(0, 5):
        tiltBySteps(tilt_motor, TILT_deg2steps(1))
        print(getIMUreading(IMU))
    for jj in range(0, 10):
        tiltBySteps(tilt_motor, -TILT_deg2steps(1))
        print(getIMUreading(IMU))
    for jj in range(0, 5):
        tiltBySteps(tilt_motor, TILT_deg2steps(1))
        print(getIMUreading(IMU))


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
    pan, tilt = getHomeLocation(IMU) 

    HOME = [pan, tilt]
    # HOME COORDINATES (POLAR): [64 deg, -85 deg]
    
    PAN_LIMITS = [HOME[0] - 34, HOME[0] + 34] # Relative Range: {30, 98} deg
    TILT_LIMITS = [HOME[0] - 35, HOME[0] + 35] # Relative Range: {-120, -50} deg

    """
    
    # scanning mode: status_tested
    # Scanning is programmed to always start from the home
    # if we're not at home, get to home using IMU feedback
    
    objectIdentified = False
    scanInProgress = True

    while not objectIdentified and scanInProgress:
        
        TILTSCAN(tilt_motor, IMU) # PAN SCAN at home
        
        for ii in range(0, 5):
            panBySteps(pan_motor, -PAN_deg2steps(1))
            print(getIMUreading(IMU))
            
        TILTSCAN(tilt_motor, IMU)
        
        for ii in range(0, 10):
            panBySteps(pan_motor, PAN_deg2steps(1))
            print(getIMUreading(IMU))

        for ii in range(0, 5):
            panBySteps(pan_motor, -PAN_deg2steps(1))
            print(getIMUreading(IMU))
            
        scanInProgress = False
        
    """


    
    global x0, y0, x1, y1, drawing, mode, frame, bbox, tracker, tracker_initialized
    global MODE_MARK


    home = [0, -75]
    panAngle = home[0]
    tiltAngle = home[1]
    bugFlag = 1

    
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
    fps_text = '?? Fps'
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
            #print('tracker_init1', tracker_initialized)
            #
            # Update the tracker with the newly acquired frame.
            #
            
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
            #
            # If tracking is lost for some reason then use the last location
            # of the bounding box to mark that last location with a red box
            #
            else:
                p1 = (int(last_good_bbox[0]), int(last_good_bbox[1]))
                p2 = (int(last_good_bbox[0] + last_good_bbox[2]), int(last_good_bbox[1] + last_good_bbox[3]))
                cv2.rectangle(frame, p1, p2, (0, 0, 255), 1)

                if mode == MODE_TRACK:
                    mode = MODE_TRACK_HOLD

        if mode == MODE_TRACK and tracker_initialized:
            
            err_pan = err_pan_i
            err_tilt = err_tilt_i

            control_pan = err_pan /(0.3*2048)
            control_tilt = err_tilt / (0.3 * 2048)
        
            #control_pan = np.arctan2(err_pan, 0.3*2940) * 180/np.pi
            #control_tilt = np.arctan2(err_tilt, 0.3*2940) * 180/np.pi

            panBySteps(pan_motor, PAN_deg2steps(control_tilt*2.5))
            tiltBySteps(tilt_motor, TILT_deg2steps(control_pan*2.5))
            
            #panAngle += control_pan
            #tiltAngle += -control_tilt
            
            print("Pixel errors: ", err_pan, err_tilt)
            print("Pan/Tilt changes: ", control_pan, control_tilt)
            # print("Pan, tilt requested: ", panAngle, tiltAngle)
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
    
    # get to home
    # PID control
    
    # OpenCV - start preview
    
    #panBySteps(pan_motor, -PAN_deg2steps(10))
    
    """
    # Image acquisition test: working
    
    for ii in range(0, 10):
        nparr = camRead(st_datastream, st_device)
        cv2.imshow('image', nparr)
        cv2.waitKey(1)
        time.sleep(1)
    """      
