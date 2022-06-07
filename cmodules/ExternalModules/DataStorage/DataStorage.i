// -----------------------
// ----- Swig Module -----
// -----------------------

%module DataStorage
%{
   #include "DataStorage.h"
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
%include "DataStorage.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ControllerModeMsgPayload.h"
struct ControllerModeMsg_C;

%include "msgPayloadDefC/ImagerThermalOutMsgPayload.h"
struct ImagerThermalOutMsg_C;

%include "msgPayloadDefC/ImagerVNIROutMsgPayload.h"
struct ImagerVNIROutMsg_C;

%include "msgPayloadDefC/FinePredictionMsgPayload.h"
struct FinePredictionMsg_C;

%include "msgPayloadDefC/GeoTrackingMsgPayload.h"
struct GeoTrackingMsg_C;

%include "msgPayloadDefC/IMUOutMsgPayload.h"
struct IMUOutMsg_C;

%include "msgPayloadDefC/MessageConsumerMsgPayload.h"
struct MessageConsumerMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
