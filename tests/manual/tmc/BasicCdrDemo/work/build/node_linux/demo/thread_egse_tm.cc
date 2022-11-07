#include "thread_egse_tm.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void egse_tm_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<EGSE_TM_REQUEST_SIZE> egse_tm_Global_Queue;

    while(true)
    {
        static taste::Request<EGSE_TM_REQUEST_SIZE> request;
        egse_tm_Global_Queue.get(request);

        egse_tm_sender_pid = request.sender_pid();
        call_egse_tm((const char*)request.data(), request.length());
    }
}

