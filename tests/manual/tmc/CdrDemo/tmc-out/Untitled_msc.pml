SystemCommand Untitled_MSC_0_sig0_param0;
SystemTelemetry Untitled_MSC_0_sig1_param0;
inline Untitled_msc_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.untitled_msc.state = Untitled_msc_States_s0;
      goto continuous_signals;
    ::(transition_id == 1)->
      if
      ::(Untitled_MSC_0_sig0_param0.selection == SystemCommand_doReport_PRESENT)->
        transition_id = -1;
        global_state.untitled_msc.state = Untitled_msc_States_s1;
        goto continuous_signals;
      ::else->
        skip;
      fi;
      transition_id = -1;
      global_state.untitled_msc.state = Untitled_msc_States_s0;
      goto continuous_signals;
    ::(transition_id == 2)->
      transition_id = -1;
      global_state.untitled_msc.state = Untitled_msc_States_s0;
      goto continuous_signals;
    ::(transition_id == 3)->
      transition_id = -1;
      global_state.untitled_msc.state = Untitled_msc_States_s2;
      goto continuous_signals;
    ::(transition_id == 4)->
      transition_id = -1;
      global_state.untitled_msc.state = Untitled_msc_States_serr;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Untitled_msc_0_check_continuous_signals()
{
  skip;
}
inline Untitled_msc_0_init()
{
  PID_assign_value(global_state.untitled_msc.sender, PID_env);
  Untitled_msc_0_transition(0);
}
inline Untitled_msc_0_PI_0_sig0(input_param)
{
  if
  ::(global_state.untitled_msc.state == Untitled_msc_States_s0)->
    SystemCommand_assign_value(Untitled_MSC_0_sig0_param0, input_param);
    Untitled_msc_0_transition(1);
  ::(global_state.untitled_msc.state == Untitled_msc_States_s1)->
    Untitled_msc_0_transition(4);
  ::else->
    skip;
  fi;
}
inline Untitled_msc_0_PI_0_sig1(input_param)
{
  if
  ::(global_state.untitled_msc.state == Untitled_msc_States_s0)->
    Untitled_msc_0_transition(2);
  ::(global_state.untitled_msc.state == Untitled_msc_States_s1)->
    SystemTelemetry_assign_value(Untitled_MSC_0_sig1_param0, input_param);
    Untitled_msc_0_transition(3);
  ::else->
    skip;
  fi;
}
