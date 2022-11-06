#include "thread_modemanager_actuatortm1.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void modemanager_actuatortm1_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<MODEMANAGER_ACTUATORTM1_REQUEST_SIZE> modemanager_actuatorTm1_Global_Queue;

    while(true)
    {
        static taste::Request<MODEMANAGER_ACTUATORTM1_REQUEST_SIZE> request;
        modemanager_actuatorTm1_Global_Queue.get(request);

        modemanager_actuatortm1_sender_pid = request.sender_pid();
        call_modemanager_actuatortm1((const char*)request.data(), request.length());
    }
}

