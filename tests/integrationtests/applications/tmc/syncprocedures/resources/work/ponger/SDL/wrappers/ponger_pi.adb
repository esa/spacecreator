-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- (Generated by TASTE/Kazoo - $TOOL_SRC/kazoo/templates/glue/language_wrappers/simu_pi_adb/function.tmplt)
with Pinger;
package body Ponger_PI is

   procedure Update_States_For_Synchronous_Calls (Global_State : asn1SccSystem_State) is
   begin
      Pinger.Ctxt := Global_State.Pinger;
   end Update_States_For_Synchronous_Calls;

   procedure Update_Global_State_After_Synchronous_Calls (Global_State : in out asn1SccSystem_State)  is
   begin
      Global_State.Pinger := Pinger.Ctxt;
   end Update_Global_State_After_Synchronous_Calls;

   procedure Startup (Global_State : in out asn1SccSystem_State) is
   begin
      --  Initialize the global state after startup function has run
      Global_State.Ponger := Ponger.Ctxt;
   end Startup;

   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State) is
   begin
      Ponger.Ctxt := Global_State.Ponger;
      Ponger.Execute_Transition (Ponger.CS_Only);
      Global_State.Ponger := Ponger.Ctxt;
   end Continuous_Signals;

   --  Provided interfaces
   procedure Ping
      (Global_State : in out asn1SccSystem_State;
      Pingarg : in out asn1SccMyinteger) is
   begin
      -- Set the current context (provided by the simulation API)
      Ponger.Ctxt := Global_State.Ponger;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Ponger.Ping
         (Pingarg => Pingarg);
      -- Update the global state
      Global_State.Ponger := Ponger.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Ping;

end Ponger_PI;
