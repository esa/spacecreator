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
define(`m4_pinger_integerinterface',`receiver_integerinterface')dnl
define(`m4_pinger_IntegerInterface_provider',`receiver')dnl
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
    pinger,
    integerinterface,
    `MyBetterInteger',
    `Pinger_integerinterface_integerArg',
)
divert(5)
    MyBetterInteger cast_Pinger_integerinterface_integerArg_to_MyBetterInteger(Pinger_integerinterface_integerArg);
divert(10)


m4_gui_handler(pinger)

m4_sporadic_itf_handler(
    receiver,
    integerinterface,
    MyBetterInteger,
     0,
     1)



include(receiver.if)



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
