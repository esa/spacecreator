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
i_DLCTxReset = ctypes.c_int.in_dll(PythonAccess, "ii_DLCTxReset").value
SendTC_DLCTxReset = PythonAccess.SendTC_DLCTxReset
i_DLCTxSDU = ctypes.c_int.in_dll(PythonAccess, "ii_DLCTxSDU").value
SendTC_DLCTxSDU = PythonAccess.SendTC_DLCTxSDU


def Invoke_DLCTxReset(var_T_Null_Record):
    if -1 == SendTC_DLCTxReset(var_T_Null_Record._ptr):
        print('Failed to send TC: DLCTxReset...\n')
        raise IOError("DLCTxReset")

def Invoke_DLCTxSDU(var_T_UART_DLC_SDU):
    if -1 == SendTC_DLCTxSDU(var_T_UART_DLC_SDU._ptr):
        print('Failed to send TC: DLCTxSDU...\n')
        raise IOError("DLCTxSDU")

def ProcessTM(self):
    pass


