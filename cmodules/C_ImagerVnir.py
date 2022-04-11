from Basilisk.moduleTemplates import cppImagerVnir


class C_ImagerVnir:

    def __init__(self):
        self.module = cppImagerVnir.CppImagerVnir()

    def get_module(self):
        return self.module
