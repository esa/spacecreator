#include "thread_modemanager_actuatortm2.h"

#include <Timer.h>
#include <Queue.h>
#include <Request.h>
#include <StartBarrier.h>

#include "transport.h"
#include "demo_interface.h"
#include "routing.h"

void modemanager_actuatortm2_job()
{
    // synchronize with other threads
    taste::StartBarrier::wait();

    using namespace std::chrono_literals;
    extern taste::Queue<MODEMANAGER_ACTUATORTM2_REQUEST_SIZE> modemanager_actuatorTm2_Global_Queue;

    while(true)
    {
        static taste::Request<MODEMANAGER_ACTUATORTM2_REQUEST_SIZE> request;
        modemanager_actuatorTm2_Global_Queue.get(request);

        modemanager_actuatortm2_sender_pid = request.sender_pid();
        call_modemanager_actuatortm2((const char*)request.data(), request.length());
    }
}

