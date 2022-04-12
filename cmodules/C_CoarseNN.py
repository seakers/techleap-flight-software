from Basilisk.moduleTemplates import CoarseNN


class C_CoarseNN:

    def __init__(self):
        self.module = CoarseNN.CoarseNN()

    def get_module(self):
        return self.module
