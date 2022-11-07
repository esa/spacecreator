divert(-1)
/*
*  This m4 file uses the following diverts:
*    1 for overall structure
*    5 for cast functions for GUI parameter subtypes
*    7 for num functions on Enum types
*    10 for signals
*    20 for functions
*/dnl
include(templates.m4)
divert(-1)
define(`m4_actuator_reporting',`systemcontrol_reporting')dnl
define(`m4_actuator_reporting_provider',`systemcontrol')dnl
define(`m4_egse_tc',`systemcontrol_tc')dnl
define(`m4_egse_tc_provider',`systemcontrol')dnl
define(`m4_sensor_reading',`systemcontrol_reading')dnl
define(`m4_sensor_reading_provider',`systemcontrol')dnl
define(`m4_systemcontrol_controlsignal',`actuator_controlsignal')dnl
define(`m4_systemcontrol_controlsignal_provider',`actuator')dnl
define(`m4_systemcontrol_tm',`egse_tm')dnl
define(`m4_systemcontrol_tm_provider',`egse')dnl
define(`m4_env_trigger',`sensor_trigger')dnl
define(`m4_env_trigger_provider',`sensor')dnl
divert(1)dnl
system taste;
/*
 *
 * Data View
 *
 */
include(dataview.if)

type math = abstract
    integer abs(integer);
    real abs(real);
    integer fix(real);
    real power(real, real);
    integer Shift_Left(integer, integer);
    integer Shift_Right(integer, integer);
    integer ceil(real);
    integer floor(real);
    real float(integer);
    integer round(real);
    real sin(real);
    real cos(real);
    integer trunc(real);
endabstract;

type enum_functions = abstract
undivert(7)
endabstract;


divert(20)
m4_sporadic_itf_handler(
    actuator,
    controlsignal,
    T_Float,
     0,
     1)




include(actuator.if)

m4_sporadic_itf_gui_handler(
    egse,
    tm,
    `T_Int',
)


m4_sporadic_itf_gui_provider(
    egse,
    tc,
    `T_Int',
    `Egse_tc_x',
)
divert(5)
    T_Int cast_Egse_tc_x_to_T_Int(Egse_tc_x);
divert(10)


m4_gui_handler(egse)

m4_cyclic_itf_handler(
    sensor,
    trigger,
     1000,
     1)




include(sensor.if)

m4_sporadic_itf_handler(
    systemcontrol,
    reading,
    T_Float,
     0,
     1)

m4_sporadic_itf_handler(
    systemcontrol,
    reporting,
    ,
     0,
     1)

m4_sporadic_itf_handler(
    systemcontrol,
    tc,
    T_Int,
     0,
     1)





include(systemcontrol.if)



divert(1)
type assign = abstract
undivert(5)
endabstract;

/*
 *
 * Interface View
 *
 */
signal set_timer(integer);
signal reset_timer();

undivert(10)

undivert(20)

endsystem;

priorityrules
undivert(30)
endpriorityrules;
