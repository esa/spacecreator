-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
with Cdrdemo_Dataview; use Cdrdemo_Dataview;
with Taste_Basictypes; use Taste_Basictypes;
with System_Dataview;  use System_Dataview;
with System_Dataview;     use System_Dataview;  -- PID type
with Simulation_Dataview; use Simulation_Dataview;
with Actuator1;

package Actuator1_PI is
   --  Interface to the simulation API
   procedure Startup (Global_State : in out asn1SccSystem_State);

   --  Stub for the Get_Sender function. Simulator does not need it as the sender
   --  is passed directly from the _Events to the _PI function and the state
   --  of the function is immediately updated with the value.
   procedure Dummy_Get_PID (Dummy_Param : asn1SccPID) is null
      with Export, Convention => C, Link_Name => "actuator1_get_sender";
   --  Execute only continuous signals (the simulator shall let the user
   --  send intermediate PIs in the queue since they have priority over
   --  continuous signals)
   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State);

   --  Provided interfaces
   procedure Actuatortc
      (Global_State : in out asn1SccSystem_State;
      Tc : in out asn1SccActuatorcommand;
       Sender_PID : asn1SccPID := asn1SccEnv);

end Actuator1_PI;
