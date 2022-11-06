#include "thread_modemanager_reading.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void modemanager_reading_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<MODEMANAGER_READING_REQUEST_SIZE> modemanager_reading_Global_Queue;

    while(true)
    {
        static taste::Request<MODEMANAGER_READING_REQUEST_SIZE> request;
        modemanager_reading_Global_Queue.get(request);

        modemanager_reading_sender_pid = request.sender_pid();
        call_modemanager_reading((const char*)request.data(), request.length());
    }
}

