inline Sensor_0_transition(id)
{
  int transition_id;
  transition_id = id;
  do
  ::if
    ::(transition_id == -1)->
      break;
    ::(transition_id == 0)->
      Number_assign_value(global_state.sensor.x, 1.0);
      Sensor_0_trigger_set(1000);
      transition_id = -1;
      global_state.sensor.state = Sensor_States_wait;
      goto continuous_signals;
    ::(transition_id == 1)->
      Sensor_0_get_sender(global_state.sensor.sender);
      Number_assign_value(global_state.sensor.x, (global_state.sensor.x * 2.0));
      Sensor_0_RI_0_reading(global_state.sensor.x);
      Sensor_0_trigger_set(1000);
      transition_id = -1;
      global_state.sensor.state = Sensor_States_wait;
      goto continuous_signals;
    fi;
    continuous_signals: 
  od;
}
inline Sensor_0_init()
{
  PID_assign_value(global_state.sensor.sender, PID_env);
  Sensor_0_transition(0);
}
inline Sensor_0_PI_0_tc()
{
  if
  ::else->
    skip;
  fi;
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
