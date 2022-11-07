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
      global_state.modemanager.state = Modemanager_States_uninitialized;
      goto continuous_signals;
    ::(transition_id == 1)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      {
        T_Integer_assign_value(global_state.modemanager.rprt.reading, global_state.modemanager.xin);
        T_Integer_assign_value(global_state.modemanager.rprt.ctrl, global_state.modemanager.xout);
        T_Integer_assign_value(global_state.modemanager.rprt.feedback, global_state.modemanager.xfeedback);
      }
      Modemanager_0_RI_0_tm(global_state.modemanager.rprt);
      transition_id = -1;
      goto continuous_signals;
    ::(transition_id == 2)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      T_Integer_assign_value(global_state.modemanager.xout, ((global_state.modemanager.xin * global_state.modemanager.cfg.param_a) + global_state.modemanager.cfg.param_b));
      Modemanager_0_RI_0_ctrl(global_state.modemanager.xout);
      transition_id = -1;
      goto continuous_signals;
    ::(transition_id == 3)->
      Modemanager_0_get_sender(global_state.modemanager.sender);
      transition_id = -1;
      global_state.modemanager.state = Modemanager_States_configured;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Modemanager_0_init()
{
  PID_assign_value(global_state.modemanager.sender, PID_env);
  Modemanager_0_transition(0);
}
inline Modemanager_0_PI_0_feedback(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_configured)->
    T_Integer_assign_value(global_state.modemanager.xfeedback, input_param);
    Modemanager_0_transition(1);
  ::else->
    skip;
  fi;
}
inline Modemanager_0_PI_0_reading(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_configured)->
    T_Integer_assign_value(global_state.modemanager.xin, input_param);
    Modemanager_0_transition(2);
  ::else->
    skip;
  fi;
}
inline Modemanager_0_PI_0_tc(input_param)
{
  if
  ::(global_state.modemanager.state == Modemanager_States_uninitialized)->
    T_Config_assign_value(global_state.modemanager.cfg, input_param);
    Modemanager_0_transition(3);
  ::else->
    skip;
  fi;
}
