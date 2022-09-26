-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- (Generated by TASTE/Kazoo - $TOOL_SRC/kazoo/templates/glue/language_wrappers/simu_pi_adb/function.tmplt)
package body Sharedmemory_PI is

   procedure Update_States_For_Synchronous_Calls (Global_State : asn1SccSystem_State) is
   begin
      --  No synchronous RIs are connected to this function
      null;
   end Update_States_For_Synchronous_Calls;

   procedure Update_Global_State_After_Synchronous_Calls (Global_State : in out asn1SccSystem_State)  is
   begin
      --  No synchronous RIs are connected to this function
      null;
   end Update_Global_State_After_Synchronous_Calls;

   procedure Startup (Global_State : in out asn1SccSystem_State) is
   begin
      --  Initialize the global state after startup function has run
      Global_State.Sharedmemory := Sharedmemory.Ctxt;
   end Startup;

   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State) is
   begin
      Sharedmemory.Ctxt := Global_State.Sharedmemory;
      Sharedmemory.Execute_Transition (Sharedmemory.CS_Only);
      Global_State.Sharedmemory := Sharedmemory.Ctxt;
   end Continuous_Signals;

   --  Provided interfaces
   procedure Memwritea
      (Global_State : in out asn1SccSystem_State;
      P1 : in out asn1SccT_Writeaccess) is
   begin
      -- Set the current context (provided by the simulation API)
      Sharedmemory.Ctxt := Global_State.Sharedmemory;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Sharedmemory.Memwritea
         (P1 => P1);
      -- Update the global state
      Global_State.Sharedmemory := Sharedmemory.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Memwritea;
   
   
   procedure Memwriteb
      (Global_State : in out asn1SccSystem_State;
      P1 : in out asn1SccT_Writeaccess) is
   begin
      -- Set the current context (provided by the simulation API)
      Sharedmemory.Ctxt := Global_State.Sharedmemory;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Sharedmemory.Memwriteb
         (P1 => P1);
      -- Update the global state
      Global_State.Sharedmemory := Sharedmemory.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Memwriteb;

end Sharedmemory_PI;
