#include "thread_modemanager_systemtc.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void modemanager_systemtc_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<MODEMANAGER_SYSTEMTC_REQUEST_SIZE> modemanager_systemTc_Global_Queue;

    while(true)
    {
        static taste::Request<MODEMANAGER_SYSTEMTC_REQUEST_SIZE> request;
        modemanager_systemTc_Global_Queue.get(request);

        modemanager_systemtc_sender_pid = request.sender_pid();
        call_modemanager_systemtc((const char*)request.data(), request.length());
    }
}

