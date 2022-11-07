#include "thread_demo_timer_manager_tick.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void demo_timer_manager_tick_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    taste::Timer::run( 0ms, 10ms, [] () -> void
    {
        static taste::Request<DEMO_TIMER_MANAGER_TICK_REQUEST_SIZE> request;
        call_demo_timer_manager_tick((const char*)request.data(), request.length());
    });
}

