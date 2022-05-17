-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Ponger is
   procedure PingInterface(arg: in out asn1SccMyInteger) is
      begin
         case ctxt.state is
            when asn1Sccwait =>
               ctxt.arg := arg;
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end PingInterface;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE Wait (12,18) at 320, 60
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when 1 =>
                  --  writeln(arg) (18,17)
                  Put (asn1SccMyInteger'Image (ctxt.arg));
                  New_Line;
                  --  NEXT_STATE Wait (20,22) at 449, 170
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