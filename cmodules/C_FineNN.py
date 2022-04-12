from Basilisk.moduleTemplates import FineNN


class C_FineNN:

    def __init__(self):
        self.module = FineNN.FineNN()

    def get_module(self):
        return self.module
