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
with Lock_Datamodel; use Lock_Datamodel;




generic
   with procedure Check_Queue (Res : out Asn1Boolean);
   with procedure RI_0_MemWrite(p1: in out asn1SccT_WriteAccess);
   with procedure RI_0_SetTurn(p1: in out asn1SccT_ProcID);
   with procedure RI_0_WantToEnter(p1: in out asn1SccT_Boolean);
   with procedure RI_0_WriteAck;
package Lock with Elaborate_Body is
   ctxt : aliased asn1SccLock_Context :=
      (Init_Done => False,
       others => <>);
   procedure Startup;
   --  Provided interface "RemoteSetsTurn"
   procedure RemoteSetsTurn(p1: in out asn1SccT_ProcID);
   --  Provided interface "RemoteWantsToEnter"
   procedure RemoteWantsToEnter(p1: in out asn1SccT_Boolean);
   --  Provided interface "WriteRequest"
   procedure WriteRequest(p1: in out asn1SccT_WriteRequest);
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 7;
end Lock;