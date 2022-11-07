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
      if
      ::((global_state.actuator.x > 10) == true)->
        Actuator_0_RI_0_feedback(global_state.actuator.x);
        transition_id = -1;
        global_state.actuator.state = Actuator_States_working;
        goto continuous_signals;
      ::((global_state.actuator.x > 10) == false)->
        Actuator_0_RI_0_feedback(0);
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
  Actuator_0_transition(0);
}
inline Actuator_0_PI_0_ctrl(input_param)
{
  if
  ::(global_state.actuator.state == Actuator_States_idle)->
    T_Integer_assign_value(global_state.actuator.x, input_param);
    Actuator_0_transition(1);
  ::(global_state.actuator.state == Actuator_States_working)->
    T_Integer_assign_value(global_state.actuator.x, input_param);
    Actuator_0_transition(1);
  ::else->
    skip;
  fi;
}
