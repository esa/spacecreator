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
i_CorruptChar = ctypes.c_int.in_dll(PythonAccess, "ii_CorruptChar").value
SendTC_CorruptChar = PythonAccess.SendTC_CorruptChar
i_DropChar = ctypes.c_int.in_dll(PythonAccess, "ii_DropChar").value
SendTC_DropChar = PythonAccess.SendTC_DropChar


def Invoke_CorruptChar(var_UINT8):
    if -1 == SendTC_CorruptChar(var_UINT8._ptr):
        print('Failed to send TC: CorruptChar...\n')
        raise IOError("CorruptChar")

def Invoke_DropChar(var_T_Null_Record):
    if -1 == SendTC_DropChar(var_T_Null_Record._ptr):
        print('Failed to send TC: DropChar...\n')
        raise IOError("DropChar")

def ProcessTM(self):
    pass


