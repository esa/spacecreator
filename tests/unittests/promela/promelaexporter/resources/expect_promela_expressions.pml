chan channel1 = [2] of {int};
init
{
    int a;
    int b;
    bool m;
    bool n;
    a = 20;
    b = 5;
    m = 0;
    n = 1;
    a = a + b;
    a = a - b;
    a = a * b;
    a = a / b;
    m = a == b;
    m = a < b;
    m = a > b;
    m = a <= b;
    m = a >= b;
    a = a % b;
    m = a != b;
    m = m && n;
    m = m || n;
    m = empty(channel1);
}
