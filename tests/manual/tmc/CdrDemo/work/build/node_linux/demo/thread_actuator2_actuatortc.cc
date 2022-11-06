#include "thread_actuator2_actuatortc.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void actuator2_actuatortc_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<ACTUATOR2_ACTUATORTC_REQUEST_SIZE> actuator2_actuatorTc_Global_Queue;

    while(true)
    {
        static taste::Request<ACTUATOR2_ACTUATORTC_REQUEST_SIZE> request;
        actuator2_actuatorTc_Global_Queue.get(request);

        actuator2_actuatortc_sender_pid = request.sender_pid();
        call_actuator2_actuatortc((const char*)request.data(), request.length());
    }
}

