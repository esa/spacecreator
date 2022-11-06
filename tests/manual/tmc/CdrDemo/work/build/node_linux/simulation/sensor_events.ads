
with Simulation_DataView; use Simulation_Dataview;

with System_Dataview;  use System_Dataview;   -- definition of the PID type



with Cdrdemo_Dataview; use Cdrdemo_Dataview;

with Sensor_PI;

package Sensor_Events is

   procedure Startup (Global_State : in out asn1SccSystem_State)
      renames Sensor_PI.Startup;


   --  Process event: execute the provided interface for Input events
   --  Only report output events
   procedure Process_Event (Event        :        asn1sccObservable_Event;
                            Global_State : in out asn1SccSystem_State);

   procedure Print_Event (Event : asn1sccSensor_Event);

end Sensor_Events;
