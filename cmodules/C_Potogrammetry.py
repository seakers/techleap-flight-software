from Basilisk.moduleTemplates import Photogrammetry


class C_Photogrammetry:

    def __init__(self):
        self.module = Photogrammetry.Photogrammetry()

    def get_module(self):
        return self.module
