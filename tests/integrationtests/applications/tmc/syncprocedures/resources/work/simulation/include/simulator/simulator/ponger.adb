-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Ponger is
   procedure Ping(pingArg: in out asn1SccMyInteger) is
      begin
         case ctxt.state is
            when asn1Sccwait =>
               ctxt.pingarg := pingArg;
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end Ping;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE Wait (11,18) at 320, 60
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when 1 =>
                  --  writeln(pingArg) (17,17)
                  Put (asn1SccMyInteger'Image (ctxt.pingArg));
                  New_Line;
                  --  Pong(pingArg) (19,19)
                  RI_0_Pong(ctxt.pingArg);
                  --  NEXT_STATE Wait (21,22) at 450, 230
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
end Ponger;