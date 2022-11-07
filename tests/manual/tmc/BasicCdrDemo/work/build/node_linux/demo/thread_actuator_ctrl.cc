#include "thread_actuator_ctrl.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void actuator_ctrl_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<ACTUATOR_CTRL_REQUEST_SIZE> actuator_ctrl_Global_Queue;

    while(true)
    {
        static taste::Request<ACTUATOR_CTRL_REQUEST_SIZE> request;
        actuator_ctrl_Global_Queue.get(request);

        actuator_ctrl_sender_pid = request.sender_pid();
        call_actuator_ctrl((const char*)request.data(), request.length());
    }
}

