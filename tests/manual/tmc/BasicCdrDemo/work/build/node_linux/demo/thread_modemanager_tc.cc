#include "thread_modemanager_tc.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void modemanager_tc_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<MODEMANAGER_TC_REQUEST_SIZE> modemanager_tc_Global_Queue;

    while(true)
    {
        static taste::Request<MODEMANAGER_TC_REQUEST_SIZE> request;
        modemanager_tc_Global_Queue.get(request);

        modemanager_tc_sender_pid = request.sender_pid();
        call_modemanager_tc((const char*)request.data(), request.length());
    }
}

