# --> Basilisk Imports
from Basilisk.architecture import messaging
from Basilisk.utilities import simulationArchTypes
from Basilisk import __path__



class AbstractModule(simulationArchTypes.PythonModelClass):

    def __init__(self, modelName, modelActive=True, modelPriority=-1):
        super(AbstractModule, self).__init__(modelName, modelActive, modelPriority)
        self.m_name = modelName

        # --> Messages IN
        self.p_msg_in = messaging.CModuleTemplateMsgReader()

        # --> Messages OUT
        self.p_msg_out = messaging.CModuleTemplateMsg()



    ##########################
    ######## Override ########
    ##########################
    def reset(self, currentTime):
        print('--> RESET MODULE:', self.m_name)
        return None

    def updateState(self, currentTime):
        print('--> UPDATE MODULE:', self.m_name)
        return None



    #########################
    ######## Utility ########
    #########################
    def get_name(self):
        return self.m_name

    def get_parent_message(self):
        msg_in_buffer = self.p_msg_in()
        return msg_in_buffer.dataVector

    def write_parent_message(self, current_time, data_vector=(0, 0, 0)):
        msg_out_buffer = messaging.CModuleTemplateMsgPayload()
        msg_out_buffer.dataVector = data_vector
        self.p_msg_out.write(msg_out_buffer, current_time, self.moduleID)
