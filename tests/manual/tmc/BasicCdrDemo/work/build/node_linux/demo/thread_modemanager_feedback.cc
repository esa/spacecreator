#include "thread_modemanager_feedback.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void modemanager_feedback_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<MODEMANAGER_FEEDBACK_REQUEST_SIZE> modemanager_feedback_Global_Queue;

    while(true)
    {
        static taste::Request<MODEMANAGER_FEEDBACK_REQUEST_SIZE> request;
        modemanager_feedback_Global_Queue.get(request);

        modemanager_feedback_sender_pid = request.sender_pid();
        call_modemanager_feedback((const char*)request.data(), request.length());
    }
}

