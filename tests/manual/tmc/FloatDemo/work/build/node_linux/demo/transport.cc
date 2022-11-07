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
    extern taste::Queue<ACTUATOR_CONTROLSIGNAL_REQUEST_SIZE> actuator_controlSignal_Global_Queue;
    actuator_controlSignal_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_controlsystem_configuration(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<CONTROLSYSTEM_CONFIGURATION_REQUEST_SIZE> controlsystem_configuration_Global_Queue;
    controlsystem_configuration_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_controlsystem_reading(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<CONTROLSYSTEM_READING_REQUEST_SIZE> controlsystem_reading_Global_Queue;
    controlsystem_reading_Global_Queue.put(sender_pid, data, length);
}




void deliver_to_host_tm(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<HOST_TM_REQUEST_SIZE> host_tm_Global_Queue;
    host_tm_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_sensor_tc(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<SENSOR_TC_REQUEST_SIZE> sensor_tc_Global_Queue;
    sensor_tc_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_sensor_trigger(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<SENSOR_TRIGGER_REQUEST_SIZE> sensor_trigger_Global_Queue;
    sensor_trigger_Global_Queue.put(sender_pid, data, length);
}



void initialize_transport()
{
}
