from Basilisk.moduleTemplates import DataStorage


class C_DataStorage:

    def __init__(self):
        self.module = DataStorage.DataStorage()

    def get_module(self):
        return self.module
