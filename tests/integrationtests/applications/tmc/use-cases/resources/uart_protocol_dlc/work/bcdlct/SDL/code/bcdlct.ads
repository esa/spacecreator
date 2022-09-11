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
with Bcdlct_Datamodel; use Bcdlct_Datamodel;

with Bcdlct_RI;
package Bcdlct with Elaborate_Body is
   ctxt : aliased asn1SccBcdlct_Context :=
      (Init_Done => False,
       others => <>);
   function Get_State return Chars_Ptr is (Bcdlct_RI.To_C_Pointer (asn1SccBcdlct_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "bcdlct_state";
   procedure Startup with Export, Convention => C, Link_Name => "Bcdlct_startup";
   --  Provided interface "DLCTxReset"
   procedure DLCTxReset;
   pragma Export(C, DLCTxReset, "bcdlct_PI_DLCTxReset");
   --  Provided interface "DLCTxSDU"
   procedure DLCTxSDU(p1: in out asn1SccT_UART_DLC_SDU);
   pragma Export(C, DLCTxSDU, "bcdlct_PI_DLCTxSDU");
   --  Provided interface "PHYTxError"
   procedure PHYTxError;
   pragma Export(C, PHYTxError, "bcdlct_PI_PHYTxError");
   --  Required interface "PHYTxChar"
   procedure RI_0_PHYTxChar (p1 : in out asn1SccUINT8) renames Bcdlct_RI.PHYTxChar;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 1;
end Bcdlct;