// -----------------------
// ----- Swig Module -----
// -----------------------

%module GeoTracking
%{
   #include "GeoTracking.h"
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
%include "GeoTracking.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ControllerModeMsgPayload.h"
struct ControllerModeMsg_C;

%include "msgPayloadDefC/GeoTrackingMsgPayload.h"
struct GeoTrackingMsg_C;

%include "msgPayloadDefC/FinePredictionMsgPayload.h"
struct FinePredictionMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
