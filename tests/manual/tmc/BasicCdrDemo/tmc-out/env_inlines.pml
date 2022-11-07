inline T_SmallInteger_generate_value(T_SmallInteger_gv)
{
    int T_SmallInteger_tmp;
    if
    ::  atomic {
        select (T_SmallInteger_tmp : 0 .. 0);
    }
    ::  atomic {
        select (T_SmallInteger_tmp : 4 .. 4);
    }
    ::  atomic {
        select (T_SmallInteger_tmp : 10 .. 10);
    }
    fi;
    T_SmallInteger_gv = T_SmallInteger_tmp;
}
inline Egse_tc_cfg_generate_value(Egse_tc_cfg_gv)
{
    T_SmallInteger_generate_value(Egse_tc_cfg_gv.param_a);
    T_SmallInteger_generate_value(Egse_tc_cfg_gv.param_b);
}
inline Sensor_reading_x_generate_value(Sensor_reading_x_gv)
{
    int Sensor_reading_x_tmp;
    if
    ::  atomic {
        select (Sensor_reading_x_tmp : 0 .. 0);
    }
    ::  atomic {
        select (Sensor_reading_x_tmp : 4 .. 4);
    }
    ::  atomic {
        select (Sensor_reading_x_tmp : 10 .. 10);
    }
    fi;
    Sensor_reading_x_gv = Sensor_reading_x_tmp;
}
inline T_Integer_generate_value(T_Integer_gv)
{
    int T_Integer_tmp;
    if
    ::  atomic {
        select (T_Integer_tmp : 0 .. 1000000);
    }
    fi;
    T_Integer_gv = T_Integer_tmp;
}
inline T_Config_generate_value(T_Config_gv)
{
    T_Integer_generate_value(T_Config_gv.param_a);
    T_Integer_generate_value(T_Config_gv.param_b);
}
