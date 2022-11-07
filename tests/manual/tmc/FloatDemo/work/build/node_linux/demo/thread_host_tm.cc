#include "thread_host_tm.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void host_tm_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<HOST_TM_REQUEST_SIZE> host_tm_Global_Queue;

    while(true)
    {
        static taste::Request<HOST_TM_REQUEST_SIZE> request;
        host_tm_Global_Queue.get(request);

        host_tm_sender_pid = request.sender_pid();
        call_host_tm((const char*)request.data(), request.length());
    }
}

