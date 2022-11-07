-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- (Generated by TASTE/Kazoo - $TOOL_SRC/kazoo/templates/glue/language_wrappers/simu_pi_adb/function.tmplt)
with Text_IO; use Text_IO;

package body Egse_PI is

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
      null;
   end Startup;

   procedure Continuous_Signals (Global_State : in out asn1SccSystem_State) is
   begin
     null;
   end Continuous_Signals;

   --  Provided interfaces
   procedure Poll (Global_State : in out asn1SccSystem_State;
                                  Sender_PID   : asn1SccPID := asn1SccEnv) is
   begin
      null;  -- ignore messages sent to ENV
      --  Put_Line ("[ENV] Received Poll");
   end Poll;
   
   
   procedure Tm
      (Global_State : in out asn1SccSystem_State;
      Reprt : in out asn1SccT_Report;
       Sender_PID : asn1SccPID := asn1SccEnv) is
   begin
      null;  -- ignore messages sent to ENV
      --  Put_Line ("[ENV] Received tm");
   end Tm;

end Egse_PI;
