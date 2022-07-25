// -----------------------
// ----- Swig Module -----
// -----------------------

%module MessageConsumer
%{
   #include "MessageConsumer.h"
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
%include "MessageConsumer.h"


// --------------------
// ----- Messages -----
// --------------------

%include "msgPayloadDefC/MessageConsumerMsgPayload.h"
struct MessageConsumerMsg_C;

%include "msgPayloadDefC/MessageConsumerManualMsgPayload.h"
struct MessageConsumerManualMsg_C;

%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
