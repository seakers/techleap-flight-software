# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, unitTestSupport, macros, vizSupport
from Basilisk.simulation import spacecraft, extForceTorque, simpleNav
from Basilisk.fswAlgorithms import mrpFeedback, inertial3D, attTrackingError
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__

bskPath = __path__[0]






class Gimbal(simulationArchTypes.PythonModelClass):


    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(Gimbal, self).__init__(modelName, modelActive, modelPriority)

        # --> Messages
        self.classificationInMsg = messaging.CModuleTemplateMsgReader()


    def reset(self, currentTime):
        return None

    def updateState(self, currentTime):

        classificationMsgBuffer = self.classificationInMsg()
        flattened_classified_image = classificationMsgBuffer.dataVector
        print('--> Image to gimbal on:', flattened_classified_image)
