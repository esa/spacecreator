#define Egse_systemtc_tc_goToIdle_PRESENT 1
#define Egse_systemtc_tc_goToOperational_PRESENT 2
#define Egse_systemtc_tc_doReport_PRESENT 3
#define SystemCommand_goToIdle_PRESENT 1
#define SystemCommand_goToOperational_PRESENT 2
#define SystemCommand_doReport_PRESENT 3
inline OptimizedDataItem_generate_value(OptimizedDataItem_gv)
{
    int OptimizedDataItem_tmp;
    if
    ::  atomic {
        select (OptimizedDataItem_tmp : 10 .. 10);
    }
    ::  atomic {
        select (OptimizedDataItem_tmp : 50 .. 50);
    }
    ::  atomic {
        select (OptimizedDataItem_tmp : 100 .. 100);
    }
    fi;
    OptimizedDataItem_gv = OptimizedDataItem_tmp;
}
inline OptimizedConfiguration_generate_value(OptimizedConfiguration_gv)
{
    OptimizedDataItem_generate_value(OptimizedConfiguration_gv.threshold1);
    OptimizedDataItem_generate_value(OptimizedConfiguration_gv.threshold2);
}
inline Dummy_generate_value(Dummy_gv)
{
    int Dummy_tmp;
    if
    ::  atomic {
        select (Dummy_tmp : 0 .. 0);
    }
    fi;
    Dummy_gv = Dummy_tmp;
}
inline Egse_systemtc_tc_generate_value(Egse_systemtc_tc_gv)
{
    if
    ::  true;
        Dummy_generate_value(Egse_systemtc_tc_gv.data.goToIdle);
        Egse_systemtc_tc_gv.selection = Egse_systemtc_tc_goToIdle_PRESENT;
    ::  true;
        OptimizedConfiguration_generate_value(Egse_systemtc_tc_gv.data.goToOperational);
        Egse_systemtc_tc_gv.selection = Egse_systemtc_tc_goToOperational_PRESENT;
    ::  true;
        Dummy_generate_value(Egse_systemtc_tc_gv.data.doReport);
        Egse_systemtc_tc_gv.selection = Egse_systemtc_tc_doReport_PRESENT;
    fi;
}
inline Sensor_reading_x_generate_value(Sensor_reading_x_gv)
{
    int Sensor_reading_x_tmp;
    if
    ::  atomic {
        select (Sensor_reading_x_tmp : 10 .. 10);
    }
    ::  atomic {
        select (Sensor_reading_x_tmp : 50 .. 50);
    }
    ::  atomic {
        select (Sensor_reading_x_tmp : 100 .. 100);
    }
    fi;
    Sensor_reading_x_gv = Sensor_reading_x_tmp;
}
inline DataItem_generate_value(DataItem_gv)
{
    int DataItem_tmp;
    if
    ::  atomic {
        select (DataItem_tmp : 0 .. 255);
    }
    fi;
    DataItem_gv = DataItem_tmp;
}
inline ActuatorConfiguration_generate_value(ActuatorConfiguration_gv)
{
    DataItem_generate_value(ActuatorConfiguration_gv.threshold1);
    DataItem_generate_value(ActuatorConfiguration_gv.threshold2);
}
inline SystemCommand_generate_value(SystemCommand_gv)
{
    if
    ::  true;
        Dummy_generate_value(SystemCommand_gv.data.goToIdle);
        SystemCommand_gv.selection = SystemCommand_goToIdle_PRESENT;
    ::  true;
        ActuatorConfiguration_generate_value(SystemCommand_gv.data.goToOperational);
        SystemCommand_gv.selection = SystemCommand_goToOperational_PRESENT;
    ::  true;
        Dummy_generate_value(SystemCommand_gv.data.doReport);
        SystemCommand_gv.selection = SystemCommand_doReport_PRESENT;
    fi;
}
