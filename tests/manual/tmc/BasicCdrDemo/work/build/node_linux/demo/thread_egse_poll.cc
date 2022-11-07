#include "thread_egse_poll.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void egse_poll_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    taste::Timer::run( 0ms, 10ms, [] () -> void
    {
        static taste::Request<EGSE_POLL_REQUEST_SIZE> request;
        call_egse_poll((const char*)request.data(), request.length());
    });
}

