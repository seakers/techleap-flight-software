# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, unitTestSupport, macros, vizSupport
from Basilisk.simulation import spacecraft, extForceTorque, simpleNav
from Basilisk.fswAlgorithms import mrpFeedback, inertial3D, attTrackingError
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__

bskPath = __path__[0]






class RGBImager(simulationArchTypes.PythonModelClass):


    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(RGBImager, self).__init__(modelName, modelActive, modelPriority)
        self.gcount = 0

        # --> Spoof image data for demo
        self.images = self.load_images()

        # --> Messages
        self.imageOutMsg = messaging.CModuleTemplateMsg()


    def load_images(self):
        images = []

        


        return images



    def reset(self, currentTime):
        return None

    def updateState(self, currentTime):
        self.gcount += 1



        # --> Create output message buffer
        imageOutMsgBuffer = messaging.CModuleTemplateMsgPayload()
        payload = [self.gcount, 0, 0]
        imageOutMsgBuffer.dataVector = payload



        self.imageOutMsg.write(imageOutMsgBuffer, currentTime, self.moduleID)

        return







