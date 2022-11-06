/* File generated by kazoo for TASTE - do not edit */
#include <cstddef>

#include "dataview-uniq.h"

extern "C"
{

// Protected provided interfaces of function Actuator1

void call_protected_Actuator1_actuatorTc
      (const char *IN_buf_tc, size_t size_IN_buf_tc);
// End of the protected provided interfaces of function Actuator1

// Required interfaces of function Actuator1

// Required interface actuatorTm in function Actuator1
// This RI is connected to: ModeManager
void vm_actuator1_actuatortm
        (asn1SccPID dest_pid,
         const char *IN_buf_tm, size_t size_IN_buf_tm);
// End of the required interfaces of function Actuator1

void actuator1_check_queue(bool* has_pending_msg);
// Protected provided interfaces of function Actuator2

void call_protected_Actuator2_actuatorTc
      (const char *IN_buf_tc, size_t size_IN_buf_tc);
// End of the protected provided interfaces of function Actuator2

// Required interfaces of function Actuator2

// Required interface actuatorTm in function Actuator2
// This RI is connected to: ModeManager
void vm_actuator2_actuatortm
        (asn1SccPID dest_pid,
         const char *IN_buf_tm, size_t size_IN_buf_tm);
// End of the required interfaces of function Actuator2

void actuator2_check_queue(bool* has_pending_msg);
// Protected provided interfaces of function EGSE

void call_protected_EGSE_Poll(void);
void call_protected_EGSE_systemTm
      (const char *IN_buf_tm, size_t size_IN_buf_tm);
// End of the protected provided interfaces of function EGSE

// Required interfaces of function EGSE

// Required interface systemTc in function EGSE
// This RI is connected to: ModeManager
void vm_egse_systemtc
        (asn1SccPID dest_pid,
         const char *IN_buf_tc, size_t size_IN_buf_tc);
// End of the required interfaces of function EGSE

void egse_check_queue(bool* has_pending_msg);
// Protected provided interfaces of function ModeManager

void call_protected_ModeManager_actuatorTm1
      (const char *IN_buf_tm, size_t size_IN_buf_tm);
void call_protected_ModeManager_actuatorTm2
      (const char *IN_buf_tm, size_t size_IN_buf_tm);
void call_protected_ModeManager_reading
      (const char *IN_buf_x, size_t size_IN_buf_x);
void call_protected_ModeManager_systemTc
      (const char *IN_buf_tc, size_t size_IN_buf_tc);
// End of the protected provided interfaces of function ModeManager

// Required interfaces of function ModeManager

// Required interface actuatorTc1 in function ModeManager
// This RI is connected to: Actuator1
void vm_modemanager_actuatortc1
        (asn1SccPID dest_pid,
         const char *IN_buf_tc, size_t size_IN_buf_tc);
// Required interface actuatorTc2 in function ModeManager
// This RI is connected to: Actuator2
void vm_modemanager_actuatortc2
        (asn1SccPID dest_pid,
         const char *IN_buf_tc, size_t size_IN_buf_tc);
// Required interface systemTm in function ModeManager
// This RI is connected to: EGSE
void vm_modemanager_systemtm
        (asn1SccPID dest_pid,
         const char *IN_buf_tm, size_t size_IN_buf_tm);
// End of the required interfaces of function ModeManager

void modemanager_check_queue(bool* has_pending_msg);
// Protected provided interfaces of function Sensor

void call_protected_Sensor_trigger(void);
// End of the protected provided interfaces of function Sensor

// Required interfaces of function Sensor

// Required interface reading in function Sensor
// This RI is connected to: ModeManager
void vm_sensor_reading
        (asn1SccPID dest_pid,
         const char *IN_buf_x, size_t size_IN_buf_x);
// End of the required interfaces of function Sensor

void sensor_check_queue(bool* has_pending_msg);

void call_actuator1_actuatortc (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// modemanager_actuatorTm1.actuatorTm1

void call_actuator2_actuatortc (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// modemanager_actuatorTm2.actuatorTm2

void call_egse_poll (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// modemanager_systemTc.systemTc

void call_egse_systemtm (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// modemanager_systemTc.systemTc

void call_modemanager_actuatortm1 (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// actuator1_actuatorTc.actuatorTc
// actuator2_actuatorTc.actuatorTc
// egse_systemTm.systemTm

void call_modemanager_actuatortm2 (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// actuator1_actuatorTc.actuatorTc
// actuator2_actuatorTc.actuatorTc
// egse_systemTm.systemTm

void call_modemanager_reading (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// actuator1_actuatorTc.actuatorTc
// actuator2_actuatorTc.actuatorTc
// egse_systemTm.systemTm

void call_modemanager_systemtc (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// actuator1_actuatorTc.actuatorTc
// actuator2_actuatorTc.actuatorTc
// egse_systemTm.systemTm

void call_sensor_trigger (const char* buf, size_t len);

// This is the list of remote threads/RIs :
// modemanager_reading.reading

}
