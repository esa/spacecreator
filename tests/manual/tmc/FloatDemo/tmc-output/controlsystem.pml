inline Controlsystem_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.controlsystem.state = Controlsystem_States_off;
      goto continuous_signals;
    ::(transition_id == 1)->
      Controlsystem_0_get_sender(global_state.controlsystem.sender);
      transition_id = -1;
      global_state.controlsystem.state = Controlsystem_States_configured;
      goto continuous_signals;
    ::(transition_id == 2)->
      Controlsystem_0_get_sender(global_state.controlsystem.sender);
      {
        Number_assign_value(global_state.controlsystem.i, (global_state.controlsystem.i + global_state.controlsystem.inputvalue));
        Number_assign_value(global_state.controlsystem.d, (global_state.controlsystem.inputvalue - global_state.controlsystem.previous));
        Number_assign_value(global_state.controlsystem.previous, global_state.controlsystem.inputvalue);
      }
      Number_assign_value(global_state.controlsystem.outputvalue, (((global_state.controlsystem.cfg.kp * global_state.controlsystem.inputvalue) + (global_state.controlsystem.cfg.ki * global_state.controlsystem.i)) + (global_state.controlsystem.cfg.kd * global_state.controlsystem.d)));
      Controlsystem_0_RI_0_controlSignal(global_state.controlsystem.outputvalue);
      transition_id = -1;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Controlsystem_0_init()
{
  PID_assign_value(global_state.controlsystem.sender, PID_env);
  Number_assign_value(global_state.controlsystem.i, 0.0);
  Number_assign_value(global_state.controlsystem.d, 0.0);
  Number_assign_value(global_state.controlsystem.previous, 0.0);
  Controlsystem_0_transition(0);
}
inline Controlsystem_0_PI_0_configuration(input_param)
{
  if
  ::(global_state.controlsystem.state == Controlsystem_States_off)->
    PidConfig_assign_value(global_state.controlsystem.cfg, input_param);
    Controlsystem_0_transition(1);
  ::else->
    skip;
  fi;
}
inline Controlsystem_0_PI_0_reading(input_param)
{
  if
  ::(global_state.controlsystem.state == Controlsystem_States_configured)->
    Number_assign_value(global_state.controlsystem.inputvalue, input_param);
    Controlsystem_0_transition(2);
  ::else->
    skip;
  fi;
}
