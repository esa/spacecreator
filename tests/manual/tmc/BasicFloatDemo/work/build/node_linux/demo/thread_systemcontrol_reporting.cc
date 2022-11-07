#include "thread_systemcontrol_reporting.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void systemcontrol_reporting_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<SYSTEMCONTROL_REPORTING_REQUEST_SIZE> systemcontrol_reporting_Global_Queue;

    while(true)
    {
        static taste::Request<SYSTEMCONTROL_REPORTING_REQUEST_SIZE> request;
        systemcontrol_reporting_Global_Queue.get(request);

        systemcontrol_reporting_sender_pid = request.sender_pid();
        call_systemcontrol_reporting((const char*)request.data(), request.length());
    }
}

