#include "transport.h"

#include <cstring>

#include <Queue.h>
#include <Request.h>

extern "C"
{
#include <Broker.h>
}

// sporadic required

// sporadic provided


void deliver_to_actuator_controlsignal(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<ACTUATOR_CONTROLSIGNAL_REQUEST_SIZE> actuator_controlsignal_Global_Queue;
    actuator_controlsignal_Global_Queue.put(sender_pid, data, length);
}




void deliver_to_egse_tm(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<EGSE_TM_REQUEST_SIZE> egse_tm_Global_Queue;
    egse_tm_Global_Queue.put(sender_pid, data, length);
}




void deliver_to_systemcontrol_reading(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<SYSTEMCONTROL_READING_REQUEST_SIZE> systemcontrol_reading_Global_Queue;
    systemcontrol_reading_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_systemcontrol_reporting(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<SYSTEMCONTROL_REPORTING_REQUEST_SIZE> systemcontrol_reporting_Global_Queue;
    systemcontrol_reporting_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_systemcontrol_tc(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<SYSTEMCONTROL_TC_REQUEST_SIZE> systemcontrol_tc_Global_Queue;
    systemcontrol_tc_Global_Queue.put(sender_pid, data, length);
}



void initialize_transport()
{
}
