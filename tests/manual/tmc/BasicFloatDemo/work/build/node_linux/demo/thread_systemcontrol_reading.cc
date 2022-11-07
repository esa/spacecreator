#include "thread_systemcontrol_reading.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void systemcontrol_reading_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<SYSTEMCONTROL_READING_REQUEST_SIZE> systemcontrol_reading_Global_Queue;

    while(true)
    {
        static taste::Request<SYSTEMCONTROL_READING_REQUEST_SIZE> request;
        systemcontrol_reading_Global_Queue.get(request);

        systemcontrol_reading_sender_pid = request.sender_pid();
        call_systemcontrol_reading((const char*)request.data(), request.length());
    }
}

