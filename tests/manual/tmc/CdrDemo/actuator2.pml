inline Actuator2_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.actuator2.state = Actuator_States_standby;
      goto continuous_signals;
    ::(transition_id == 1)->
      Actuator2_0_get_sender(global_state.actuator2.sender);
      if
      ::(global_state.actuator2.cmd == ActuatorCommand_actuatorStandby)->
        Actuator2_0_RI_0_actuatorTm(ActuatorStatus_actuatorStandingBy);
        transition_id = -1;
        global_state.actuator2.state = Actuator_States_standby;
        goto continuous_signals;
      ::(global_state.actuator2.cmd == ActuatorCommand_actuatorExecute)->
        Actuator2_0_RI_0_actuatorTm(ActuatorStatus_actuatorExecuting);
        transition_id = -1;
        global_state.actuator2.state = Actuator_States_executing;
        goto continuous_signals;
      ::else->
        transition_id = -1;
        goto continuous_signals;
      fi;
    ::(transition_id == 2)->
      Actuator2_0_get_sender(global_state.actuator2.sender);
      if
      ::(global_state.actuator2.cmd == ActuatorCommand_actuatorStandby)->
        Actuator2_0_RI_0_actuatorTm(ActuatorStatus_actuatorStandingBy);
        transition_id = -1;
        global_state.actuator2.state = Actuator_States_standby;
        goto continuous_signals;
      ::else->
        transition_id = -1;
        goto continuous_signals;
      fi;
    ::(transition_id == 3)->
      Actuator2_0_get_sender(global_state.actuator2.sender);
      if
      ::(global_state.actuator2.cmd == ActuatorCommand_actuatorArm)->
        Actuator2_0_RI_0_actuatorTm(ActuatorStatus_actuatorArmed);
        transition_id = -1;
        global_state.actuator2.state = Actuator_States_armed;
        goto continuous_signals;
      ::else->
        transition_id = -1;
        goto continuous_signals;
      fi;
    fi;
    continuous_signals: 
  od;
}
inline Actuator2_0_init()
{
  PID_assign_value(global_state.actuator2.sender, PID_env);
  Actuator2_0_transition(0);
}
inline Actuator2_0_PI_0_actuatorTc(input_param)
{
  if
  ::(global_state.actuator2.state == Actuator_States_armed)->
    ActuatorCommand_assign_value(global_state.actuator2.cmd, input_param);
    Actuator2_0_transition(1);
  ::(global_state.actuator2.state == Actuator_States_executing)->
    ActuatorCommand_assign_value(global_state.actuator2.cmd, input_param);
    Actuator2_0_transition(2);
  ::(global_state.actuator2.state == Actuator_States_standby)->
    ActuatorCommand_assign_value(global_state.actuator2.cmd, input_param);
    Actuator2_0_transition(3);
  ::else->
    skip;
  fi;
}
