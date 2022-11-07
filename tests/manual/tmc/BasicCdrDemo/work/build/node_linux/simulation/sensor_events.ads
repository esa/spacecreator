
with Simulation_DataView; use Simulation_Dataview;

with System_Dataview;  use System_Dataview;   -- definition of the PID type



with Basiccdrdemo_Dataview; use Basiccdrdemo_Dataview;

with Sensor_PI;

package Sensor_Events is

   procedure Startup (Global_State : in out asn1SccSystem_State)
      renames Sensor_PI.Startup;


   --  Process event: execute the provided interface for Input events
   --  Only report output events
   procedure Process_Event (Event        :        asn1sccObservable_Event;
                            Global_State : in out asn1SccSystem_State);

   procedure Print_Event (Event : asn1sccSensor_Event);

   --  Helper function to determine the destination function of a GUI event
   -- (e.g. used by the simulator to highlight arrows in the statechart)
   function Get_Event_Dest (Event : access asn1sccObservable_Event) return asn1sccPID is
      (case Event.Output_Event.Event.Sensor.Msg_Out.Kind is
          when Reading_PRESENT => asn1SccModemanager,
          when others => asn1sccEnv)
   with Export, Convention => C, Link_Name => "sensor_event_dest";

   --  For exhaustive simulation:
   procedure Exhaust_Interfaces
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean));

private
   procedure Exhaust_Reading
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean));
end Sensor_Events;
