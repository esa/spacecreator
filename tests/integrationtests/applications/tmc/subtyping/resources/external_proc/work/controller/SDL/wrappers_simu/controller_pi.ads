-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
with Externalproc_Dataview; use Externalproc_Dataview;
with Taste_Basictypes;      use Taste_Basictypes;
with Simulation_Dataview; use Simulation_Dataview;
with Controller;

package Controller_PI is
   --  Interface to the simulation API
   procedure Startup (Global_State : in out asn1SccSystem_State);
   --  Execute only continuous signals (the simulator shall let the user
   --  send intermediate PIs in the queue since they have priority over
   --  continuous signals)
   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State);

   --  Provided interfaces
   procedure Test (Global_State : in out asn1SccSystem_State);

end Controller_PI;
