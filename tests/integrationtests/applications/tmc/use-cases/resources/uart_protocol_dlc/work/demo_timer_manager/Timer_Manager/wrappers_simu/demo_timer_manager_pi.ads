-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
with Basic_Types;                use Basic_Types;
with Bilevel_Types;              use Bilevel_Types;
with Uart_Dlc;                   use Uart_Dlc;
with Taste_Basictypes;           use Taste_Basictypes;
with Uart_Protocol_Dlc_Dataview; use Uart_Protocol_Dlc_Dataview;
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
   procedure Reset_Rtdlcr_Frametimer (Global_State : in out asn1SccSystem_State);
   
   
   procedure Set_Rtdlcr_Frametimer
      (Global_State : in out asn1SccSystem_State;
      Val : in out asn1SccT_Uint32);
   
   
   
   procedure Tick (Global_State : in out asn1SccSystem_State);

end Demo_Timer_Manager_PI;
