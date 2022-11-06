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


void deliver_to_actuator1_actuatortc(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<ACTUATOR1_ACTUATORTC_REQUEST_SIZE> actuator1_actuatorTc_Global_Queue;
    actuator1_actuatorTc_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_actuator2_actuatortc(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<ACTUATOR2_ACTUATORTC_REQUEST_SIZE> actuator2_actuatorTc_Global_Queue;
    actuator2_actuatorTc_Global_Queue.put(sender_pid, data, length);
}




void deliver_to_egse_systemtm(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<EGSE_SYSTEMTM_REQUEST_SIZE> egse_systemTm_Global_Queue;
    egse_systemTm_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_actuatortm1(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_ACTUATORTM1_REQUEST_SIZE> modemanager_actuatorTm1_Global_Queue;
    modemanager_actuatorTm1_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_actuatortm2(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_ACTUATORTM2_REQUEST_SIZE> modemanager_actuatorTm2_Global_Queue;
    modemanager_actuatorTm2_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_reading(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_READING_REQUEST_SIZE> modemanager_reading_Global_Queue;
    modemanager_reading_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_systemtc(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_SYSTEMTC_REQUEST_SIZE> modemanager_systemTc_Global_Queue;
    modemanager_systemTc_Global_Queue.put(sender_pid, data, length);
}




void initialize_transport()
{
}
