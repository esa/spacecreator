-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- Generated by TASTE (kazoo/templates/glue/language_wrappers/ri_adb/function.tmplt)
with Simulator;            use Simulator;
with Simulation_Dataview;  use Simulation_Dataview;
with System_Dataview;      use System_Dataview;
package body Egse_RI is

   procedure Tc
      (Cfg : in out asn1SccTconfig;
       Dest_PID : asn1sccPID := asn1sccEnv) is
      --  Generate an event to the simulator for the output
      Event_Out_Modemanager : asn1SccObservable_Event (Kind => Output_Event_PRESENT);
   begin
      if Dest_PID = asn1sccEnv or Dest_PID = asn1sccModemanager then
         Event_Out_Modemanager.Output_Event :=
           (Source => asn1sccEgse,
            Dest   => asn1sccModemanager,
            Event  => (Kind => Egse_PRESENT,
                       Egse =>
                         (Kind   => Msg_Out_PRESENT,
                          Msg_Out =>
                             (Kind => Tc_PRESENT,
                        Tc =>
                              (Cfg => Cfg)))));
         Push_Event (Event_Out_Modemanager);
      end if;
   
   end Tc;

end Egse_RI;
