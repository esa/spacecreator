inline T_Int_generate_value(T_Int_gv)
{
    int T_Int_tmp;
    if
    ::  atomic {
        select (T_Int_tmp : -10000 .. 10000);
    }
    fi;
    T_Int_gv = T_Int_tmp;
}
