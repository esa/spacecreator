#include "thread_sensor_tc.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void sensor_tc_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<SENSOR_TC_REQUEST_SIZE> sensor_tc_Global_Queue;

    while(true)
    {
        static taste::Request<SENSOR_TC_REQUEST_SIZE> request;
        sensor_tc_Global_Queue.get(request);

        sensor_tc_sender_pid = request.sender_pid();
        call_sensor_tc((const char*)request.data(), request.length());
    }
}

