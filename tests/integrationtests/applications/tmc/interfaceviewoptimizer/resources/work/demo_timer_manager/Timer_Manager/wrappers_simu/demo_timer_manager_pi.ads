-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
with Environment_Selector_Dataview; use Environment_Selector_Dataview;
with Taste_Basictypes;              use Taste_Basictypes;
with Simulation_Dataview; use Simulation_Dataview;
with Demo_Timer_Manager;

package Demo_Timer_Manager_PI is
   --  Interface to the simulation API
   procedure Startup (Global_State : in out asn1SccSystem_State);
   --  Execute only continuous signals (the simulator shall let the user
   --  send intermediate PIs in the queue since they have priority over
   --  continuous signals)
   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State);

   --  Provided interfaces
   procedure Reset_Pinger_Pingertimer (Global_State : in out asn1SccSystem_State);
   
   
   procedure Reset_Ponger_Pongertimer (Global_State : in out asn1SccSystem_State);
   
   
   procedure Set_Pinger_Pingertimer
      (Global_State : in out asn1SccSystem_State;
      Val : in out asn1SccT_Uint32);
   
   
   
   procedure Set_Ponger_Pongertimer
      (Global_State : in out asn1SccSystem_State;
      Val : in out asn1SccT_Uint32);
   
   
   
   procedure Tick (Global_State : in out asn1SccSystem_State);

end Demo_Timer_Manager_PI;
