
with Simulation_DataView; use Simulation_Dataview;

with System_Dataview;  use System_Dataview;   -- definition of the PID type

with Constrained_Absent_Dataview; use Constrained_Absent_Dataview;


with Constrained_Absent_Dataview; use Constrained_Absent_Dataview;

with Function_1_PI;

package Function_1_Events is

   procedure Startup (Global_State : in out asn1SccSystem_State)
      renames Function_1_PI.Startup;


   --  Process event: execute the provided interface for Input events
   --  Only report output events
   procedure Process_Event (Event        :        asn1sccObservable_Event;
                            Global_State : in out asn1SccSystem_State);

   procedure Print_Event (Event : asn1sccFunction_1_Event);

   --  Helper function to determine the destination function of a GUI event
   -- (e.g. used by the simulator to highlight arrows in the statechart)
   function Get_Event_Dest (Event : access asn1sccObservable_Event) return asn1sccPID is
      (case Event.Output_Event.Event.Function_1.Msg_Out.Kind is
          when Pi_1_PRESENT => asn1SccFunction_2,
          when others => asn1sccEnv)
   with Export, Convention => C, Link_Name => "function_1_event_dest";

   --  For exhaustive simulation:
   procedure Exhaust_Interfaces
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean));

private
   procedure Exhaust_Pi_1
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean));
end Function_1_Events;
