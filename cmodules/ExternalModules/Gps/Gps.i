// -----------------------
// ----- Swig Module -----
// -----------------------

%module Gps
%{
   #include "Gps.h"
%}

%pythoncode %{
from Basilisk.architecture.swig_common_model import *
%}


// ---------------------------
// ----- Parameter Types -----
// ---------------------------

%include "std_string.i"
%include "swig_conly_data.i"
%include "swig_eigen.i"


// ------------------
// ----- Module -----
// ------------------

%include "sys_model.h"
%include "Gps.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/GpsOutMsgPayload.h"
struct GpsOutMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
