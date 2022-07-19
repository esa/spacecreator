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
define(`m4_clock_tick',`pinger_tick')dnl
define(`m4_clock_Tick_provider',`pinger')dnl
define(`m4_pinger_ping',`ponger_ping')dnl
define(`m4_pinger_Ping_provider',`ponger')dnl
define(`m4_ponger_pong',`pinger_pong')dnl
define(`m4_ponger_Pong_provider',`pinger')dnl
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

m4_sporadic_itf_gui_provider(
    clock,
    tick,
    ,
)


m4_gui_handler(clock)

// ERROR: Interface "Pong" in function "Pinger" has unsupported kind: "UNPROTECTED_OPERATION"

m4_sporadic_itf_handler(
    pinger,
    tick,
    ,
     0,
     1)




include(pinger.if)

// ERROR: Interface "Ping" in function "Ponger" has unsupported kind: "UNPROTECTED_OPERATION"




include(ponger.if)



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
