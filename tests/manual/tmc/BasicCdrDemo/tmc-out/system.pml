#include "dataview.pml"
#include "actuator.pml"
#include "modemanager.pml"
#include "env_inlines.pml"
typedef system_state {
    Actuator_Context actuator;
    Modemanager_Context modemanager;
    AggregateTimerData timers;
}

int inited;
chan Actuator_ctrl_channel = [1] of {T_Integer};
T_Integer Actuator_ctrl_signal_parameter;
bool Actuator_ctrl_channel_used = 0;
chan Egse_tm_channel = [1] of {T_Report};
T_Report Egse_tm_signal_parameter;
bool Egse_tm_channel_used = 0;
chan Modemanager_feedback_channel = [1] of {T_Integer};
T_Integer Modemanager_feedback_signal_parameter;
bool Modemanager_feedback_channel_used = 0;
chan Modemanager_reading_channel = [1] of {T_Integer};
T_Integer Modemanager_reading_signal_parameter;
bool Modemanager_reading_channel_used = 0;
chan Modemanager_tc_channel = [1] of {T_Config};
T_Config Modemanager_tc_signal_parameter;
bool Modemanager_tc_channel_used = 0;
system_state global_state;
chan Actuator_lock = [1] of {int};
chan Modemanager_lock = [1] of {int};
inline Modemanager_0_RI_0_ctrl(Actuator_ctrl_x)
{
    Actuator_ctrl_channel!Actuator_ctrl_x;
}
inline Actuator_check_queue()
{
    atomic {
        empty(Actuator_ctrl_channel);
    }
}
inline Actuator_0_get_sender(Actuator_sender_arg)
{
    skip;
}
inline Modemanager_0_RI_0_tm(EGSE_tm_reprt)
{
    Egse_tm_channel!EGSE_tm_reprt;
}
inline Actuator_0_RI_0_feedback(ModeManager_feedback_x)
{
    Modemanager_feedback_channel!ModeManager_feedback_x;
}
inline Sensor_0_RI_0_reading(ModeManager_reading_x)
{
    Modemanager_reading_channel!ModeManager_reading_x;
}
inline Egse_0_RI_0_tc(ModeManager_tc_cfg)
{
    Modemanager_tc_channel!ModeManager_tc_cfg;
}
inline Modemanager_check_queue()
{
    atomic {
        (empty(Modemanager_feedback_channel) && (empty(Modemanager_reading_channel) && empty(Modemanager_tc_channel)));
    }
}
inline Modemanager_0_get_sender(Modemanager_sender_arg)
{
    skip;
}
active proctype Actuator_ctrl() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Actuator_ctrl_channel);
        Actuator_lock?_;
Actuator_ctrl_loop:
        if
        ::  nempty(Actuator_ctrl_channel);
            Actuator_ctrl_channel?Actuator_ctrl_signal_parameter;
            Actuator_ctrl_channel_used = 1;
            Actuator_0_PI_0_ctrl(Actuator_ctrl_signal_parameter);
            goto Actuator_ctrl_loop;
        ::  empty(Actuator_ctrl_channel);
            skip;
        fi;
        Actuator_lock!1;
    }
    od;
}
active proctype Egse_tm() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Egse_tm_channel);
Egse_tm_loop:
        if
        ::  nempty(Egse_tm_channel);
            Egse_tm_channel?Egse_tm_signal_parameter;
            Egse_tm_channel_used = 1;
            goto Egse_tm_loop;
        ::  empty(Egse_tm_channel);
            skip;
        fi;
    }
    od;
}
active proctype Egse_tc() priority 1
{
    inited;
    T_Config value;
    int inputVectorCounter;
    for(inputVectorCounter : 0 .. 3)
    {
        atomic {
            Egse_tc_cfg_generate_value(value);
            Egse_0_RI_0_tc(value);
        }
    }
}
active proctype Modemanager_feedback() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Modemanager_feedback_channel);
        Modemanager_lock?_;
Modemanager_feedback_loop:
        if
        ::  nempty(Modemanager_feedback_channel);
            Modemanager_feedback_channel?Modemanager_feedback_signal_parameter;
            Modemanager_feedback_channel_used = 1;
            Modemanager_0_PI_0_feedback(Modemanager_feedback_signal_parameter);
            goto Modemanager_feedback_loop;
        ::  empty(Modemanager_feedback_channel);
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
active proctype Modemanager_tc() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Modemanager_tc_channel);
        Modemanager_lock?_;
Modemanager_tc_loop:
        if
        ::  nempty(Modemanager_tc_channel);
            Modemanager_tc_channel?Modemanager_tc_signal_parameter;
            Modemanager_tc_channel_used = 1;
            Modemanager_0_PI_0_tc(Modemanager_tc_signal_parameter);
            goto Modemanager_tc_loop;
        ::  empty(Modemanager_tc_channel);
            skip;
        fi;
        Modemanager_lock!1;
    }
    od;
}
active proctype Sensor_reading() priority 1
{
    inited;
    T_Integer value;
    int inputVectorCounter;
    for(inputVectorCounter : 0 .. 3)
    {
        atomic {
            Sensor_reading_x_generate_value(value);
            Sensor_0_RI_0_reading(value);
        }
    }
}
init
{
    atomic {
        global_dataview_init();
        Actuator_0_init();
        Actuator_lock!1;
        Modemanager_0_init();
        Modemanager_lock!1;
        inited = 1;
    }
}
#include "scl.pml"
