#include "dataview.pml"
#include "actuator.pml"
#include "controlsystem.pml"
#include "host.pml"
#include "sensor.pml"
#include "env_inlines.pml"
typedef system_state {
    Sensor_Context sensor;
    Host_Context host;
    Controlsystem_Context controlsystem;
    Actuator_Context actuator;
    AggregateTimerData timers;
}

int inited;
chan Actuator_controlSignal_channel = [1] of {Number};
Number Actuator_controlsignal_signal_parameter;
bool Actuator_controlsignal_channel_used = 0;
chan Controlsystem_configuration_channel = [1] of {PidConfig};
PidConfig Controlsystem_configuration_signal_parameter;
bool Controlsystem_configuration_channel_used = 0;
chan Controlsystem_reading_channel = [1] of {Number};
Number Controlsystem_reading_signal_parameter;
bool Controlsystem_reading_channel_used = 0;
chan Host_tm_channel = [1] of {int};
chan Sensor_tc_channel = [1] of {int};
chan Sensor_trigger_channel = [1] of {int};
system_state global_state;
chan Controlsystem_lock = [1] of {int};
chan Actuator_lock = [1] of {int};
chan Sensor_lock = [1] of {int};
chan Host_lock = [1] of {int};
inline Sensor_0_trigger_set(Sensor_trigger_interval)
{
    global_state.timers.sensor.trigger.interval = Sensor_trigger_interval;
    global_state.timers.sensor.trigger.timer_enabled = true;
    printf("set_timer Sensor trigger %d\n", Sensor_trigger_interval);
}
inline Sensor_0_trigger_reset()
{
    global_state.timers.sensor.trigger.timer_enabled = false;
    printf("reset_timer Sensor trigger\n");
}
inline Controlsystem_0_RI_0_controlSignal(Actuator_controlSignal_controlValue)
{
    Actuator_controlSignal_channel!Actuator_controlSignal_controlValue;
}
inline Actuator_check_queue()
{
    atomic {
        empty(Actuator_controlSignal_channel);
    }
}
inline Actuator_0_get_sender(Actuator_sender_arg)
{
    skip;
}
inline Host_0_RI_0_configuration(ControlSystem_configuration_cfg)
{
    Controlsystem_configuration_channel!ControlSystem_configuration_cfg;
}
inline Sensor_0_RI_0_reading(ControlSystem_reading_signalValue)
{
    Controlsystem_reading_channel!ControlSystem_reading_signalValue;
}
inline Controlsystem_check_queue()
{
    atomic {
        (empty(Controlsystem_configuration_channel) && empty(Controlsystem_reading_channel));
    }
}
inline Controlsystem_0_get_sender(Controlsystem_sender_arg)
{
    skip;
}
inline Controlsystem_0_RI_0_tm()
{
    int dummy;
    Host_tm_channel!dummy;
}
inline Host_check_queue()
{
    atomic {
        empty(Host_tm_channel);
    }
}
inline Host_0_get_sender(Host_sender_arg)
{
    skip;
}
inline Host_0_RI_0_tc()
{
    int dummy;
    Sensor_tc_channel!dummy;
}
inline Sensor_check_queue()
{
    atomic {
        empty(Sensor_tc_channel);
    }
}
inline Sensor_0_get_sender(Sensor_sender_arg)
{
    skip;
}
active proctype timer_manager_proc() priority 1
{
    inited;
    do
    ::  atomic {
        true;
        if
        ::  global_state.timers.sensor.trigger.timer_enabled;
            Sensor_trigger_channel!0;
            global_state.timers.sensor.trigger.timer_enabled = false;
        ::  else;
            skip;
        fi;
    }
    od;
}
active proctype Actuator_controlSignal() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Actuator_controlSignal_channel);
        Actuator_lock?_;
Actuator_controlsignal_loop:
        if
        ::  nempty(Actuator_controlSignal_channel);
            Actuator_controlSignal_channel?Actuator_controlsignal_signal_parameter;
            Actuator_controlsignal_channel_used = 1;
            Actuator_0_PI_0_controlSignal(Actuator_controlsignal_signal_parameter);
            goto Actuator_controlsignal_loop;
        ::  empty(Actuator_controlSignal_channel);
            skip;
        fi;
        Actuator_lock!1;
    }
    od;
}
active proctype Controlsystem_configuration() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Controlsystem_configuration_channel);
        Controlsystem_lock?_;
Controlsystem_configuration_loop:
        if
        ::  nempty(Controlsystem_configuration_channel);
            Controlsystem_configuration_channel?Controlsystem_configuration_signal_parameter;
            Controlsystem_configuration_channel_used = 1;
            Controlsystem_0_PI_0_configuration(Controlsystem_configuration_signal_parameter);
            goto Controlsystem_configuration_loop;
        ::  empty(Controlsystem_configuration_channel);
            skip;
        fi;
        Controlsystem_lock!1;
    }
    od;
}
active proctype Controlsystem_reading() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Controlsystem_reading_channel);
        Controlsystem_lock?_;
Controlsystem_reading_loop:
        if
        ::  nempty(Controlsystem_reading_channel);
            Controlsystem_reading_channel?Controlsystem_reading_signal_parameter;
            Controlsystem_reading_channel_used = 1;
            Controlsystem_0_PI_0_reading(Controlsystem_reading_signal_parameter);
            goto Controlsystem_reading_loop;
        ::  empty(Controlsystem_reading_channel);
            skip;
        fi;
        Controlsystem_lock!1;
    }
    od;
}
active proctype Host_tm() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Host_tm_channel);
        Host_lock?_;
Host_tm_loop:
        if
        ::  nempty(Host_tm_channel);
            Host_tm_channel?_;
            Host_0_PI_0_tm();
            goto Host_tm_loop;
        ::  empty(Host_tm_channel);
            skip;
        fi;
        Host_lock!1;
    }
    od;
}
active proctype Sensor_tc() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Sensor_tc_channel);
        Sensor_lock?_;
Sensor_tc_loop:
        if
        ::  nempty(Sensor_tc_channel);
            Sensor_tc_channel?_;
            Sensor_0_PI_0_tc();
            goto Sensor_tc_loop;
        ::  empty(Sensor_tc_channel);
            skip;
        fi;
        Sensor_lock!1;
    }
    od;
}
active proctype Sensor_trigger() priority 1
{
    inited;
    do
    ::  atomic {
        nempty(Sensor_trigger_channel);
        Sensor_lock?_;
Sensor_trigger_loop:
        if
        ::  nempty(Sensor_trigger_channel);
            Sensor_trigger_channel?_;
            Sensor_0_PI_0_trigger();
            goto Sensor_trigger_loop;
        ::  empty(Sensor_trigger_channel);
            skip;
        fi;
        Sensor_lock!1;
    }
    od;
}
init
{
    atomic {
        global_dataview_init();
        Controlsystem_0_init();
        Controlsystem_lock!1;
        Actuator_0_init();
        Actuator_lock!1;
        Sensor_0_init();
        Sensor_lock!1;
        Host_0_init();
        Host_lock!1;
        inited = 1;
    }
}
