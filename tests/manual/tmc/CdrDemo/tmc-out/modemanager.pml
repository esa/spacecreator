inline Modemanager_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.modemanager.state = Modemanager_States_idle;
      goto continuous_signals;
    ::(transition_id == 1)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      if
      ::(global_state.modemanager.stc.selection == SystemCommand_goToOperational_PRESENT)->
        DataItem_assign_value(global_state.modemanager.threshold1, global_state.modemanager.stc.data.goToOperational.threshold1);
        DataItem_assign_value(global_state.modemanager.threshold2, global_state.modemanager.stc.data.goToOperational.threshold2);
        Modemanager_0_RI_0_actuatorTc1(ActuatorCommand_actuatorArm);
        Modemanager_0_RI_0_actuatorTc2(ActuatorCommand_actuatorArm);
        transition_id = -1;
        global_state.modemanager.state = Modemanager_States_starting;
        goto continuous_signals;
      ::(global_state.modemanager.stc.selection == SystemCommand_doReport_PRESENT)->
        {
          ActuatorStatus_assign_value(global_state.modemanager.stm.status1, global_state.modemanager.status1);
          ActuatorStatus_assign_value(global_state.modemanager.stm.status2, global_state.modemanager.status2);
          DataItem_assign_value(global_state.modemanager.stm.reading, global_state.modemanager.r);
        }
        Modemanager_0_RI_0_systemTm(global_state.modemanager.stm);
        transition_id = -1;
        goto continuous_signals;
      ::else->
        transition_id = -1;
        goto continuous_signals;
      fi;
    ::(transition_id == 2)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      if
      ::(global_state.modemanager.stc.selection == SystemCommand_goToIdle_PRESENT)->
        Modemanager_0_RI_0_actuatorTc1(ActuatorCommand_actuatorStandby);
        Modemanager_0_RI_0_actuatorTc2(ActuatorCommand_actuatorStandby);
        transition_id = -1;
        global_state.modemanager.state = Modemanager_States_stopping;
        goto continuous_signals;
      ::(global_state.modemanager.stc.selection == SystemCommand_doReport_PRESENT)->
        {
          ActuatorStatus_assign_value(global_state.modemanager.stm.status1, global_state.modemanager.status1);
          ActuatorStatus_assign_value(global_state.modemanager.stm.status2, global_state.modemanager.status2);
          DataItem_assign_value(global_state.modemanager.stm.reading, global_state.modemanager.r);
        }
        Modemanager_0_RI_0_systemTm(global_state.modemanager.stm);
        transition_id = -1;
        goto continuous_signals;
      ::else->
        transition_id = -1;
        goto continuous_signals;
      fi;
    ::(transition_id == 3)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      if
      ::(global_state.modemanager.r < global_state.modemanager.threshold1)->
        Modemanager_0_RI_0_actuatorTc1(ActuatorCommand_actuatorExecute);
      ::else->
        Modemanager_0_RI_0_actuatorTc1(ActuatorCommand_actuatorStandby);
      fi;
      if
      ::(global_state.modemanager.r > global_state.modemanager.threshold2)->
        Modemanager_0_RI_0_actuatorTc2(ActuatorCommand_actuatorExecute);
      ::else->
        Modemanager_0_RI_0_actuatorTc2(ActuatorCommand_actuatorStandby);
      fi;
      transition_id = -1;
      goto continuous_signals;
    ::(transition_id == 4)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      goto startcheck;
    ::(transition_id == 5)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      startcheck: 
      if
      ::(((global_state.modemanager.status1 == ActuatorStatus_actuatorArmed) && (global_state.modemanager.status2 == ActuatorStatus_actuatorArmed)) == false)->
        transition_id = -1;
        goto continuous_signals;
      ::(((global_state.modemanager.status1 == ActuatorStatus_actuatorArmed) && (global_state.modemanager.status2 == ActuatorStatus_actuatorArmed)) == true)->
        transition_id = -1;
        global_state.modemanager.state = Modemanager_States_operational;
        goto continuous_signals;
      fi;
    ::(transition_id == 6)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      goto stopcheck;
    ::(transition_id == 7)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      stopcheck: 
      if
      ::(((global_state.modemanager.status1 == ActuatorStatus_actuatorStandingBy) && (global_state.modemanager.status2 == ActuatorStatus_actuatorStandingBy)) == false)->
        transition_id = -1;
        goto continuous_signals;
      ::(((global_state.modemanager.status1 == ActuatorStatus_actuatorStandingBy) && (global_state.modemanager.status2 == ActuatorStatus_actuatorStandingBy)) == true)->
        transition_id = -1;
        global_state.modemanager.state = Modemanager_States_idle;
        goto continuous_signals;
      fi;
    fi;
    continuous_signals: 
  od;
}
inline Modemanager_0_init()
{
  PID_assign_value(global_state.modemanager.sender, PID_env);
  Modemanager_0_transition(0);
}
inline Modemanager_0_PI_0_actuatorTm1(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_starting)->
    ActuatorStatus_assign_value(global_state.modemanager.status1, input_param);
    Modemanager_0_transition(5);
  ::(global_state.modemanager.state == Modemanager_States_stopping)->
    ActuatorStatus_assign_value(global_state.modemanager.status1, input_param);
    Modemanager_0_transition(7);
  ::else->
    skip;
  fi;
}
inline Modemanager_0_PI_0_actuatorTm2(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_starting)->
    ActuatorStatus_assign_value(global_state.modemanager.status2, input_param);
    Modemanager_0_transition(4);
  ::(global_state.modemanager.state == Modemanager_States_stopping)->
    ActuatorStatus_assign_value(global_state.modemanager.status2, input_param);
    Modemanager_0_transition(6);
  ::else->
    skip;
  fi;
}
inline Modemanager_0_PI_0_reading(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_operational)->
    DataItem_assign_value(global_state.modemanager.r, input_param);
    Modemanager_0_transition(3);
  ::else->
    skip;
  fi;
}
inline Modemanager_0_PI_0_systemTc(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_idle)->
    SystemCommand_assign_value(global_state.modemanager.stc, input_param);
    Modemanager_0_transition(1);
  ::(global_state.modemanager.state == Modemanager_States_operational)->
    SystemCommand_assign_value(global_state.modemanager.stc, input_param);
    Modemanager_0_transition(2);
  ::else->
    skip;
  fi;
}
