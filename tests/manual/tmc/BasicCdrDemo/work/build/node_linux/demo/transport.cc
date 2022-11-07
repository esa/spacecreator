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


void deliver_to_actuator_ctrl(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<ACTUATOR_CTRL_REQUEST_SIZE> actuator_ctrl_Global_Queue;
    actuator_ctrl_Global_Queue.put(sender_pid, data, length);
}




void deliver_to_egse_tm(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<EGSE_TM_REQUEST_SIZE> egse_tm_Global_Queue;
    egse_tm_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_feedback(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_FEEDBACK_REQUEST_SIZE> modemanager_feedback_Global_Queue;
    modemanager_feedback_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_reading(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_READING_REQUEST_SIZE> modemanager_reading_Global_Queue;
    modemanager_reading_Global_Queue.put(sender_pid, data, length);
}



void deliver_to_modemanager_tc(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length)
{
    extern taste::Queue<MODEMANAGER_TC_REQUEST_SIZE> modemanager_tc_Global_Queue;
    modemanager_tc_Global_Queue.put(sender_pid, data, length);
}




void initialize_transport()
{
}
