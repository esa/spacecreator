-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Untitled_MSC is
   function Observer_State_Status return asn1SccObserver_State_Kind is
      (case ctxt.State is
        when asn1Sccserr => asn1SccError_State,
        when asn1Sccs2 => asn1SccSuccess_State,
        when others => asn1SccRegular_State);
         

      procedure Execute_Transition (Id : Integer) is
         trId : Integer := Id;
         Message_Pending : Asn1Boolean := True;
         begin
            while (trId /= -1) loop
               case trId is
                  when 0 =>
                     --  NEXT_STATE s0 (24,26) at 0, 1837
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when 1 =>
                     --  DECISION present(sig0_param0) (-1,-1)
                     --  ANSWER doReport (32,25)
                     if (To_SystemCommand_Selection (event.input_event.event.ModeManager.msg_in.systemTc.tc.Kind)) = asn1SccdoReport_PRESENT then
                        --  NEXT_STATE s1 (34,38) at 0, 2062
                        trId := -1;
                        ctxt.State := asn1Sccs1;
                        goto Continuous_Signals;
                        --  ANSWER ELSE (None,None)
                     else
                        null;
                     end if;
                     --  NEXT_STATE s0 (39,30) at 1200, 2062
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when 2 =>
                     --  NEXT_STATE s0 (43,30) at 300, 1987
                     trId := -1;
                     ctxt.State := asn1Sccs0;
                     goto Continuous_Signals;
                  when 3 =>
                     --  NEXT_STATE s2 (50,30) at 1500, 1987
                     trId := -1;
                     ctxt.State := asn1Sccs2;
                     goto Continuous_Signals;
                  when 4 =>
                     --  NEXT_STATE sErr (54,30) at 1800, 1987
                     trId := -1;
                     ctxt.State := asn1SccsErr;
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
               if ctxt.State = asn1Sccs0 then
                  --  Priority: 1
                  --  DECISION sig0(sig0_param0) (28,22)
                  --  ANSWER true (None,None)
                  if ((((To_Observable_Event_Selection (event.Kind) = asn1Sccinput_event_PRESENT) and then (event.input_event.dest = asn1Sccmodemanager)) and then (To_Modemanager_Event_msg_in_Selection (event.input_event.event.modemanager.msg_in.Kind) = asn1Sccsystemtc_PRESENT))) then
                     trId := 1;
                  end if;
               end if;
               if ctxt.State = asn1Sccs1 then
                  --  Priority: 1
                  --  DECISION sig1(sig1_param0) (48,22)
                  --  ANSWER true (None,None)
                  if ((((To_Observable_Event_Selection (event.Kind) = asn1Sccinput_event_PRESENT) and then (event.input_event.dest = asn1Sccegse)) and then (To_Egse_Event_msg_in_Selection (event.input_event.event.egse.msg_in.Kind) = asn1Sccsystemtm_PRESENT))) then
                     trId := 3;
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
end Untitled_MSC;