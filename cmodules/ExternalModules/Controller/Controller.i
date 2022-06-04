// -----------------------
// ----- Swig Module -----
// -----------------------

%module Controller
%{
   #include "Controller.h"
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
%include "Controller.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/MessageConsumerMsgPayload.h"
struct MessageConsumerMsg_C;

%include "msgPayloadDefC/ControllerModeMsgPayload.h"
struct ControllerModeMsg_C;

%include "msgPayloadDefC/ControllerManualAnglesMsgPayload.h"
struct ControllerManualAnglesMsg_C;



%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
