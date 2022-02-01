import matplotlib.pyplot as plt
import numpy as np
import sys
import os

# --> Basilisk Imports
from Basilisk.utilities import simulationArchTypes

from Basilisk.utilities import SimulationBaseClass, unitTestSupport, macros, vizSupport
from Basilisk.simulation import spacecraft, extForceTorque, simpleNav
from Basilisk.fswAlgorithms import mrpFeedback, inertial3D, attTrackingError
from Basilisk.architecture import messaging
from Basilisk.moduleTemplates import cModuleTemplate, cppModuleTemplate
from Basilisk import __path__
bskPath = __path__[0]
fileName = os.path.basename(os.path.splitext(__file__)[0])


# --> Module Imports
# from modules.adcs import ADCS
# from modules.comms import CommsSystem
# from modules.payload import PayloadSystem
# from modules.power import PowerSystem
# from modules.thermal import ThermalSystem

class PythonMRPPD(simulationArchTypes.PythonModelClass):


    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(PythonMRPPD, self).__init__(modelName, modelActive, modelPriority)

        # Proportional gain term used in control
        self.K = 0
        # Derivative gain term used in control
        self.P = 0
        # Input guidance structure message
        self.guidInMsg = messaging.AttGuidMsgReader()
        # Output body torque message name
        self.cmdTorqueOutMsg = messaging.CmdTorqueBodyMsg()

    def reset(self, currentTime):
        return


    def updateState(self, currentTime):
        # read input message
        guidMsgBuffer = self.guidInMsg()

        # create output message buffer
        torqueOutMsgBuffer = messaging.CmdTorqueBodyMsgPayload()

        # compute control solution
        lrCmd = np.array(guidMsgBuffer.sigma_BR) * self.K + np.array(guidMsgBuffer.omega_BR_B) * self.P
        torqueOutMsgBuffer.torqueRequestBody = (-lrCmd).tolist()

        self.cmdTorqueOutMsg.write(torqueOutMsgBuffer, currentTime, self.moduleID)

        def print_output():
            """Sample Python module method"""
            print(currentTime * 1.0E-9)
            print(torqueOutMsgBuffer.torqueRequestBody)
            print(guidMsgBuffer.sigma_BR)
            print(guidMsgBuffer.omega_BR_B)

        return






def run():
    print('--> SIMULATING FLIGHT SOFTWARE')
    process_name = 'OBC'



    # --> 1. Create a new simulation module
    sim = SimulationBaseClass.SimBaseClass()
    sim_process = sim.CreateNewProcess(process_name)



    # --> 2. Create processes (executed in order created, have priority)
    proc_fsw = sim.CreateNewProcess('FSW')
    proc_adcs = sim.CreateNewProcess('ADCS')

    return 0

def create_model():
    #  Create a sim module as an empty container
    #  Create a sim module as an empty container
    #  Create a sim module as an empty container

    scSim = SimulationBaseClass.SimBaseClass()

    dynProcess = scSim.CreateNewProcess("dynamicsProcess")

    dynProcess.addTask(scSim.CreateNewTask("dynamicsTask", macros.sec2nano(1.)))

    mod1 = cModuleTemplate.cModuleTemplateConfig()
    mod1Wrap = scSim.setModelDataWrap(mod1)
    mod1Wrap.ModelTag = "cModule1"
    scSim.AddModelToTask("dynamicsTask", mod1Wrap, mod1)
    mod1.dummy = -10
    print(mod1.dummy)


    scSim.InitializeSimulation()
    print(mod1.dummy)


    scSim.TotalSim.SingleStepProcesses()
    print(mod1.dummy)

def connecting_messages():
    scSim = SimulationBaseClass.SimBaseClass()

    dynProcess = scSim.CreateNewProcess("dynamicsProcess")

    dynProcess.addTask(scSim.CreateNewTask("dynamicsTask", macros.sec2nano(5.)))

    mod1 = cModuleTemplate.cModuleTemplateConfig()
    mod1Wrap = scSim.setModelDataWrap(mod1)
    mod1Wrap.ModelTag = "cModule1"

    mod2 = cppModuleTemplate.CppModuleTemplate()
    mod2.ModelTag = "cppModule2"

    scSim.AddModelToTask("dynamicsTask", mod1Wrap, mod1)
    scSim.AddModelToTask("dynamicsTask", mod2)

    # --> SUBSCRIBE
    mod2.dataInMsg.subscribeTo(mod1.dataOutMsg)
    mod1.dataInMsg.subscribeTo(mod2.dataOutMsg)

    scSim.InitializeSimulation()
    scSim.ConfigureStopTime(macros.sec2nano(5.0))


    scSim.ExecuteSimulation()
    return 0

def recording_messages():
    scSim = SimulationBaseClass.SimBaseClass()
    dynProcess = scSim.CreateNewProcess("dynamicsProcess")
    dynProcess.addTask(scSim.CreateNewTask("dynamicsTask", macros.sec2nano(1.)))



    mod1 = cModuleTemplate.cModuleTemplateConfig()
    mod1Wrap = scSim.setModelDataWrap(mod1)
    mod1Wrap.ModelTag = "cModule1"
    scSim.AddModelToTask("dynamicsTask", mod1Wrap, mod1)
    mod1.dataInMsg.subscribeTo(mod1.dataOutMsg)




    msgRec = mod1.dataOutMsg.recorder()
    scSim.AddModelToTask("dynamicsTask", msgRec)



    msgRec2 = mod1.dataOutMsg.recorder(macros.sec2nano(20.))
    scSim.AddModelToTask("dynamicsTask", msgRec2)




    scSim.InitializeSimulation()
    scSim.ConfigureStopTime(macros.sec2nano(60.0))
    scSim.ExecuteSimulation()
    plt.close("all")
    plt.figure(1)
    figureList = {}
    for idx in range(3):
        plt.plot(msgRec.times() * macros.NANO2SEC, msgRec.dataVector[:, idx],
            color = unitTestSupport.getLineColor(idx, 3),
            label = '$x_{' + str(idx) + '}$')
        plt.plot(msgRec2.times() * macros.NANO2SEC, msgRec2.dataVector[:, idx], '--',
            color = unitTestSupport.getLineColor(idx, 3),
            label = r'$\hat x_{' + str(idx) + '}$')
        plt.legend(loc='lower right')
        plt.xlabel('Time [sec]')
        plt.ylabel('Module Data [units]')
        if "pytest" not in sys.modules:
            plt.show()
        figureList["bsk-4"] = plt.figure(1)
        plt.close("all")
        return figureList

def stand_alone_message():
    scSim = SimulationBaseClass.SimBaseClass()
    dynProcess = scSim.CreateNewProcess("dynamicsProcess")
    dynProcess.addTask(scSim.CreateNewTask("dynamicsTask", macros.sec2nano(1.)))

    # create modules
    mod1 = cppModuleTemplate.CppModuleTemplate()
    mod1.ModelTag = "cppModule1"
    scSim.AddModelToTask("dynamicsTask", mod1)

    # create stand-alone input message
    msgData = messaging.CModuleTemplateMsgPayload()
    msgData.dataVector = [1., 2., 3.]
    msg = messaging.CModuleTemplateMsg().write(msgData)

    # connect to stand-alone msg
    mod1.dataInMsg.subscribeTo(msg)

    return 0

def get_spacecraft():
    scObject = spacecraft.Spacecraft()
    scObject.ModelTag = "bsk-Sat"
    # define the simulation inertia
    I = [900., 0., 0.,
         0., 800., 0.,
         0., 0., 600.]
    scObject.hub.mHub = 750.0  # kg - spacecraft mass
    scObject.hub.r_BcB_B = [[0.0], [0.0], [0.0]]  # m - position vector of body-fixed point B relative to CM
    scObject.hub.IHubPntBc_B = unitTestSupport.np2EigenMatrix3d(I)
    scObject.hub.sigma_BNInit = [[0.1], [0.2], [-0.3]]  # sigma_BN_B
    scObject.hub.omega_BN_BInit = [[0.001], [-0.01], [0.03]]  # rad/s - omega_BN_B
    return scObject




def py_test():

    # Create simulation variable names
    simTaskName = "simTask"
    simProcessName = "simProcess"
    pyTaskName = "pyTask"
    pyProcessName = "pyProcess"



    #  Create a sim module as an empty container
    scSim = SimulationBaseClass.SimBaseClass()
    simulationTime = macros.min2nano(10.)
    simulationTimeStep = macros.sec2nano(.1)

    #
    #  create the simulation process
    #
    dynProcess = scSim.CreateNewProcess(simProcessName, 10)
    dynProcess.addTask(scSim.CreateNewTask(simTaskName, simulationTimeStep))

    #
    #  create the python process
    #
    pyModulesProcess = scSim.CreateNewPythonProcess(pyProcessName, 9)
    pyModulesProcess.createPythonTask(pyTaskName, simulationTimeStep)




    # ---------- 5 Predefined models

    # Spacecraft Module
    scObject = get_spacecraft()
    scSim.AddModelToTask(simTaskName, scObject)

    # Disturbance Module
    extFTObject = extForceTorque.ExtForceTorque()
    extFTObject.ModelTag = "externalDisturbance"
    scObject.addDynamicEffector(extFTObject)
    scSim.AddModelToTask(simTaskName, extFTObject)

    # add the simple Navigation sensor module.  This sets the SC attitude, rate, position
    sNavObject = simpleNav.SimpleNav()
    sNavObject.ModelTag = "SimpleNavigation"
    scSim.AddModelToTask(simTaskName, sNavObject)

    #
    #   setup the FSW algorithm tasks
    #

    # setup inertial3D guidance module
    inertial3DConfig = inertial3D.inertial3DConfig()
    inertial3DWrap = scSim.setModelDataWrap(inertial3DConfig)
    inertial3DWrap.ModelTag = "inertial3D"
    scSim.AddModelToTask(simTaskName, inertial3DWrap, inertial3DConfig)
    inertial3DConfig.sigma_R0N = [0., 0., 0.]  # set the desired inertial orientation

    # setup the attitude tracking error evaluation module
    attErrorConfig = attTrackingError.attTrackingErrorConfig()
    attErrorWrap = scSim.setModelDataWrap(attErrorConfig)
    attErrorWrap.ModelTag = "attErrorInertial3D"
    scSim.AddModelToTask(simTaskName, attErrorWrap, attErrorConfig)

    # ----------



    # setup Python MRP PD control module
    pyMRPPD = PythonMRPPD("pyMRP_PD", True, 100)
    pyMRPPD.K = 3.5
    pyMRPPD.P = 30.0
    pyModulesProcess.addModelToTask(pyTaskName, pyMRPPD)

    #
    #   Setup data logging before the simulation is initialized
    #
    numDataPoints = 50
    samplingTime = unitTestSupport.samplingTime(simulationTime, simulationTimeStep, numDataPoints)
    attErrorLog = attErrorConfig.attGuidOutMsg.recorder(samplingTime)
    mrpLog = pyMRPPD.cmdTorqueOutMsg.recorder(samplingTime)
    scSim.AddModelToTask(simTaskName, attErrorLog)
    scSim.AddModelToTask(simTaskName, mrpLog)

    #
    # connect the messages to the modules
    #
    sNavObject.scStateInMsg.subscribeTo(scObject.scStateOutMsg)
    attErrorConfig.attNavInMsg.subscribeTo(sNavObject.attOutMsg)
    attErrorConfig.attRefInMsg.subscribeTo(inertial3DConfig.attRefOutMsg)



    # Py module connections
    pyMRPPD.guidInMsg.subscribeTo(attErrorConfig.attGuidOutMsg)
    extFTObject.cmdTorqueInMsg.subscribeTo(pyMRPPD.cmdTorqueOutMsg)



    # if this scenario is to interface with the BSK Viz, uncomment the following lines
    vizSupport.enableUnityVisualization(scSim, simTaskName, scObject
                                        # , saveFile=fileName
                                        )

    #
    #   initialize Simulation
    #
    scSim.InitializeSimulation()

    #
    #   configure a simulation stop time time and execute the simulation run
    #
    scSim.ConfigureStopTime(simulationTime)
    scSim.ExecuteSimulation()

    #
    #   retrieve the logged data
    #
    dataLr = mrpLog.torqueRequestBody
    dataSigmaBR = attErrorLog.sigma_BR
    dataOmegaBR = attErrorLog.omega_BR_B
    timeAxis = attErrorLog.times()
    np.set_printoptions(precision=16)

    #
    #   plot the results
    #
    plt.close("all")  # clears out plots from earlier test runs
    plt.figure(1)
    for idx in range(3):
        plt.plot(timeAxis * macros.NANO2MIN, dataSigmaBR[:, idx],
                 color=unitTestSupport.getLineColor(idx, 3),
                 label=r'$\sigma_' + str(idx) + '$')
    plt.legend(loc='lower right')
    plt.xlabel('Time [min]')
    plt.ylabel(r'Attitude Error $\sigma_{B/R}$')
    figureList = {}
    pltName = fileName + "1"
    figureList[pltName] = plt.figure(1)

    plt.figure(2)
    for idx in range(3):
        plt.plot(timeAxis * macros.NANO2MIN, dataLr[:, idx],
                 color=unitTestSupport.getLineColor(idx, 3),
                 label='$L_{r,' + str(idx) + '}$')
    plt.legend(loc='lower right')
    plt.xlabel('Time [min]')
    plt.ylabel('Control Torque $L_r$ [Nm]')
    pltName = fileName + "2"
    figureList[pltName] = plt.figure(2)

    plt.figure(3)
    for idx in range(3):
        plt.plot(timeAxis * macros.NANO2MIN, dataOmegaBR[:, idx],
                 color=unitTestSupport.getLineColor(idx, 3),
                 label=r'$\omega_{BR,' + str(idx) + '}$')
    plt.legend(loc='lower right')
    plt.xlabel('Time [min]')
    plt.ylabel('Rate Tracking Error [rad/s] ')

    plt.show()

    # close the plots being saved off to avoid over-writing old and new figures
    plt.close("all")

    return figureList














if __name__ == "__main__":
    py_test()