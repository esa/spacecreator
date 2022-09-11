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
with rtdlcr_Datamodel; use rtdlcr_Datamodel;

with rtdlcr_RI;
package rtdlcr with Elaborate_Body is
   ctxt : aliased asn1SccRtdlcr_Context :=
      (Init_Done => False,
       frame_timeout => 1000,
      others => <>);
   function Get_State return Chars_Ptr is (Rtdlcr_RI.To_C_Pointer (asn1Sccrtdlcr_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "rtdlcr_state";
   procedure Startup with Export, Convention => C, Link_Name => "rtdlcr_startup";
   --  Provided interface "DLCRxReset"
   procedure DLCRxReset;
   pragma Export(C, DLCRxReset, "rtdlcr_PI_DLCRxReset");
   --  Provided interface "PHYRxChar"
   procedure PHYRxChar(p1: in out asn1SccUINT8);
   pragma Export(C, PHYRxChar, "rtdlcr_PI_PHYRxChar");
   --  Provided interface "PHYRxError"
   procedure PHYRxError;
   pragma Export(C, PHYRxError, "rtdlcr_PI_PHYRxError");
   --  Provided interface "FrameTimer"
   procedure FrameTimer;
   pragma Export(C, FrameTimer, "rtdlcr_PI_FrameTimer");
   --  Required interface "DLCRxError"
   procedure RI_0_DLCRxError (p1 : in out asn1SccT_UART_DLC_RxErrorCode) renames rtdlcr_RI.DLCRxError;
   --  Required interface "DLCRxSDU"
   procedure RI_0_DLCRxSDU (p1 : in out asn1SccT_UART_DLC_SDU) renames rtdlcr_RI.DLCRxSDU;
   --  Timer FrameTimer SET and RESET functions
   procedure SET_FrameTimer (Val : in out asn1SccT_UInt32) renames rtdlcr_RI.Set_FrameTimer;
   procedure RESET_FrameTimer renames rtdlcr_RI.Reset_FrameTimer;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 5;
end rtdlcr;