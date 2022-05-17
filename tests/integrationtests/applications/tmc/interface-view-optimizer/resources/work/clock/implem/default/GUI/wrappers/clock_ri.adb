-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- Generated by Kazoo (templates/glue/language_wrappers/ri_adb/function.tmplt)
-- Date : 2022-05-16 12:08:08
with Simulator;            use Simulator;
with Simulation_Dataview;  use Simulation_Dataview;
package body Clock_RI is

   procedure Clockinterface is
      --  Generate an event to the simulator for the output
      Event_Out_Pinger : asn1SccObservable_Event (Kind => Output_Event_PRESENT);
   begin
      Event_Out_Pinger.Output_Event :=
        (Source => asn1sccClock,
         Dest   => asn1sccPinger,
         Event  => (Kind => Clock_PRESENT,
                    Clock =>
                      (Kind   => Msg_Out_PRESENT,
                       Msg_Out =>
                          (Kind => Clockinterface_PRESENT,
                        Clockinterface => (null record)))));
      Push_Event (Event_Out_Pinger);
   
   end Clockinterface;

end Clock_RI;
