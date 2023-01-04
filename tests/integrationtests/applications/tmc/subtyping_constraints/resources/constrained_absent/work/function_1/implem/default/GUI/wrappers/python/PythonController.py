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
i_PI_2 = ctypes.c_int.in_dll(PythonAccess, "ii_PI_2").value
i_PI_1 = ctypes.c_int.in_dll(PythonAccess, "ii_PI_1").value
SendTC_PI_1 = PythonAccess.SendTC_PI_1

class Poll_function_1(threading.Thread):
    def run(self):
        self._bDie = False
        while True:
            if self._bDie:
                return
            queue_name = str(os.geteuid()) + "_function_1_PI_Python_queue"
            self._msgQueue = OpenMsgQueueForReading(queue_name.encode("utf-8"))
            if (self._msgQueue != -1): break
            print("Communication channel over %d_function_1_PI_Python_queue not established yet...\n" % os.geteuid())
            time.sleep(1)
        bufferSize = GetMsgQueueBufferSize(self._msgQueue)
        self._pMem = ctypes.create_string_buffer(bufferSize).raw
        while not self._bDie:
            self.messageReceivedType = RetrieveMessageFromQueue(self._msgQueue, bufferSize, self._pMem)
            if self.messageReceivedType == -1:
                time.sleep(0.01)
                continue
            ProcessTM(self)

def Invoke_PI_1(var_MySeqOf):
    if -1 == SendTC_PI_1(var_MySeqOf._ptr):
        print('Failed to send TC: PI_1...\n')
        raise IOError("PI_1")

def ProcessTM(self):
    if self.messageReceivedType == i_PI_2:
        print("\n"+chr(27)+"[32m" + "Received Telemetry: PI_2" + chr(27) + "[0m\n")
        backup = self._pMem
        # Read the data for param p1
        var_p1 = dataview_uniq_asn.MyChoice()
        var_p1.SetData(self._pMem)
        print("Parameter p1:")
        var_p1.PrintAll()
        print()
        # self._pMem = DV.MovePtrBySizeOf_MyChoice(self._pMem)
        # Revert the pointer to start of the data
        self._pMem = backup

if __name__ == "__main__":
    poll_function_1 = Poll_function_1()
    poll_function_1.start()
    try:
        time.sleep(1e8)
    except:
        poll_function_1._bDie = True
        poll_function_1.join()