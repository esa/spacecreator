#include "thread_controlsystem_reading.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void controlsystem_reading_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<CONTROLSYSTEM_READING_REQUEST_SIZE> controlsystem_reading_Global_Queue;

    while(true)
    {
        static taste::Request<CONTROLSYSTEM_READING_REQUEST_SIZE> request;
        controlsystem_reading_Global_Queue.get(request);

        controlsystem_reading_sender_pid = request.sender_pid();
        call_controlsystem_reading((const char*)request.data(), request.length());
    }
}

