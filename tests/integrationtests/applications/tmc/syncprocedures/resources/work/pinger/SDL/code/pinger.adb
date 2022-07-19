-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Pinger is
   procedure Pong(pongArg: in out asn1SccMyInteger) is
      begin
         case ctxt.state is
            when asn1Sccwait =>
               ctxt.pongarg := pongArg;
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end Pong;
      

   procedure Tick is
      begin
         case ctxt.state is
            when asn1Sccwait =>
               Execute_Transition (2);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end Tick;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      tmp3 : asn1SccMyInteger;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE Wait (11,18) at 320, 60
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when 1 =>
                  --  writeln(pongArg) (17,17)
                  Put (asn1SccMyInteger'Image (ctxt.pongArg));
                  New_Line;
                  --  NEXT_STATE Wait (19,22) at 620, 178
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when 2 =>
                  --  Ping(0) (26,19)
                  tmp3 := asn1SccMyInteger (0);
                  RI_0_Ping(tmp3);
                  --  NEXT_STATE Wait (28,22) at 450, 175
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