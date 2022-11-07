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
define(`m4_actuator_feedback',`modemanager_feedback')dnl
define(`m4_actuator_feedback_provider',`modemanager')dnl
define(`m4_egse_tc',`modemanager_tc')dnl
define(`m4_egse_tc_provider',`modemanager')dnl
define(`m4_modemanager_ctrl',`actuator_ctrl')dnl
define(`m4_modemanager_ctrl_provider',`actuator')dnl
define(`m4_modemanager_tm',`egse_tm')dnl
define(`m4_modemanager_tm_provider',`egse')dnl
define(`m4_sensor_reading',`modemanager_reading')dnl
define(`m4_sensor_reading_provider',`modemanager')dnl
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
    ctrl,
    T_Integer,
     0,
     1)




include(actuator.if)

m4_sporadic_itf_gui_handler(
    egse,
    tm,
    `T_Report',
)


m4_sporadic_itf_gui_provider(
    egse,
    tc,
    `T_Config',
    `Egse_tc_cfg',
)
divert(5)
    T_Config cast_Egse_tc_cfg_to_T_Config(Egse_tc_cfg);
divert(10)


m4_gui_handler(egse)

m4_sporadic_itf_handler(
    modemanager,
    feedback,
    T_Integer,
     0,
     1)

m4_sporadic_itf_handler(
    modemanager,
    reading,
    T_Integer,
     0,
     1)

m4_sporadic_itf_handler(
    modemanager,
    tc,
    T_Config,
     0,
     1)





include(modemanager.if)


m4_sporadic_itf_gui_provider(
    sensor,
    reading,
    `T_Integer',
    `Sensor_reading_x',
)
divert(5)
    T_Integer cast_Sensor_reading_x_to_T_Integer(Sensor_reading_x);
divert(10)


m4_gui_handler(sensor)



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
