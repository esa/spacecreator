#define SystemCommand_goToIdle_PRESENT 1
#define SystemCommand_goToOperational_PRESENT 2
#define SystemCommand_doReport_PRESENT 3
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
