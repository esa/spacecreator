inline Sensor_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      transition_id = -1;
      global_state.sensor.state = Sensor_States_wait;
      goto continuous_signals;
    ::(transition_id == 1)->
      Sensor_0_get_sender(global_state.sensor.sender);
      T_Float_assign_value(global_state.sensor.x, (global_state.sensor.x + 1.0));
      if
      ::((global_state.sensor.x > 10.0) == true)->
        T_Float_assign_value(global_state.sensor.x, 0.0);
      ::((global_state.sensor.x > 10.0) == false)->
        skip;
      fi;
      Sensor_0_RI_0_reading(global_state.sensor.x);
      transition_id = -1;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Sensor_0_init()
{
  PID_assign_value(global_state.sensor.sender, PID_env);
  T_Float_assign_value(global_state.sensor.x, 0.0);
  Sensor_0_transition(0);
}
inline Sensor_0_PI_0_trigger()
{
  if
  ::(global_state.sensor.state == Sensor_States_wait)->
    Sensor_0_transition(1);
  ::else->
    skip;
  fi;
}
