#include "dataview.pml"
#include "actuator1.pml"
#include "actuator2.pml"
#include "modemanager.pml"
#include "env_inlines.pml"
typedef system_state {
    Modemanager_Context modemanager;
    Actuator_Context actuator1;
    Actuator_Context actuator2;
    AggregateTimerData timers;
}

int inited;
chan Actuator1_actuatorTc_channel = [1] of {ActuatorCommand};
ActuatorCommand Actuator1_actuatortc_signal_parameter;
bool Actuator1_actuatortc_channel_used = 0;
chan Actuator2_actuatorTc_channel = [1] of {ActuatorCommand};
ActuatorCommand Actuator2_actuatortc_signal_parameter;
bool Actuator2_actuatortc_channel_used = 0;
chan Egse_systemTm_channel = [1] of {SystemTelemetry};
SystemTelemetry Egse_systemtm_signal_parameter;
bool Egse_systemtm_channel_used = 0;
chan Modemanager_actuatorTm1_channel = [1] of {ActuatorStatus};
ActuatorStatus Modemanager_actuatortm1_signal_parameter;
bool Modemanager_actuatortm1_channel_used = 0;
chan Modemanager_actuatorTm2_channel = [1] of {ActuatorStatus};
ActuatorStatus Modemanager_actuatortm2_signal_parameter;
bool Modemanager_actuatortm2_channel_used = 0;
chan Modemanager_reading_channel = [1] of {DataItem};
DataItem Modemanager_reading_signal_parameter;
bool Modemanager_reading_channel_used = 0;
chan Modemanager_systemTc_channel = [1] of {SystemCommand};
SystemCommand Modemanager_systemtc_signal_parameter;
bool Modemanager_systemtc_channel_used = 0;
system_state global_state;
chan Actuator1_lock = [1] of {int};
chan Actuator2_lock = [1] of {int};
chan Modemanager_lock = [1] of {int};
inline Modemanager_0_RI_0_actuatorTc1(Actuator1_actuatorTc_tc)
{
    Actuator1_actuatorTc_channel!Actuator1_actuatorTc_tc;
}
inline Actuator1_check_queue()
{
    atomic {
        empty(Actuator1_actuatorTc_channel);
    }
}
inline Actuator1_0_get_sender(Actuator1_sender_arg)
{
    skip;
}
inline Modemanager_0_RI_0_actuatorTc2(Actuator2_actuatorTc_tc)
{
    Actuator2_actuatorTc_channel!Actuator2_actuatorTc_tc;
}
inline Actuator2_check_queue()
{
    atomic {
        empty(Actuator2_actuatorTc_channel);
    }
}
inline Actuator2_0_get_sender(Actuator2_sender_arg)
{
    skip;
}
inline Modemanager_0_RI_0_systemTm(EGSE_systemTm_tm)
{
    Egse_systemTm_channel!EGSE_systemTm_tm;
}
inline Actuator1_0_RI_0_actuatorTm(ModeManager_actuatorTm1_tm)
{
    Modemanager_actuatorTm1_channel!ModeManager_actuatorTm1_tm;
}
inline Actuator2_0_RI_0_actuatorTm(ModeManager_actuatorTm2_tm)
{
    Modemanager_actuatorTm2_channel!ModeManager_actuatorTm2_tm;
}
inline Sensor_0_RI_0_reading(ModeManager_reading_x)
{
    Modemanager_reading_channel!ModeManager_reading_x;
}
inline Egse_0_RI_0_systemTc(ModeManager_systemTc_tc)
{
    Modemanager_systemTc_channel!ModeManager_systemTc_tc;
}
inline Modemanager_check_queue()
{
    atomic {
        (empty(Modemanager_actuatorTm1_channel) && (empty(Modemanager_actuatorTm2_channel) && (empty(Modemanager_reading_channel) && empty(Modemanager_systemTc_channel))));
    }
}
inline Modemanager_0_get_sender(Modemanager_sender_arg)
{
    skip;
}
active proctype Actuator1_actuatorTc() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Actuator1_actuatorTc_channel);
        Actuator1_lock?_;
Actuator1_actuatortc_loop:
        if
        ::  nempty(Actuator1_actuatorTc_channel);
            Actuator1_actuatorTc_channel?Actuator1_actuatortc_signal_parameter;
            Actuator1_actuatortc_channel_used = 1;
            Actuator1_0_PI_0_actuatorTc(Actuator1_actuatortc_signal_parameter);
            goto Actuator1_actuatortc_loop;
        ::  empty(Actuator1_actuatorTc_channel);
            skip;
        fi;
        Actuator1_lock!1;
    }
    od;
}
active proctype Actuator2_actuatorTc() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Actuator2_actuatorTc_channel);
        Actuator2_lock?_;
Actuator2_actuatortc_loop:
        if
        ::  nempty(Actuator2_actuatorTc_channel);
            Actuator2_actuatorTc_channel?Actuator2_actuatortc_signal_parameter;
            Actuator2_actuatortc_channel_used = 1;
            Actuator2_0_PI_0_actuatorTc(Actuator2_actuatortc_signal_parameter);
            goto Actuator2_actuatortc_loop;
        ::  empty(Actuator2_actuatorTc_channel);
            skip;
        fi;
        Actuator2_lock!1;
    }
    od;
}
active proctype Egse_systemTm() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Egse_systemTm_channel);
Egse_systemtm_loop:
        if
        ::  nempty(Egse_systemTm_channel);
            Egse_systemTm_channel?Egse_systemtm_signal_parameter;
            Egse_systemtm_channel_used = 1;
            goto Egse_systemtm_loop;
        ::  empty(Egse_systemTm_channel);
            skip;
        fi;
    }
    od;
}
active proctype Egse_systemTc() priority 1
{
    inited;
    SystemCommand value;
    int inputVectorCounter;
    for(inputVectorCounter : 0 .. 3)
    {
        atomic {
            SystemCommand_generate_value(value);
            Egse_0_RI_0_systemTc(value);
        }
    }
}
active proctype Modemanager_actuatorTm1() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Modemanager_actuatorTm1_channel);
        Modemanager_lock?_;
Modemanager_actuatortm1_loop:
        if
        ::  nempty(Modemanager_actuatorTm1_channel);
            Modemanager_actuatorTm1_channel?Modemanager_actuatortm1_signal_parameter;
            Modemanager_actuatortm1_channel_used = 1;
            Modemanager_0_PI_0_actuatorTm1(Modemanager_actuatortm1_signal_parameter);
            goto Modemanager_actuatortm1_loop;
        ::  empty(Modemanager_actuatorTm1_channel);
            skip;
        fi;
        Modemanager_lock!1;
    }
    od;
}
active proctype Modemanager_actuatorTm2() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Modemanager_actuatorTm2_channel);
        Modemanager_lock?_;
Modemanager_actuatortm2_loop:
        if
        ::  nempty(Modemanager_actuatorTm2_channel);
            Modemanager_actuatorTm2_channel?Modemanager_actuatortm2_signal_parameter;
            Modemanager_actuatortm2_channel_used = 1;
            Modemanager_0_PI_0_actuatorTm2(Modemanager_actuatortm2_signal_parameter);
            goto Modemanager_actuatortm2_loop;
        ::  empty(Modemanager_actuatorTm2_channel);
            skip;
        fi;
        Modemanager_lock!1;
    }
    od;
}
active proctype Modemanager_reading() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Modemanager_reading_channel);
        Modemanager_lock?_;
Modemanager_reading_loop:
        if
        ::  nempty(Modemanager_reading_channel);
            Modemanager_reading_channel?Modemanager_reading_signal_parameter;
            Modemanager_reading_channel_used = 1;
            Modemanager_0_PI_0_reading(Modemanager_reading_signal_parameter);
            goto Modemanager_reading_loop;
        ::  empty(Modemanager_reading_channel);
            skip;
        fi;
        Modemanager_lock!1;
    }
    od;
}
active proctype Modemanager_systemTc() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Modemanager_systemTc_channel);
        Modemanager_lock?_;
Modemanager_systemtc_loop:
        if
        ::  nempty(Modemanager_systemTc_channel);
            Modemanager_systemTc_channel?Modemanager_systemtc_signal_parameter;
            Modemanager_systemtc_channel_used = 1;
            Modemanager_0_PI_0_systemTc(Modemanager_systemtc_signal_parameter);
            goto Modemanager_systemtc_loop;
        ::  empty(Modemanager_systemTc_channel);
            skip;
        fi;
        Modemanager_lock!1;
    }
    od;
}
active proctype Sensor_reading() priority 1
{
    inited;
    DataItem value;
    int inputVectorCounter;
    for(inputVectorCounter : 0 .. 3)
    {
        atomic {
            DataItem_generate_value(value);
            Sensor_0_RI_0_reading(value);
        }
    }
}
init
{
    atomic {
        global_dataview_init();
        Actuator1_0_init();
        Actuator1_lock!1;
        Actuator2_0_init();
        Actuator2_lock!1;
        Modemanager_0_init();
        Modemanager_lock!1;
        inited = 1;
    }
}
