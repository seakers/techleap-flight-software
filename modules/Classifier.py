# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, unitTestSupport, macros, vizSupport
from Basilisk.simulation import spacecraft, extForceTorque, simpleNav
from Basilisk.fswAlgorithms import mrpFeedback, inertial3D, attTrackingError
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__

import numpy as np

bskPath = __path__[0]






class Classifier(simulationArchTypes.PythonModelClass):


    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(Classifier, self).__init__(modelName, modelActive, modelPriority)

        # --> Messages
        self.imageInMsg = messaging.CModuleTemplateMsgReader()
        self.classificationOutMsg = messaging.CModuleTemplateMsg()

        # --> ML Model
        self.model = None


    def reset(self, currentTime):
        return None

    def updateState(self, currentTime):

        # --> 1. Get and unflatten image
        imageMsgBuffer = self.imageInMsg()
        flattened_image = imageMsgBuffer.dataVector
        image = self.unflatten_image(flattened_image)

        # --> 2. Classify image
        classified_image = self.classify(image)
        print('--> FINISHED CLASSIFICATION', classified_image)

        # --> 3. Send classified image to Gimbal module
        classificationOutMsgBuffer = messaging.CModuleTemplateMsgPayload()
        classificationOutMsgBuffer.dataVector = self.flatten_image(classified_image)
        self.classificationOutMsg.write(classificationOutMsgBuffer, currentTime, self.moduleID)





    def unflatten_image(self, flattened_image):
        return flattened_image

    def flatten_image(self, image):
        return image


    def classify(self, image):


        return image
