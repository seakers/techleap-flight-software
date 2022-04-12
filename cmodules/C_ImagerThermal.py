from Basilisk.moduleTemplates import ImagerThermal


class C_ImagerThermal:

    def __init__(self):
        self.module = ImagerThermal.ImagerThermal()

    def get_module(self):
        return self.module
