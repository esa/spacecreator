
with Simulation_DataView; use Simulation_Dataview;





with Clock_PI;

package Clock_Events is

   procedure Startup (Global_State : in out asn1SccSystem_State)
      renames Clock_PI.Startup;


   --  Process event: execute the provided interface for Input events
   --  Only report output events
   procedure Process_Event (Event        :        asn1sccObservable_Event;
                            Global_State : in out asn1SccSystem_State);

   procedure Print_Event (Event : asn1sccClock_Event);

   --  Helper function to determine the destination function of a GUI event
   -- (e.g. used by the simulator to highlight arrows in the statechart)
   function Get_Event_Dest (Event : access asn1sccObservable_Event) return asn1sccPID is
      (case Event.Output_Event.Event.Clock.Msg_Out.Kind is
          when Tick_PRESENT => asn1SccPinger,
          when others => asn1sccEnv)
   with Export, Convention => C, Link_Name => "clock_event_dest";

   --  For exhaustive simulation:
   procedure Exhaust_Interfaces
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean));

private
   procedure Exhaust_Tick
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean));
end Clock_Events;
