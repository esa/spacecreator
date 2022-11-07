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
define(`m4_controlsystem_controlsignal',`actuator_controlsignal')dnl
define(`m4_controlsystem_controlSignal_provider',`actuator')dnl
define(`m4_controlsystem_tm',`host_tm')dnl
define(`m4_controlsystem_tm_provider',`host')dnl
define(`m4_host_configuration',`controlsystem_configuration')dnl
define(`m4_host_configuration_provider',`controlsystem')dnl
define(`m4_host_tc',`sensor_tc')dnl
define(`m4_host_tc_provider',`sensor')dnl
define(`m4_sensor_reading',`controlsystem_reading')dnl
define(`m4_sensor_reading_provider',`controlsystem')dnl
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
    Number,
     0,
     1)



include(actuator.if)

m4_sporadic_itf_handler(
    controlsystem,
    configuration,
    PidConfig,
     0,
     1)

m4_sporadic_itf_handler(
    controlsystem,
    reading,
    Number,
     0,
     1)





include(controlsystem.if)

m4_sporadic_itf_handler(
    host,
    tm,
    ,
     0,
     1)





include(host.if)

m4_timer_handler(sensor,trigger)
m4_sporadic_itf_handler(
    sensor,
    tc,
    ,
     0,
     1)




include(sensor.if)



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
