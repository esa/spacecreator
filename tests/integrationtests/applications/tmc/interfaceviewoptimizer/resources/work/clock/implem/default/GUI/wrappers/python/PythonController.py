from __future__ import absolute_import


import threading, time, sys, os, ctypes

import DV
try:
    PythonAccess = ctypes.cdll.LoadLibrary("./PythonAccess.so")
except OSError:
    folder = os.path.dirname(os.path.abspath(__file__))
    PythonAccess = ctypes.cdll.LoadLibrary(folder + "/PythonAccess.so")
    sys.path.append(folder)
OpenMsgQueueForReading = PythonAccess.OpenMsgQueueForReading
OpenMsgQueueForReading.restype = ctypes.c_int
CloseMsgQueue =  PythonAccess.CloseMsgQueue
GetMsgQueueBufferSize = PythonAccess.GetMsgQueueBufferSize
GetMsgQueueBufferSize.restype = ctypes.c_int
RetrieveMessageFromQueue = PythonAccess.RetrieveMessageFromQueue
RetrieveMessageFromQueue.restype = ctypes.c_int
i_ClockInterface = ctypes.c_int.in_dll(PythonAccess, "ii_ClockInterface").value
SendTC_ClockInterface = PythonAccess.SendTC_ClockInterface


def Invoke_ClockInterface(var_T_Null_Record):
    if -1 == SendTC_ClockInterface(var_T_Null_Record._ptr):
        print('Failed to send TC: ClockInterface...\n')
        raise IOError("ClockInterface")

def ProcessTM(self):
    pass


