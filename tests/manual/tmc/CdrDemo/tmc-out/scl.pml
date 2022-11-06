never{
  start: 
  do
  ::inited->
    break;
  ::else->
    goto start;
  od;
  accept_all: 
  do
  :: atomic {
    (global_state.untitled_msc.state == Untitled_msc_States_serr)->
      printf("Observer entered errorstate: untitled_msc\n");
      assert((! (global_state.untitled_msc.state == Untitled_msc_States_serr)));
    }
  ::(global_state.untitled_msc.state == Untitled_msc_States_s2)->
    goto state_0;
  ::else->
    goto accept_all;
  od;
  state_0: 
  do
  :: atomic {
    (global_state.untitled_msc.state == Untitled_msc_States_serr)->
      printf("Observer entered errorstate: untitled_msc\n");
      assert((! (global_state.untitled_msc.state == Untitled_msc_States_serr)));
    }
  ::else->
    goto state_0;
  od;
}
