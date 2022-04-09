# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, unitTestSupport, macros, vizSupport
from Basilisk.simulation import spacecraft, extForceTorque, simpleNav
from Basilisk.fswAlgorithms import mrpFeedback, inertial3D, attTrackingError
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__

import numpy as np
import pickle
import torch
import os
import sys

bskPath = __path__[0]

sys.path.insert(1, '/app')

from neuralnet.test_trainednet import CNN, FCN8s




class Classifier(simulationArchTypes.PythonModelClass):


    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(Classifier, self).__init__(modelName, modelActive, modelPriority)

        # --> Messages
        self.imageInMsg = messaging.CModuleTemplateMsgReader()
        self.classificationOutMsg = messaging.CModuleTemplateMsg()

        # --> ML Model
        torch.cuda.empty_cache()
        DEVICE = "cuda"
        batch_size = 1
        n_class = 2
        thres = torch.Tensor([.666]).to(DEVICE)  # try: 0, -.2, -.1, .1, .2, .3, .4
        flnm = "666"
        dataset_dir = "/app/neuralnet/landsat_datasets_manual_5bands/"
        full_dataset = []
        for str in os.listdir(dataset_dir):
            f = open(dataset_dir + str, "rb")
            dataset = pickle.load(f)
            f.close()
            full_dataset.extend(dataset)


        fifteenpercent = int(len(full_dataset) * 0.15)
        train_set, val_set = torch.utils.data.random_split(full_dataset, [len(full_dataset) - fifteenpercent, fifteenpercent])
        test_loader = torch.utils.data.DataLoader(dataset=val_set, batch_size=batch_size, shuffle=False)
        cnn_model = CNN().to(DEVICE)
        self.model = FCN8s(pretrained_net=cnn_model, n_class=n_class).to(DEVICE)
        self.model.load_state_dict(torch.load('./trained_net.p'))
        self.model.eval()

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
        image_tensor = torch.FloatTensor(flattened_image)






        return flattened_image

    def flatten_image(self, image):
        return image


    def classify(self, image):


        return image
