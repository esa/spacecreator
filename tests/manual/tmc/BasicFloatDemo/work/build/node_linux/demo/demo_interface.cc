/* File generated by kazoo for TASTE - do not edit */
#include "demo_interface.h"
#include <cstring>

#include <Request.h>
#include <Lock.h>
#include <Queue.h>

#include "transport.h"
#include "routing.h"

extern "C"
{

#include "actuator_vm_if.h"

void call_actuator_controlsignal (const char* buf, size_t len)
{
    actuator_controlsignal (buf, len);
}

static bool thread_actuator_controlsignal_check_queue()
{
	extern taste::Queue<ACTUATOR_CONTROLSIGNAL_REQUEST_SIZE> actuator_controlsignal_Global_Queue;
	return !actuator_controlsignal_Global_Queue.is_empty();
}
#include "egse_vm_if.h"

void call_egse_poll (const char* buf, size_t len)
{
    extern taste::Lock egse_Mutex;

    egse_Mutex.lock();

    egse_Poll();
	egse_Mutex.unlock();
}

static bool thread_egse_poll_check_queue()
{
	extern taste::Queue<EGSE_POLL_REQUEST_SIZE> egse_Poll_Global_Queue;
	return !egse_Poll_Global_Queue.is_empty();
}
#include "egse_vm_if.h"

void call_egse_tm (const char* buf, size_t len)
{
    extern taste::Lock egse_Mutex;

    egse_Mutex.lock();

    egse_tm (buf, len);
	egse_Mutex.unlock();
}

static bool thread_egse_tm_check_queue()
{
	extern taste::Queue<EGSE_TM_REQUEST_SIZE> egse_tm_Global_Queue;
	return !egse_tm_Global_Queue.is_empty();
}
#include "sensor_vm_if.h"

void call_sensor_trigger (const char* buf, size_t len)
{
    sensor_trigger();
}

static bool thread_sensor_trigger_check_queue()
{
	extern taste::Queue<SENSOR_TRIGGER_REQUEST_SIZE> sensor_trigger_Global_Queue;
	return !sensor_trigger_Global_Queue.is_empty();
}
#include "systemcontrol_vm_if.h"

void call_systemcontrol_reading (const char* buf, size_t len)
{
    extern taste::Lock systemcontrol_Mutex;

    systemcontrol_Mutex.lock();

    systemcontrol_reading (buf, len);
	systemcontrol_Mutex.unlock();
}

static bool thread_systemcontrol_reading_check_queue()
{
	extern taste::Queue<SYSTEMCONTROL_READING_REQUEST_SIZE> systemcontrol_reading_Global_Queue;
	return !systemcontrol_reading_Global_Queue.is_empty();
}
#include "systemcontrol_vm_if.h"

void call_systemcontrol_reporting (const char* buf, size_t len)
{
    extern taste::Lock systemcontrol_Mutex;

    systemcontrol_Mutex.lock();

    systemcontrol_reporting();
	systemcontrol_Mutex.unlock();
}

static bool thread_systemcontrol_reporting_check_queue()
{
	extern taste::Queue<SYSTEMCONTROL_REPORTING_REQUEST_SIZE> systemcontrol_reporting_Global_Queue;
	return !systemcontrol_reporting_Global_Queue.is_empty();
}
#include "systemcontrol_vm_if.h"

void call_systemcontrol_tc (const char* buf, size_t len)
{
    extern taste::Lock systemcontrol_Mutex;

    systemcontrol_Mutex.lock();

    systemcontrol_tc (buf, len);
	systemcontrol_Mutex.unlock();
}

static bool thread_systemcontrol_tc_check_queue()
{
	extern taste::Queue<SYSTEMCONTROL_TC_REQUEST_SIZE> systemcontrol_tc_Global_Queue;
	return !systemcontrol_tc_Global_Queue.is_empty();
}


// Required interfaces of function Actuator

// Required interface reporting in function Actuator
void vm_actuator_reporting(asn1SccPID dest_pid)
{
    static uint8_t buffer_placeholder[1];
    if (PID_env == dest_pid || PID_systemcontrol == dest_pid)
        deliver_to_systemcontrol_reporting(PID_actuator, buffer_placeholder, 0);
}
#include "actuator_vm_if.h"


void call_protected_Actuator_controlsignal
      (const char *IN_buf_x, size_t size_IN_buf_x)

{
    extern taste::Lock actuator_Mutex;

    actuator_Mutex.lock();

    actuator_controlsignal
      (IN_buf_x, size_IN_buf_x);


    actuator_Mutex.unlock();
}

void actuator_check_queue(bool* has_pending_msg)
{
    *has_pending_msg = false;
    if(thread_actuator_controlsignal_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
}
// Required interfaces of function EGSE

// Required interface tc in function EGSE
void vm_egse_tc
        (asn1SccPID dest_pid,
         const char *IN_buf_x, size_t size_IN_buf_x)

{
    if (PID_env == dest_pid || PID_systemcontrol == dest_pid)
        deliver_to_systemcontrol_tc(PID_egse, reinterpret_cast<const uint8_t*>(IN_buf_x), size_IN_buf_x);
}
#include "egse_vm_if.h"


void call_protected_EGSE_Poll(void)
{
    extern taste::Lock egse_Mutex;

    egse_Mutex.lock();

    egse_Poll();

    egse_Mutex.unlock();
}
void call_protected_EGSE_tm
      (const char *IN_buf_y, size_t size_IN_buf_y)

{
    extern taste::Lock egse_Mutex;

    egse_Mutex.lock();

    egse_tm
      (IN_buf_y, size_IN_buf_y);


    egse_Mutex.unlock();
}

void egse_check_queue(bool* has_pending_msg)
{
    *has_pending_msg = false;
    if(thread_egse_poll_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
    if(thread_egse_tm_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
}
// Required interfaces of function Sensor

// Required interface reading in function Sensor
void vm_sensor_reading
        (asn1SccPID dest_pid,
         const char *IN_buf_x, size_t size_IN_buf_x)

{
    if (PID_env == dest_pid || PID_systemcontrol == dest_pid)
        deliver_to_systemcontrol_reading(PID_sensor, reinterpret_cast<const uint8_t*>(IN_buf_x), size_IN_buf_x);
}
#include "sensor_vm_if.h"


void call_protected_Sensor_trigger(void)
{
    extern taste::Lock sensor_Mutex;

    sensor_Mutex.lock();

    sensor_trigger();

    sensor_Mutex.unlock();
}

void sensor_check_queue(bool* has_pending_msg)
{
    *has_pending_msg = false;
    if(thread_sensor_trigger_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
}
// Required interfaces of function SystemControl

// Required interface controlsignal in function SystemControl
void vm_systemcontrol_controlsignal
        (asn1SccPID dest_pid,
         const char *IN_buf_x, size_t size_IN_buf_x)

{
    if (PID_env == dest_pid || PID_actuator == dest_pid)
        deliver_to_actuator_controlsignal(PID_systemcontrol, reinterpret_cast<const uint8_t*>(IN_buf_x), size_IN_buf_x);
}
// Required interface tm in function SystemControl
void vm_systemcontrol_tm
        (asn1SccPID dest_pid,
         const char *IN_buf_y, size_t size_IN_buf_y)

{
    if (PID_env == dest_pid || PID_egse == dest_pid)
        deliver_to_egse_tm(PID_systemcontrol, reinterpret_cast<const uint8_t*>(IN_buf_y), size_IN_buf_y);
}
#include "systemcontrol_vm_if.h"


void call_protected_SystemControl_reading
      (const char *IN_buf_x, size_t size_IN_buf_x)

{
    extern taste::Lock systemcontrol_Mutex;

    systemcontrol_Mutex.lock();

    systemcontrol_reading
      (IN_buf_x, size_IN_buf_x);


    systemcontrol_Mutex.unlock();
}
void call_protected_SystemControl_reporting(void)
{
    extern taste::Lock systemcontrol_Mutex;

    systemcontrol_Mutex.lock();

    systemcontrol_reporting();

    systemcontrol_Mutex.unlock();
}
void call_protected_SystemControl_tc
      (const char *IN_buf_x, size_t size_IN_buf_x)

{
    extern taste::Lock systemcontrol_Mutex;

    systemcontrol_Mutex.lock();

    systemcontrol_tc
      (IN_buf_x, size_IN_buf_x);


    systemcontrol_Mutex.unlock();
}

void systemcontrol_check_queue(bool* has_pending_msg)
{
    *has_pending_msg = false;
    if(thread_systemcontrol_reading_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
    if(thread_systemcontrol_reporting_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
    if(thread_systemcontrol_tc_check_queue())
    {
        *has_pending_msg = true;
        return;
    }
}
}
