#include "thread_actuator1_actuatortc.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void actuator1_actuatortc_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<ACTUATOR1_ACTUATORTC_REQUEST_SIZE> actuator1_actuatorTc_Global_Queue;

    while(true)
    {
        static taste::Request<ACTUATOR1_ACTUATORTC_REQUEST_SIZE> request;
        actuator1_actuatorTc_Global_Queue.get(request);

        actuator1_actuatortc_sender_pid = request.sender_pid();
        call_actuator1_actuatortc((const char*)request.data(), request.length());
    }
}

