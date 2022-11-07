#include "thread_host_trigger.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void host_trigger_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<HOST_TRIGGER_REQUEST_SIZE> host_trigger_Global_Queue;

    while(true)
    {
        static taste::Request<HOST_TRIGGER_REQUEST_SIZE> request;
        host_trigger_Global_Queue.get(request);

        // minimal inter-arrival time
        auto wakeupTime = std::chrono::steady_clock::now() + 1ms;
        host_trigger_sender_pid = request.sender_pid();
        call_host_trigger((const char*)request.data(), request.length());
        std::this_thread::sleep_until(wakeupTime);
    }
}

