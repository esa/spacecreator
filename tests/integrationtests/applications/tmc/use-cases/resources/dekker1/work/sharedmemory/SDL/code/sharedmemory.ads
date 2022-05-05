-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with DEKKER1_DATAVIEW;
use DEKKER1_DATAVIEW;
with TASTE_BasicTypes;
use TASTE_BasicTypes;
with adaasn1rtl;
use adaasn1rtl;
with Sharedmemory_Datamodel; use Sharedmemory_Datamodel;

with Sharedmemory_RI;
package Sharedmemory with Elaborate_Body is
   ctxt : aliased asn1SccSharedmemory_Context :=
      (Init_Done => False,
       others => <>);
   function Get_State return Chars_Ptr is (Sharedmemory_RI.To_C_Pointer (asn1SccSharedmemory_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "sharedmemory_state";
   procedure Startup with Export, Convention => C, Link_Name => "Sharedmemory_startup";
   --  Provided interface "MemWriteA"
   procedure MemWriteA(p1: in out asn1SccT_WriteAccess);
   pragma Export(C, MemWriteA, "sharedmemory_PI_MemWriteA");
   --  Provided interface "MemWriteB"
   procedure MemWriteB(p1: in out asn1SccT_WriteAccess);
   pragma Export(C, MemWriteB, "sharedmemory_PI_MemWriteB");
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 3;
end Sharedmemory;