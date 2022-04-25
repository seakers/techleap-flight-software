// -----------------------
// ----- Swig Module -----
// -----------------------

%module FineNN
%{
   #include "FineNN.h"
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
%include "FineNN.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
struct ImagerThermalOutMsg_C;

%include "msgPayloadDefC/CoarsePredictionMsgPayload.h"
struct CoarsePredictionMsg_C;

%include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
struct ImagerVNIROutMsg_C;

%include "msgPayloadDefC/FinePredictionMsgPayload.h"
struct FinePredictionMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
