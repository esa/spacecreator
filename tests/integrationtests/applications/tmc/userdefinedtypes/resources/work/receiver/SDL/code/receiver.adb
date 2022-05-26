-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Receiver is
   procedure IntegerInterface(integerArg: in out asn1SccMyBetterInteger) is
      begin
         case ctxt.state is
            when asn1Sccwait =>
               ctxt.integerarg := integerArg;
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end IntegerInterface;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE Wait (13,18) at 437, 67
                  trId := -1;
                  ctxt.State := asn1SccWait;
                  goto Continuous_Signals;
               when 1 =>
                  --  integerValue := integerArg (19,17)
                  ctxt.integerValue := ctxt.integerArg;
                  --  integerHandled := true (21,17)
                  ctxt.integerHandled := true;
                  --  NEXT_STATE Wait (23,22) at 683, 242
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
end Receiver;