-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with TASTE_BasicTypes;
use TASTE_BasicTypes;
with USERDEFINED_TYPES_DATAVIEW;
use USERDEFINED_TYPES_DATAVIEW;
with adaasn1rtl;
use adaasn1rtl;
with Receiver_Datamodel; use Receiver_Datamodel;

with Receiver_RI;
package Receiver with Elaborate_Body is
   ctxt : aliased asn1SccReceiver_Context :=
      (Init_Done => False,
       integervalue => 2,
      integerhandled => false,
      others => <>);
   function Get_State return Chars_Ptr is (Receiver_RI.To_C_Pointer (asn1SccReceiver_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "receiver_state";
   procedure Startup with Export, Convention => C, Link_Name => "Receiver_startup";
   --  Provided interface "IntegerInterface"
   procedure IntegerInterface(integerArg: in out asn1SccMyBetterInteger);
   pragma Export(C, IntegerInterface, "receiver_PI_IntegerInterface");
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 2;
end Receiver;