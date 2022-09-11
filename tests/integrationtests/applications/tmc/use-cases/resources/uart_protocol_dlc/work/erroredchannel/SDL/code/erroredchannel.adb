-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Erroredchannel is
   procedure CorruptChar(p1: in out asn1SccUINT8) is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               ctxt.corruption := p1;
               Execute_Transition (3);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end CorruptChar;
      

   procedure DropChar is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               Execute_Transition (4);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end DropChar;
      

   procedure PHYTxChar(p1: in out asn1SccUINT8) is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               ctxt.txchar := p1;
               Execute_Transition (2);
            when asn1Sccdrop =>
               ctxt.txchar := p1;
               Execute_Transition (1);
            when asn1Scccorrupt =>
               ctxt.txchar := p1;
               Execute_Transition (5);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end PHYTxChar;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE IDLE (12,18) at 51, 105
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 1 =>
                  --  NEXT_STATE IDLE (18,22) at 973, 211
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 2 =>
                  --  PHYRxChar(txChar) (25,19)
                  RI_0_PHYRxChar(ctxt.txChar);
                  --  NEXT_STATE IDLE (27,22) at 286, 245
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 3 =>
                  --  NEXT_STATE CORRUPT (31,22) at 638, 190
                  trId := -1;
                  ctxt.State := asn1SccCORRUPT;
                  goto Continuous_Signals;
               when 4 =>
                  --  NEXT_STATE DROP (35,22) at 466, 192
                  trId := -1;
                  ctxt.State := asn1SccDROP;
                  goto Continuous_Signals;
               when 5 =>
                  --  txChar := txChar xor corruption (42,17)
                  ctxt.txChar := (ctxt.txChar xor ctxt.corruption);
                  --  PHYRxChar(txChar) (44,19)
                  RI_0_PHYRxChar(ctxt.txChar);
                  --  NEXT_STATE IDLE (46,22) at 876, 534
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
end Erroredchannel;