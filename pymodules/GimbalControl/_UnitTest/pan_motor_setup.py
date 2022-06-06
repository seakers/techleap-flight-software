# Init for tilt motor
# Run at the very beginning to command the motor speed, accel, stepsize,

import serial, time

def pan_init():
    try:
        ser = pan_motor_init() # Initialise the serial port
        ser.open()         # open the serial port
        time.sleep(1)
    except Exception as e:
        print ('error opening PAN - serial port')
        exit()

    if ser.isOpen():
        try:
            ser.flushInput()
            ser.flushOutput()
            pan_motor_setup(ser)  # Complete motor setup and enable motor
            return ser 
        except Exception as e1:
            print ("Error Communicating...: " + str(e1))
    else:
        print ("Cannot open serial port ")

# Motor functions
# Initialization parameters. Note the serial port and baud rate of your project
# may vary. Our default baud rate is 9600
def pan_motor_init():
    ser=serial.Serial()
    ser.port = "COM6"
    ser.baudrate = 9600
    ser.bytesize = serial.EIGHTBITS
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    ser.timeout= .1
    ser.xonxoff = False
    ser.rtscts = False
    ser.dsrdtr = False
    ser.writeTimeout = 0
    return ser

# When we send a serial command, the program will check and print
# the response given by the drive.
def pan_motor_send(ser,command):
        ser.write((command+'\r').encode())
        response = ser.read(15).decode()
        if len(response) > 0:
            #print (response)
            ser.flushInput()
            return response

def pan_motor_setup(ser):
        """
        Setup initial motor parameters, also resets alarm
        """
        pan_motor_send(ser,'1EG51200') # Sets microstepping to 20,000 steps per revolution
        pan_motor_send(ser,'1EF1') # detect stall -> reflects in status command RS=AED as alarm
        pan_motor_send(ser,'1EF2') # prevent stall 
        pan_motor_send(ser,'1IFD') # Sets the format of drive responses to decimal
        pan_motor_send(ser,'1SP0') # Sets the starting position at 0
        pan_motor_send(ser,'1AR') # Alarm reset
        pan_motor_send(ser,'1AC0.168') # Acceleration 
        pan_motor_send(ser,'1DE0.168') # Deceleration
        pan_motor_send(ser,'1VE0.01') # Velocity 
        pan_motor_send(ser,'1ME')  # Enable Motor
        pan_motor_send(ser,'1IP')


def panBySteps(ser, nsteps):
        moveCmd = '1FL' + str(nsteps)
        pan_motor_send(ser, moveCmd) # Moves CW -> nsteps +
        # nsteps -> -ve -> move CCW
        """
        This section demonstrates the drives ability to poll immediate position
        and check status to see if the move is done.
        """
        time.sleep(0.85)
        return pan_motor_send(ser, '1IP') # IP is immediate position
        #time.sleep(0.2)
        #send(ser,'2IP')
        #time.sleep(0.1)
        #send(ser,'2RS') # We end by requesting the status of the drive.


