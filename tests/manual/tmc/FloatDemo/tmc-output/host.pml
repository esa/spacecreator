inline Host_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      {
        Number_assign_value(global_state.host.cfg.kp, 1.3);
        Number_assign_value(global_state.host.cfg.kd, 2.1);
        Number_assign_value(global_state.host.cfg.ki, 3.2);
      }
      Host_0_RI_0_configuration(global_state.host.cfg);
      transition_id = -1;
      global_state.host.state = Host_States_wait;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Host_0_init()
{
  PID_assign_value(global_state.host.sender, PID_env);
  Host_0_transition(0);
}
inline Host_0_PI_0_tm()
{
  if
  ::else->
    skip;
  fi;
}
