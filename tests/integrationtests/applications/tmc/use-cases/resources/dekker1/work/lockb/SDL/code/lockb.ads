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


with Lockb_RI;
with Lock;
package Lockb with Elaborate_Body is
   procedure Startup with Export, Convention => C, Link_Name => "Lockb_startup";
   --  Provided interface "RemoteSetsTurn"
   procedure RemoteSetsTurn(p1: in out asn1SccT_ProcID);
   pragma Export(C, RemoteSetsTurn, "lockb_PI_RemoteSetsTurn");
   --  Provided interface "RemoteWantsToEnter"
   procedure RemoteWantsToEnter(p1: in out asn1SccT_Boolean);
   pragma Export(C, RemoteWantsToEnter, "lockb_PI_RemoteWantsToEnter");
   --  Provided interface "WriteRequest"
   procedure WriteRequest(p1: in out asn1SccT_WriteRequest);
   pragma Export(C, WriteRequest, "lockb_PI_WriteRequest");
   --  Required interface "MemWrite"
   --  Required interface "SetTurn"
   --  Required interface "WantToEnter"
   --  Paramless required interface "WriteAck"
   package Lockb_Instance is new Lock (RI_0_MemWrite => Lockb_RI.MemWrite, RI_0_SetTurn => Lockb_RI.SetTurn, RI_0_WantToEnter => Lockb_RI.WantToEnter, RI_0_WriteAck => Lockb_RI.WriteAck, Check_Queue => Lockb_RI.Check_Queue);
   function Get_State return chars_ptr is (Lockb_RI.To_C_Pointer (Lockb_Instance.ctxt.State'Img)) with Export, Convention => C, Link_Name => "lockb_state";
   procedure Execute_Transition (Id : Integer) renames Lockb_Instance.Execute_Transition;
   CS_Only : constant := Lockb_Instance.CS_Only;
end Lockb;