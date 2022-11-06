#include "thread_egse_systemtm.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void egse_systemtm_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<EGSE_SYSTEMTM_REQUEST_SIZE> egse_systemTm_Global_Queue;

    while(true)
    {
        static taste::Request<EGSE_SYSTEMTM_REQUEST_SIZE> request;
        egse_systemTm_Global_Queue.get(request);

        egse_systemtm_sender_pid = request.sender_pid();
        call_egse_systemtm((const char*)request.data(), request.length());
    }
}

