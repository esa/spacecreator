proctype testProctype()
{
    {
        1;
    }
    atomic {
        1;
    }
    d_step {
        1;
    }
    do
    ::  1;
        2;
    ::  atomic {
        1;
        2;
    }
    ::  d_step {
        1;
        2;
    }
    od;
    if
    ::  1;
        2;
    ::  atomic {
        1;
        2;
    }
    ::  d_step {
        1;
        2;
    }
    fi;
}
