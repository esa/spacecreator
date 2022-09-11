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
import dataview_uniq_asn
i_DLCRxError = ctypes.c_int.in_dll(PythonAccess, "ii_DLCRxError").value
i_DLCRxSDU = ctypes.c_int.in_dll(PythonAccess, "ii_DLCRxSDU").value
i_DLCRxReset = ctypes.c_int.in_dll(PythonAccess, "ii_DLCRxReset").value
SendTC_DLCRxReset = PythonAccess.SendTC_DLCRxReset

class Poll_rtapp(threading.Thread):
    def run(self):
        self._bDie = False
        while True:
            if self._bDie:
                return
            queue_name = str(os.geteuid()) + "_rtapp_PI_Python_queue"
            self._msgQueue = OpenMsgQueueForReading(queue_name.encode("utf-8"))
            if (self._msgQueue != -1): break
            print("Communication channel over %d_rtapp_PI_Python_queue not established yet...\n" % os.geteuid())
            time.sleep(1)
        bufferSize = GetMsgQueueBufferSize(self._msgQueue)
        self._pMem = ctypes.create_string_buffer(bufferSize).raw
        while not self._bDie:
            self.messageReceivedType = RetrieveMessageFromQueue(self._msgQueue, bufferSize, self._pMem)
            if self.messageReceivedType == -1:
                time.sleep(0.01)
                continue
            ProcessTM(self)

def Invoke_DLCRxReset(var_T_Null_Record):
    if -1 == SendTC_DLCRxReset(var_T_Null_Record._ptr):
        print('Failed to send TC: DLCRxReset...\n')
        raise IOError("DLCRxReset")

def ProcessTM(self):
    if self.messageReceivedType == i_DLCRxError:
        print("\n"+chr(27)+"[32m" + "Received Telemetry: DLCRxError" + chr(27) + "[0m\n")
        backup = self._pMem
        # Read the data for param p1
        var_p1 = dataview_uniq_asn.T_UART_DLC_RxErrorCode()
        var_p1.SetData(self._pMem)
        print("Parameter p1:")
        var_p1.PrintAll()
        print()
        # self._pMem = DV.MovePtrBySizeOf_T_UART_DLC_RxErrorCode(self._pMem)
        # Revert the pointer to start of the data
        self._pMem = backup
    if self.messageReceivedType == i_DLCRxSDU:
        print("\n"+chr(27)+"[32m" + "Received Telemetry: DLCRxSDU" + chr(27) + "[0m\n")
        backup = self._pMem
        # Read the data for param p1
        var_p1 = dataview_uniq_asn.T_UART_DLC_SDU()
        var_p1.SetData(self._pMem)
        print("Parameter p1:")
        var_p1.PrintAll()
        print()
        # self._pMem = DV.MovePtrBySizeOf_T_UART_DLC_SDU(self._pMem)
        # Revert the pointer to start of the data
        self._pMem = backup

if __name__ == "__main__":
    poll_rtapp = Poll_rtapp()
    poll_rtapp.start()
    try:
        time.sleep(1e8)
    except:
        poll_rtapp._bDie = True
        poll_rtapp.join()