import os
import sys

# --> Basilisk Imports
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__

# --> Parent Import
sys.path.insert(1, '/app')
from pymodules.AbstractModule import AbstractModule


class Mock(AbstractModule):

    def __init__(self, modelName, modelActive=True, modelPriority=-1, mode='default'):
        super(Mock, self).__init__(modelName, modelActive, modelPriority)
        self.mode = mode

        # --> Messages IN
        self.msg_in = messaging.CModuleTemplateMsgReader()

        # --> Messages OUT
        self.msg_out = messaging.CModuleTemplateMsg()

    ##########################
    ######## Override ########
    ##########################
    def reset(self, currentTime):
        return None

    def updateState(self, currentTime):
        update_func = getattr(self, self.mode, None)
        if callable(update_func):
            print('--> UPDATE FUNCTION:', self.m_name)
            update_func(currentTime)
        else:
            print('--> MOCK UPDATE FUNC DOES NOT EXIST:', self.m_name, self.mode)

    ##########################
    ########## Mock ##########
    ##########################
    def mtest_m1(self, currentTime):
        message = [0.0, 0.0, 0.0]
        print(message)
        self.write_parent_message(currentTime, message)

    def mtest_m2(self, currentTime):
        message = self.get_parent_message()
        message[0] += 1
        print(message)
        self.write_parent_message(currentTime, message)

    def mtest_m3(self, currentTime):
        message = self.get_parent_message()
        message[0] += 1
        print(message)











