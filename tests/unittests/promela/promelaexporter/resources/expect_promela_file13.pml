int inited;
chan channel1 = [2] of {int};
init
{
    inited = 1;
    inited;
    2;
    int param = 4;
    channel1!param;
    channel1?param;
    fn();
}
