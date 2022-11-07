T_Integer observerdemo_0_y;
T_Integer observerdemo_0_x;
inline Observerdemo_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.observerdemo.state = Observerdemo_States_idle;
      goto continuous_signals;
    ::(transition_id == 1)->
      if
      ::((observerdemo_0_y == global_state.observerdemo.reference) == false)->
        transition_id = -1;
        global_state.observerdemo.state = Observerdemo_States_incorrectvalue;
        goto continuous_signals;
      ::((observerdemo_0_y == global_state.observerdemo.reference) == true)->
        transition_id = -1;
        global_state.observerdemo.state = Observerdemo_States_idle;
        goto continuous_signals;
      fi;
    ::(transition_id == 2)->
      T_Integer_assign_value(global_state.observerdemo.reference, observerdemo_0_x);
      transition_id = -1;
      global_state.observerdemo.state = Observerdemo_States_checking;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Observerdemo_0_check_continuous_signals()
{
  skip;
}
inline Observerdemo_0_init()
{
  PID_assign_value(global_state.observerdemo.sender, PID_env);
  Observerdemo_0_transition(0);
}
inline Observerdemo_0_PI_0_ctrl_sig(input_param)
{
  if
  ::(global_state.observerdemo.state == Observerdemo_States_idle)->
    T_Integer_assign_value(observerdemo_0_x, input_param);
    Observerdemo_0_transition(2);
  ::else->
    skip;
  fi;
}
inline Observerdemo_0_PI_0_feedback_sig(input_param)
{
  if
  ::(global_state.observerdemo.state == Observerdemo_States_checking)->
    T_Integer_assign_value(observerdemo_0_y, input_param);
    Observerdemo_0_transition(1);
  ::else->
    skip;
  fi;
}
