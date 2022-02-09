int reset;
int set;
init
{
    reset = 1;
    do
    ::  reset;
        reset = 0;
        set = 1;
    ::  set;
        reset = 1;
        set = 0;
    od;
}
