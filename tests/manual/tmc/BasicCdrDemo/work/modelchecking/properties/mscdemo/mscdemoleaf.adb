-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body mscdemoleaf is
   function Observer_State_Status return asn1SccObserver_State_Kind is
      (case ctxt.State is
        when asn1Sccs2 => asn1SccError_State,
        when others => asn1SccRegular_State);
         

      procedure Execute_Transition (Id : Integer) is
         trId : Integer := Id;
         Message_Pending : Asn1Boolean := True;
         begin
            while (trId /= -1) loop
               case trId is
                  when 0 =>
                     --  NEXT_STATE s0 (23,26) at 0, 1837
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when 1 =>
                     --  NEXT_STATE s1 (29,30) at 0, 1987
                     trId := -1;
                     ctxt.State := asn1Sccs1;
                     goto Continuous_Signals;
                  when 2 =>
                     --  NEXT_STATE s0 (33,30) at 300, 1987
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when 3 =>
                     --  DECISION sig1_param0 (-1,-1)
                     --  ANSWER 0 (42,25)
                     if (event.input_event.event.ModeManager.msg_in.feedback.x) = asn1SccTInteger(0) then
                        --  NEXT_STATE s2 (44,38) at 900, 2062
                        trId := -1;
                        ctxt.State := asn1Sccs2;
                        goto Continuous_Signals;
                        --  ANSWER ELSE (None,None)
                     else
                        null;
                     end if;
                     --  NEXT_STATE s0 (49,30) at 2100, 2062
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when 4 =>
                     --  NEXT_STATE s1 (53,30) at 1200, 1987
                     trId := -1;
                     ctxt.State := asn1Sccs1;
                     goto Continuous_Signals;
                  when 5 =>
                     --  NEXT_STATE s0 (57,30) at 1500, 1987
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when CS_Only =>
                     trId := -1;
                     goto Continuous_Signals;
                  when others =>
                     null;
               end case;
               <<Continuous_Signals>>
               --  Process observer transitions
               Message_Pending := False;
               if Message_Pending or trId /= -1 then
                  goto Next_Transition;
               end if;
               if ctxt.State = asn1Sccs1 then
                  --  Priority: 1
                  --  DECISION sig1(sig1_param0) (38,22)
                  --  ANSWER true (None,None)
                  if ((((To_Observable_Event_Selection (event.Kind) = asn1Sccinput_event_PRESENT) and then (event.input_event.dest = asn1Sccmodemanager)) and then (To_Modemanager_Event_msg_in_Selection (event.input_event.event.modemanager.msg_in.Kind) = asn1Sccfeedback_PRESENT))) then
                     trId := 3;
                     --  Priority: 1
                     --  DECISION sig0(sig0_param0) (51,22)
                     --  ANSWER true (None,None)
                  elsif ((((To_Observable_Event_Selection (event.Kind) = asn1Sccinput_event_PRESENT) and then (event.input_event.dest = asn1Sccactuator)) and then (To_Actuator_Event_msg_in_Selection (event.input_event.event.actuator.msg_in.Kind) = asn1Sccctrl_PRESENT))) then
                     trId := 4;
                  end if;
               end if;
               if ctxt.State = asn1Sccs0 then
                  --  Priority: 1
                  --  DECISION sig0(sig0_param0) (27,22)
                  --  ANSWER true (None,None)
                  if ((((To_Observable_Event_Selection (event.Kind) = asn1Sccinput_event_PRESENT) and then (event.input_event.dest = asn1Sccactuator)) and then (To_Actuator_Event_msg_in_Selection (event.input_event.event.actuator.msg_in.Kind) = asn1Sccctrl_PRESENT))) then
                     trId := 1;
                  end if;
               end if;
               <<Next_Transition>>
            end loop;
         end Execute_Transition;
         

      procedure Startup is
         begin
            Execute_Transition (0);
            ctxt.Init_Done := True;
         end Startup;
      begin
         Startup;
end mscdemoleaf;