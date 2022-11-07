#include "thread_controlsystem_configuration.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void controlsystem_configuration_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<CONTROLSYSTEM_CONFIGURATION_REQUEST_SIZE> controlsystem_configuration_Global_Queue;

    while(true)
    {
        static taste::Request<CONTROLSYSTEM_CONFIGURATION_REQUEST_SIZE> request;
        controlsystem_configuration_Global_Queue.get(request);

        controlsystem_configuration_sender_pid = request.sender_pid();
        call_controlsystem_configuration((const char*)request.data(), request.length());
    }
}

