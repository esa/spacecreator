-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Pinger is
   procedure ClockInterface is
      begin
         case ctxt.state is
            when asn1Sccwait =>
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end ClockInterface;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  value := 0 (12,13)
                  ctxt.value := 0;
                  --  NEXT_STATE Wait (14,18) at 320, 114
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when 1 =>
                  --  value := value + 1 (20,17)
                  ctxt.value := (ctxt.value + 1);
                  --  PingInterface(value) (22,19)
                  RI_0_PingInterface(ctxt.value);
                  --  NEXT_STATE Wait (24,22) at 449, 231
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when CS_Only =>
                  trId := -1;
                  goto Continuous_Signals;
               when others =>
                  null;
            end case;
            <<Continuous_Signals>>
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
end Pinger;