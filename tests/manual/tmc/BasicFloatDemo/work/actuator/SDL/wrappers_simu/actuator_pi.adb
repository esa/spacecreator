-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- (Generated by TASTE/Kazoo - $TOOL_SRC/kazoo/templates/glue/language_wrappers/simu_pi_adb/function.tmplt)
package body Actuator_PI is

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
      Global_State.Actuator := Actuator.Ctxt;
   end Startup;

   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State) is
   begin
      Actuator.Ctxt := Global_State.Actuator;
      Actuator.Execute_Transition (Actuator.CS_Only);
      Global_State.Actuator := Actuator.Ctxt;
   end Continuous_Signals;

   --  Provided interfaces
   procedure Controlsignal
      (Global_State : in out asn1SccSystem_State;
      X : in out asn1SccT_Float;
       Sender_PID : asn1SccPID := asn1SccEnv) is
   begin
      -- Set the current context (provided by the simulation API)
      Actuator.Ctxt := Global_State.Actuator;
      Actuator.Ctxt.Sender := Sender_PID;
      --  If user code calls synchronous RIs, set their state too
      Update_States_For_Synchronous_Calls (Global_State);
      Actuator.Controlsignal
         (X => X);
      -- Update the global state
      Global_State.Actuator := Actuator.Ctxt;
      --  If user code called synchronous RIs, populate their state
      Update_Global_State_After_Synchronous_Calls (Global_State);
   end Controlsignal;

end Actuator_PI;
