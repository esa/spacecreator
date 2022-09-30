-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- (Generated by TASTE/Kazoo - $TOOL_SRC/kazoo/templates/glue/language_wrappers/simu_pi_adb/function.tmplt)
package body Erroredchannel_PI is

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
      Global_State.Erroredchannel := Erroredchannel.Ctxt;
   end Startup;

   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State) is
   begin
      Erroredchannel.Ctxt := Global_State.Erroredchannel;
      Erroredchannel.Execute_Transition (Erroredchannel.CS_Only);
      Global_State.Erroredchannel := Erroredchannel.Ctxt;
   end Continuous_Signals;

   --  Provided interfaces
   procedure Corruptchar
      (Global_State : in out asn1SccSystem_State;
      P1 : in out asn1SccUint8) is
   begin
      -- Set the current context (provided by the simulation API)
      Erroredchannel.Ctxt := Global_State.Erroredchannel;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Erroredchannel.Corruptchar
         (P1 => P1);
      -- Update the global state
      Global_State.Erroredchannel := Erroredchannel.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Corruptchar;
   
   
   procedure Dropchar (Global_State : in out asn1SccSystem_State) is
   begin
      -- Set the current context (provided by the simulation API)
      Erroredchannel.Ctxt := Global_State.Erroredchannel;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Erroredchannel.Dropchar;
      -- Update the global state
      Global_State.Erroredchannel := Erroredchannel.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Dropchar;
   
   
   procedure Phytxchar
      (Global_State : in out asn1SccSystem_State;
      P1 : in out asn1SccUint8) is
   begin
      -- Set the current context (provided by the simulation API)
      Erroredchannel.Ctxt := Global_State.Erroredchannel;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Erroredchannel.Phytxchar
         (P1 => P1);
      -- Update the global state
      Global_State.Erroredchannel := Erroredchannel.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Phytxchar;

end Erroredchannel_PI;
