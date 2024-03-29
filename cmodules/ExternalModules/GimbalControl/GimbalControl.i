// -----------------------
// ----- Swig Module -----
// -----------------------

%module GimbalControl
%{
   #include "GimbalControl.h"
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
%include "GimbalControl.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/ControllerModeMsgPayload.h"
struct ControllerModeMsg_C;

%include "msgPayloadDefC/FinePredictionMsgPayload.h"
struct FinePredictionMsg_C;

%include "msgPayloadDefC/MessageConsumerMsgPayload.h"
struct MessageConsumerMsg_C;

%include "msgPayloadDefC/MessageConsumerManualMsgPayload.h"
struct MessageConsumerManualMsg_C;

%include "msgPayloadDefC/IMUOutMsgPayload.h"
struct IMUOutMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
