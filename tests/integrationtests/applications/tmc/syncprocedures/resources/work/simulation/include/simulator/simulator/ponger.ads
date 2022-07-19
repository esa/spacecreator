-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with SYNCPROCEDURES_DATAVIEW;
use SYNCPROCEDURES_DATAVIEW;
with TASTE_BasicTypes;
use TASTE_BasicTypes;
with adaasn1rtl;
use adaasn1rtl;
with Ponger_Datamodel; use Ponger_Datamodel;

with Ponger_RI;
package Ponger with Elaborate_Body is
   ctxt : aliased asn1SccPonger_Context :=
      (Init_Done => False,
       others => <>);
   function Get_State return Chars_Ptr is (Ponger_RI.To_C_Pointer (asn1SccPonger_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "ponger_state";
   procedure Startup with Export, Convention => C, Link_Name => "Ponger_startup";
   --  Provided interface "Ping"
   procedure Ping(pingArg: in out asn1SccMyInteger);
   pragma Export(C, Ping, "ponger_PI_Ping");
   --  Required interface "Pong"
   procedure RI_0_Pong (pongArg : in out asn1SccMyInteger) renames Ponger_RI.Pong;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 2;
end Ponger;