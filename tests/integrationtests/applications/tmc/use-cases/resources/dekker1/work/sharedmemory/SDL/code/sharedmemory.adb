-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Sharedmemory is
   procedure MemWriteA(p1: in out asn1SccT_WriteAccess) is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               ctxt.writeaccess := p1;
               Execute_Transition (2);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end MemWriteA;
      

   procedure MemWriteB(p1: in out asn1SccT_WriteAccess) is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               ctxt.writeaccess := p1;
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end MemWriteB;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE IDLE (12,18) at 417, 144
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 1 =>
                  --  writeln('WriteB addr ', 
                  --  writeAccess.write_address,
                  --  ' val ',
                  --  writeAccess.write_value) (18,17)
                  Put ("WriteB addr ");
                  Put (asn1SccT_UInt16'Image (ctxt.writeAccess.write_address));
                  Put (" val ");
                  Put (asn1SccT_UInt8'Image (ctxt.writeAccess.write_value));
                  New_Line;
                  --  memBuffer(writeAccess.write_address) :=
                  --  writeAccess.write_value (23,17)
                  ctxt.memBuffer.Data(1 + Integer (ctxt.writeAccess.write_address)) := ctxt.writeAccess.write_value;
                  --  NEXT_STATE IDLE (26,22) at 849, 516
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 2 =>
                  --  writeln('WriteA addr ', 
                  --  writeAccess.write_address,
                  --  ' val ',
                  --  writeAccess.write_value) (30,17)
                  Put ("WriteA addr ");
                  Put (asn1SccT_UInt16'Image (ctxt.writeAccess.write_address));
                  Put (" val ");
                  Put (asn1SccT_UInt8'Image (ctxt.writeAccess.write_value));
                  New_Line;
                  --  memBuffer(writeAccess.write_address) :=
                  --  writeAccess.write_value (35,17)
                  ctxt.memBuffer.Data(1 + Integer (ctxt.writeAccess.write_address)) := ctxt.writeAccess.write_value;
                  --  NEXT_STATE IDLE (38,22) at 567, 516
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
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
end Sharedmemory;