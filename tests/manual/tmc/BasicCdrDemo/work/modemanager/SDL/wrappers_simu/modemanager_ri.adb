-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- Generated by TASTE (kazoo/templates/glue/language_wrappers/ri_adb/function.tmplt)
with Simulator;            use Simulator;
with Simulation_Dataview;  use Simulation_Dataview;
with System_Dataview;      use System_Dataview;
package body Modemanager_RI is

   procedure Ctrl
      (X : in out asn1SccTinteger;
       Dest_PID : asn1sccPID := asn1sccEnv) is
      --  Generate an event to the simulator for the output
      Event_Out_Actuator : asn1SccObservable_Event (Kind => Output_Event_PRESENT);
   begin
      if Dest_PID = asn1sccEnv or Dest_PID = asn1sccActuator then
         Event_Out_Actuator.Output_Event :=
           (Source => asn1sccModemanager,
            Dest   => asn1sccActuator,
            Event  => (Kind => Modemanager_PRESENT,
                       Modemanager =>
                         (Kind   => Msg_Out_PRESENT,
                          Msg_Out =>
                             (Kind => Ctrl_PRESENT,
                        Ctrl =>
                              (X => X)))));
         Push_Event (Event_Out_Actuator);
      end if;
   
   end Ctrl;
   
   
   
   procedure Tm
      (Reprt : in out asn1SccTreport;
       Dest_PID : asn1sccPID := asn1sccEnv) is
      --  Generate an event to the simulator for the output
      Event_Out_Egse : asn1SccObservable_Event (Kind => Output_Event_PRESENT);
   begin
      if Dest_PID = asn1sccEnv or Dest_PID = asn1sccEgse then
         Event_Out_Egse.Output_Event :=
           (Source => asn1sccModemanager,
            Dest   => asn1sccEgse,
            Event  => (Kind => Modemanager_PRESENT,
                       Modemanager =>
                         (Kind   => Msg_Out_PRESENT,
                          Msg_Out =>
                             (Kind => Tm_PRESENT,
                        Tm =>
                              (Reprt => Reprt)))));
         Push_Event (Event_Out_Egse);
      end if;
   
   end Tm;

end Modemanager_RI;
