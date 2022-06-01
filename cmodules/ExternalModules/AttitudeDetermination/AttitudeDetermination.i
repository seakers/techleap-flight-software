// -----------------------
// ----- Swig Module -----
// -----------------------

%module AttitudeDetermination
%{
   #include "AttitudeDetermination.h"
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
%include "AttitudeDetermination.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ControllerModeMsgPayload.h"
struct ControllerModeMsg_C;

%include "msgPayloadDefC/InertialMeasurementUnitOutMsgPayload.h"
struct InertialMeasurementUnitOutMsg_C;

%include "msgPayloadDefC/GpsOutMsgPayload.h"
struct GpsOutMsg_C;

%include "msgPayloadDefC/AttitudeDeterminationAnglesMsgPayload.h"
struct AttitudeDeterminationAnglesMsg_C;

%include "msgPayloadDefC/GeoTrackingMsgPayload.h"
struct GeoTrackingMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
