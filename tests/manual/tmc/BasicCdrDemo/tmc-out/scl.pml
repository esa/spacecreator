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
    (global_state.observerdemo.state == Observerdemo_States_incorrectvalue)->
      printf("Observer entered errorstate: observerdemo\n");
      assert((! (global_state.observerdemo.state == Observerdemo_States_incorrectvalue)));
    }
  ::else->
    goto system_inited;
  od;
}
