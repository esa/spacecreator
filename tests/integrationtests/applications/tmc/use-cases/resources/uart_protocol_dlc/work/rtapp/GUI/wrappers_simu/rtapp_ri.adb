-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- Generated by Kazoo (templates/glue/language_wrappers/ri_adb/function.tmplt)
-- Date : 2022-09-30 02:04:18
with Simulator;            use Simulator;
with Simulation_Dataview;  use Simulation_Dataview;
with System_Dataview;      use System_Dataview;
package body Rtapp_RI is

   procedure Dlcrxreset is
      --  Generate an event to the simulator for the output
      Event_Out_Rtdlcr : asn1SccObservable_Event (Kind => Output_Event_PRESENT);
   begin
      Event_Out_Rtdlcr.Output_Event :=
        (Source => asn1sccRtapp,
         Dest   => asn1sccRtdlcr,
         Event  => (Kind => Rtapp_PRESENT,
                    Rtapp =>
                      (Kind   => Msg_Out_PRESENT,
                       Msg_Out =>
                          (Kind => Dlcrxreset_PRESENT,
                        Dlcrxreset => (null record)))));
      Push_Event (Event_Out_Rtdlcr);
   
   end Dlcrxreset;

end Rtapp_RI;
