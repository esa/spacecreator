#include "thread_actuator_controlsignal.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void actuator_controlsignal_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<ACTUATOR_CONTROLSIGNAL_REQUEST_SIZE> actuator_controlsignal_Global_Queue;

    while(true)
    {
        static taste::Request<ACTUATOR_CONTROLSIGNAL_REQUEST_SIZE> request;
        actuator_controlsignal_Global_Queue.get(request);

        actuator_controlsignal_sender_pid = request.sender_pid();
        call_actuator_controlsignal((const char*)request.data(), request.length());
    }
}

