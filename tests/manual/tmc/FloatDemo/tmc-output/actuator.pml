inline Actuator_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.actuator.state = Actuator_States_idle;
      goto continuous_signals;
    ::(transition_id == 1)->
      Actuator_0_get_sender(global_state.actuator.sender);
      Number_assign_value(global_state.actuator.setting, (1.5 * global_state.actuator.inputcontrol));
      if
      ::((global_state.actuator.setting > 10.0) == true)->
        transition_id = -1;
        global_state.actuator.state = Actuator_States_activated;
        goto continuous_signals;
      ::((global_state.actuator.setting > 10.0) == false)->
        transition_id = -1;
        global_state.actuator.state = Actuator_States_idle;
        goto continuous_signals;
      fi;
    fi;
    continuous_signals: 
  od;
}
inline Actuator_0_init()
{
  PID_assign_value(global_state.actuator.sender, PID_env);
  Number_assign_value(global_state.actuator.setting, 0.0);
  Actuator_0_transition(0);
}
inline Actuator_0_PI_0_controlSignal(input_param)
{
  if
  ::(global_state.actuator.state == Actuator_States_activated)->
    Number_assign_value(global_state.actuator.inputcontrol, input_param);
    Actuator_0_transition(1);
  ::(global_state.actuator.state == Actuator_States_idle)->
    Number_assign_value(global_state.actuator.inputcontrol, input_param);
    Actuator_0_transition(1);
  ::else->
    skip;
  fi;
}
