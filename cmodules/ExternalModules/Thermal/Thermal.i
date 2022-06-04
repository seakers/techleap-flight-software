// -----------------------
// ----- Swig Module -----
// -----------------------

%module Thermal
%{
   #include "Thermal.h"
%}

%pythoncode %{
from Basilisk.architecture.swig_common_model import *
%}


// ---------------------------
// ----- Parameter Types -----
// ---------------------------

%include "std_string.i"
%include "swig_conly_data.i"

// ------------------
// ----- Module -----
// ------------------

%include "sys_model.h"
%include "Thermal.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ThermalMsgPayload.h"
struct ThermalMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
