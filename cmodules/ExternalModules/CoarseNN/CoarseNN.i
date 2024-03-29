// -----------------------
// ----- Swig Module -----
// -----------------------

%module CoarseNN
%{
   #include "CoarseNN.h"
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
%include "CoarseNN.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ControllerModeMsgPayload.h"
struct ControllerModeMsg_C;

%include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
struct ImagerVNIROutMsg_C;

%include "msgPayloadDefC/CoarsePredictionMsgPayload.h"
struct CoarsePredictionMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
