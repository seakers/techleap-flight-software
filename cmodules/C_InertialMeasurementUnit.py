from Basilisk.moduleTemplates import InertialMeasurementUnit


class C_InertialMeasurementUnit:

    def __init__(self):
        self.module = InertialMeasurementUnit.InertialMeasurementUnit()

    def get_module(self):
        return self.module
