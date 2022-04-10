import sys
import os

# --> Basilisk Imports
from Basilisk.utilities import SimulationBaseClass, macros
from Basilisk import __path__
bskPath = __path__[0]
fileName = os.path.basename(os.path.splitext(__file__)[0])

# --> Module imports
sys.path.insert(1, '/app')
from modules.Template.Module import Template


from simulation.api import SimulationClient


def run():
    sim_client = SimulationClient(time_step=1.0, duration=3.0)
    sim_client.new_py_module(Template('TemplateModule'))
    sim_client.run()





if __name__ == "__main__":
    run()
