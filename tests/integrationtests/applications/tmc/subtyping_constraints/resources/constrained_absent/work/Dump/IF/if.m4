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
define(`m4_function_1_pi_1',`function_2_pi_1')dnl
define(`m4_function_1_PI_1_provider',`function_2')dnl
define(`m4_function_2_pi_2',`function_1_pi_2')dnl
define(`m4_function_2_PI_2_provider',`function_1')dnl
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
m4_sporadic_itf_gui_handler(
    function_1,
    pi_2,
    `MyChoice',
)


m4_sporadic_itf_gui_provider(
    function_1,
    pi_1,
    `MySeqOf',
    `Function_1_pi_1_p1',
)
divert(5)
    MySeqOf cast_Function_1_pi_1_p1_to_MySeqOf(Function_1_pi_1_p1);
divert(10)


m4_gui_handler(function_1)

m4_sporadic_itf_handler(
    function_2,
    pi_1,
    MySeqOf,
     0,
     1)




include(function_2.if)



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
