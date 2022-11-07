#include "thread_systemcontrol_tc.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void systemcontrol_tc_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<SYSTEMCONTROL_TC_REQUEST_SIZE> systemcontrol_tc_Global_Queue;

    while(true)
    {
        static taste::Request<SYSTEMCONTROL_TC_REQUEST_SIZE> request;
        systemcontrol_tc_Global_Queue.get(request);

        systemcontrol_tc_sender_pid = request.sender_pid();
        call_systemcontrol_tc((const char*)request.data(), request.length());
    }
}

