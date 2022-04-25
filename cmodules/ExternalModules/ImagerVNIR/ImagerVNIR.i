

// -----------------------
// ----- Swig Module -----
// -----------------------


%module ImagerVNIR
%{
   #include "ImagerVNIR.h"
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
%include "ImagerVNIR.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
struct ImagerVNIROutMsg_C;


%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
