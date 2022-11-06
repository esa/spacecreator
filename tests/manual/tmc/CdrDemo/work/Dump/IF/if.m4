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
define(`m4_actuator1_actuatortm',`modemanager_actuatortm1')dnl
define(`m4_actuator1_actuatorTm_provider',`modemanager')dnl
define(`m4_actuator2_actuatortm',`modemanager_actuatortm2')dnl
define(`m4_actuator2_actuatorTm_provider',`modemanager')dnl
define(`m4_egse_systemtc',`modemanager_systemtc')dnl
define(`m4_egse_systemTc_provider',`modemanager')dnl
define(`m4_modemanager_actuatortc1',`actuator1_actuatortc')dnl
define(`m4_modemanager_actuatorTc1_provider',`actuator1')dnl
define(`m4_modemanager_actuatortc2',`actuator2_actuatortc')dnl
define(`m4_modemanager_actuatorTc2_provider',`actuator2')dnl
define(`m4_modemanager_systemtm',`egse_systemtm')dnl
define(`m4_modemanager_systemTm_provider',`egse')dnl
define(`m4_sensor_reading',`modemanager_reading')dnl
define(`m4_sensor_reading_provider',`modemanager')dnl
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
    actuator1,
    actuatortc,
    ActuatorCommand,
     0,
     1)




include(actuator1.if)

m4_sporadic_itf_handler(
    actuator2,
    actuatortc,
    ActuatorCommand,
     0,
     1)




include(actuator2.if)

m4_sporadic_itf_gui_handler(
    egse,
    systemtm,
    `SystemTelemetry',
)


m4_sporadic_itf_gui_provider(
    egse,
    systemtc,
    `SystemCommand',
    `Egse_systemtc_tc',
)
divert(5)
    SystemCommand cast_Egse_systemtc_tc_to_SystemCommand(Egse_systemtc_tc);
divert(10)


m4_gui_handler(egse)

m4_sporadic_itf_handler(
    modemanager,
    actuatortm1,
    ActuatorStatus,
     0,
     1)

m4_sporadic_itf_handler(
    modemanager,
    actuatortm2,
    ActuatorStatus,
     0,
     1)

m4_sporadic_itf_handler(
    modemanager,
    reading,
    DataItem,
     0,
     1)

m4_sporadic_itf_handler(
    modemanager,
    systemtc,
    SystemCommand,
     0,
     1)






include(modemanager.if)

m4_c_function(sensor,(trigger,(),(),(), 0),

)



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
