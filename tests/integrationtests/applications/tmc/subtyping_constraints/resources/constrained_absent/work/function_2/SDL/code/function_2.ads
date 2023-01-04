-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with CONSTRAINED_ABSENT_DATAVIEW;
use CONSTRAINED_ABSENT_DATAVIEW;
with TASTE_BasicTypes;
use TASTE_BasicTypes;
with System_Dataview;
use System_Dataview;
with adaasn1rtl;
use adaasn1rtl;
with Function_2_Datamodel; use Function_2_Datamodel;

with Function_2_RI;
package Function_2 with Elaborate_Body is
   ctxt : aliased asn1SccFunction_2_Context :=
      (Init_Done => False,
       others => <>);
   function To_Mychoice_selection (Src : CONSTRAINED_ABSENT_DATAVIEW.asn1SccMychoice_selection) return Function_2_Datamodel.asn1SccFunction_2_Mychoice_selection is (Function_2_Datamodel.asn1SccFunction_2_Mychoice_selection'Enum_Val (Src'Enum_Rep));
   function Get_State return Chars_Ptr is (Function_2_RI.To_C_Pointer (asn1SccFunction_2_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "function_2_state";
   procedure Startup with Export, Convention => C, Link_Name => "Function_2_startup";
   --  Provided interface "PI_1"
   procedure PI_1(p1: in out asn1SccMySeqOf);
   pragma Export(C, PI_1, "function_2_PI_PI_1");
   --  Required interface "PI_2"
   procedure RI_0_PI_2 (p1 : in out asn1SccMyChoice) renames Function_2_RI.PI_2;
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 1;
end Function_2;