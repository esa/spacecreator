inline Systemcontrol_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.systemcontrol.state = Systemcontrol_States_wait;
      goto continuous_signals;
    ::(transition_id == 1)->
      Systemcontrol_0_get_sender(global_state.systemcontrol.sender);
      T_Float_assign_value(global_state.systemcontrol.xout, ((global_state.systemcontrol.xin * 2.5) - (global_state.systemcontrol.lastx * global_state.systemcontrol.lastx)));
      T_Float_assign_value(global_state.systemcontrol.lastx, global_state.systemcontrol.xin);
      Systemcontrol_0_RI_0_controlsignal(global_state.systemcontrol.xout);
      transition_id = -1;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Systemcontrol_0_init()
{
  PID_assign_value(global_state.systemcontrol.sender, PID_env);
  T_Float_assign_value(global_state.systemcontrol.xin, 0.0);
  T_Float_assign_value(global_state.systemcontrol.lastx, 0.0);
  T_Float_assign_value(global_state.systemcontrol.xout, 0.0);
  Systemcontrol_0_transition(0);
}
inline Systemcontrol_0_PI_0_reading(input_param)
{
  if
  ::(global_state.systemcontrol.state == Systemcontrol_States_wait)->
    T_Float_assign_value(global_state.systemcontrol.xin, input_param);
    Systemcontrol_0_transition(1);
  ::else->
    skip;
  fi;
}
inline Systemcontrol_0_PI_0_reporting()
{
  if
  ::else->
    skip;
  fi;
}
inline Systemcontrol_0_PI_0_tc(input_param)
{
  if
  ::else->
    skip;
  fi;
}
