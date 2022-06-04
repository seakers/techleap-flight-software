# Init for tilt motor
# Run at the very beginning to command the motor speed, accel, stepsize,

import serial, time

def tilt_init():
    try:
        ser = motor_init() # Initialise the serial port
        ser.open()         # open the serial port
        time.sleep(1)
    except Exception as e:
        print ('error opening serial port')
        exit()

    if ser.isOpen():
        try:
            ser.flushInput()
            ser.flushOutput()
            motor_setup(ser)  # Complete motor setup and enable motor
            return ser 
        except Exception as e1:
            print ("Error Communicating...: " + str(e1))
    else:
        print ("Cannot open serial port ")

# Motor functions
# Initialization parameters. Note the serial port and baud rate of your project
# may vary. Our default baud rate is 9600
def motor_init():
    ser=serial.Serial()
    ser.port = "COM8"
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
def send(ser,command):
        ser.write((command+'\r').encode())
        response = ser.read(15).decode()
        if len(response) > 0:
            #print (response)
            return response
            ser.flushInput()

def motor_setup(ser):
        """
        Setup initial motor parameters, also resets alarm
        """
        send(ser,'2EG51200') # Sets microstepping to 20,000 steps per revolution
        send(ser,'2EF1') # detect stall -> reflects in status command RS=AED as alarm
        send(ser,'2EF2') # prevent stall 
        send(ser,'2IFD') # Sets the format of drive responses to decimal
        send(ser,'2SP0') # Sets the starting position at 0
        send(ser,'2AR') # Alarm reset
        send(ser,'2AC1') # Acceleration 
        send(ser,'2DE0.2') # Deceleration
        send(ser,'2VE5') # Velocity 
        send(ser,'2ME')  # Enable Motor
        send(ser,'2IP')


def tiltBySteps(ser, nsteps):
        moveCmd = '2FL' + str(nsteps)
        send(ser, moveCmd) # Moves CW -> nsteps +
        # nsteps -> -ve -> move CCW
        """
        This section demonstrates the drives ability to poll immediate position
        and check status to see if the move is done.
        """
        time.sleep(0.1)
        return send(ser, '2IP') # IP is immediate position
        #time.sleep(0.2)
        #send(ser,'2IP')
        #time.sleep(0.1)
        #send(ser,'2RS') # We end by requesting the status of the drive.

