never{
  start: 
  do
  ::inited->
    break;
  ::else->
    goto start;
  od;
  system_inited: 
  do
  :: atomic {
    (global_state.actuator.x > 5)->
      printf("SCL violation: never actuator.x  > 5;\n");
      assert((! (global_state.actuator.x > 5)));
    }
  ::else->
    goto system_inited;
  od;
}
