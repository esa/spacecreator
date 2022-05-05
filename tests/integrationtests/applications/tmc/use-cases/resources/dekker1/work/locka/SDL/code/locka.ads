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


with Locka_RI;
with Lock;
package Locka with Elaborate_Body is
   procedure Startup with Export, Convention => C, Link_Name => "Locka_startup";
   --  Provided interface "RemoteSetsTurn"
   procedure RemoteSetsTurn(p1: in out asn1SccT_ProcID);
   pragma Export(C, RemoteSetsTurn, "locka_PI_RemoteSetsTurn");
   --  Provided interface "RemoteWantsToEnter"
   procedure RemoteWantsToEnter(p1: in out asn1SccT_Boolean);
   pragma Export(C, RemoteWantsToEnter, "locka_PI_RemoteWantsToEnter");
   --  Provided interface "WriteRequest"
   procedure WriteRequest(p1: in out asn1SccT_WriteRequest);
   pragma Export(C, WriteRequest, "locka_PI_WriteRequest");
   --  Required interface "MemWrite"
   --  Required interface "SetTurn"
   --  Required interface "WantToEnter"
   --  Paramless required interface "WriteAck"
   package Locka_Instance is new Lock (RI_0_MemWrite => Locka_RI.MemWrite, RI_0_SetTurn => Locka_RI.SetTurn, RI_0_WantToEnter => Locka_RI.WantToEnter, RI_0_WriteAck => Locka_RI.WriteAck, Check_Queue => Locka_RI.Check_Queue);
   function Get_State return chars_ptr is (Locka_RI.To_C_Pointer (Locka_Instance.ctxt.State'Img)) with Export, Convention => C, Link_Name => "locka_state";
   procedure Execute_Transition (Id : Integer) renames Locka_Instance.Execute_Transition;
   CS_Only : constant := Locka_Instance.CS_Only;
end Locka;