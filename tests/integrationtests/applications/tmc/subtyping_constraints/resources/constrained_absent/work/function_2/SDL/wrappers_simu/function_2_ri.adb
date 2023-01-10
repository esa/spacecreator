-- DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
-- Generated by Kazoo (templates/glue/language_wrappers/ri_adb/function.tmplt)
-- Date : 2023-01-10 11:55:33
with Simulator;            use Simulator;
with Simulation_Dataview;  use Simulation_Dataview;
with System_Dataview;      use System_Dataview;
package body Function_2_RI is

   procedure Pi_2
      (P1 : in out asn1SccMychoice) is
      --  Generate an event to the simulator for the output
      Event_Out_Function_1 : asn1SccObservable_Event (Kind => Output_Event_PRESENT);
   begin
      Event_Out_Function_1.Output_Event :=
        (Source => asn1sccFunction_2,
         Dest   => asn1sccFunction_1,
         Event  => (Kind => Function_2_PRESENT,
                    Function_2 =>
                      (Kind   => Msg_Out_PRESENT,
                       Msg_Out =>
                          (Kind => Pi_2_PRESENT,
                     Pi_2 =>
                           (P1 => P1)))));
      Push_Event (Event_Out_Function_1);
   
   end Pi_2;

end Function_2_RI;
