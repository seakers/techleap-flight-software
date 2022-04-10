import os
import sys

# --> Basilisk Imports
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__

# --> Parent Import
sys.path.insert(1, '/app')
from modules.AbstractModule import AbstractModule


class Template(AbstractModule):

    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(Template, self).__init__(modelName, modelActive, modelPriority)

        # --> Messages IN
        self.msg_in = messaging.CModuleTemplateMsgReader()

        # --> Messages OUT
        self.msg_out = messaging.CModuleTemplateMsg()

    ##########################
    ######## Override ########
    ##########################
    # def reset(self, currentTime):
    #     return None
    #
    # def updateState(self, currentTime):
    #     return None











