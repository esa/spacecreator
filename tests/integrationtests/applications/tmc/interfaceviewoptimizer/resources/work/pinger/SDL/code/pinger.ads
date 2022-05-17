-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with ENVIRONMENT_SELECTOR_DATAVIEW;
use ENVIRONMENT_SELECTOR_DATAVIEW;
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
   --  Provided interface "ClockInterface"
   procedure ClockInterface;
   pragma Export(C, ClockInterface, "pinger_PI_ClockInterface");
   --  Required interface "PingInterface"
   procedure RI_0_PingInterface (arg : in out asn1SccMyInteger) renames Pinger_RI.PingInterface;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 2;
end Pinger;