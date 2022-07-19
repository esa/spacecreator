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
with Pinger_Datamodel; use Pinger_Datamodel;

with Pinger_RI;
package Pinger with Elaborate_Body is
   ctxt : aliased asn1SccPinger_Context :=
      (Init_Done => False,
       others => <>);
   function Get_State return Chars_Ptr is (Pinger_RI.To_C_Pointer (asn1SccPinger_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "pinger_state";
   procedure Startup with Export, Convention => C, Link_Name => "Pinger_startup";
   --  Provided interface "Pong"
   procedure Pong(pongArg: in out asn1SccMyInteger);
   pragma Export(C, Pong, "pinger_PI_Pong");
   --  Provided interface "Tick"
   procedure Tick;
   pragma Export(C, Tick, "pinger_PI_Tick");
   --  Required interface "Ping"
   procedure RI_0_Ping (pingArg : in out asn1SccMyInteger) renames Pinger_RI.Ping;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 3;
end Pinger;