#include "thread_sensor_poll.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void sensor_poll_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    taste::Timer::run( 0ms, 10ms, [] () -> void
    {
        static taste::Request<SENSOR_POLL_REQUEST_SIZE> request;
        call_sensor_poll((const char*)request.data(), request.length());
    });
}

