-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with Basic_Types;
use Basic_Types;
with Bilevel_Types;
use Bilevel_Types;
with UART_DLC;
use UART_DLC;
with TASTE_BasicTypes;
use TASTE_BasicTypes;
with UART_PROTOCOL_DLC_DATAVIEW;
use UART_PROTOCOL_DLC_DATAVIEW;
with System_Dataview;
use System_Dataview;
with adaasn1rtl;
use adaasn1rtl;
with Erroredchannel_Datamodel; use Erroredchannel_Datamodel;

with Erroredchannel_RI;
package Erroredchannel with Elaborate_Body is
   ctxt : aliased asn1SccErroredchannel_Context :=
      (Init_Done => False,
       others => <>);
   function Get_State return Chars_Ptr is (Erroredchannel_RI.To_C_Pointer (asn1SccErroredchannel_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "erroredchannel_state";
   procedure Startup with Export, Convention => C, Link_Name => "Erroredchannel_startup";
   --  Provided interface "CorruptChar"
   procedure CorruptChar(p1: in out asn1SccUINT8);
   pragma Export(C, CorruptChar, "erroredchannel_PI_CorruptChar");
   --  Provided interface "DropChar"
   procedure DropChar;
   pragma Export(C, DropChar, "erroredchannel_PI_DropChar");
   --  Provided interface "PHYTxChar"
   procedure PHYTxChar(p1: in out asn1SccUINT8);
   pragma Export(C, PHYTxChar, "erroredchannel_PI_PHYTxChar");
   --  Required interface "PHYRxChar"
   procedure RI_0_PHYRxChar (p1 : in out asn1SccUINT8) renames Erroredchannel_RI.PHYRxChar;
   --  Paramless required interface "PHYRxError"
   procedure RI_0_PHYRxError renames Erroredchannel_RI.PHYRxError;
   --  Paramless required interface "PHYTxError"
   procedure RI_0_PHYTxError renames Erroredchannel_RI.PHYTxError;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 6;
end Erroredchannel;