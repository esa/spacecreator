-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Lock is
   procedure RemoteSetsTurn(p1: in out asn1SccT_ProcID) is
      begin
         case ctxt.state is
            when asn1Sccwait_turn =>
               ctxt.turn := p1;
               Execute_Transition (6);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end RemoteSetsTurn;
      

   procedure RemoteWantsToEnter(p1: in out asn1SccT_Boolean) is
      begin
         case ctxt.state is
            when asn1Sccwait_nocontention =>
               ctxt.remotewantstoenter := p1;
               Execute_Transition (4);
            when asn1Sccwriting =>
               ctxt.remotewantstoenter := p1;
               Execute_Transition (4);
            when asn1Sccidle =>
               ctxt.remotewantstoenter := p1;
               Execute_Transition (4);
            when asn1Sccwait_turn =>
               ctxt.remotewantstoenter := p1;
               Execute_Transition (4);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end RemoteWantsToEnter;
      

   procedure WriteRequest(p1: in out asn1SccT_WriteRequest) is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               ctxt.writerequest := p1;
               Execute_Transition (3);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end WriteRequest;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      Message_Pending : Asn1Boolean := True;
      tmp72 : asn1SccT_Boolean;
      --  !! stack: _call_external_function line 1440
      tmp67 : asn1SccT_Boolean;
      tmp39 : asn1SccT_Boolean;
      tmp79 : asn1SccT_Boolean;
      tmp53 : asn1SccT_Boolean;
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  remoteWantsToEnter := false (21,13)
                  ctxt.remoteWantsToEnter := false;
                  --  turn := 0 (22,0)
                  ctxt.turn := 0;
                  --  turnError := false (23,0)
                  ctxt.turnError := false;
                  --  remoteProcID := 1 - myProcID (25,13)
                  ctxt.remoteProcID := (1 - ctxt.myProcID);
                  --  NEXT_STATE IDLE (27,18) at 324, 224
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 1 =>
                  --  myWriteValue.write_address := 
                  --  writeRequest.start_address + writeCount (33,17)
                  ctxt.myWriteValue.write_address := (ctxt.writeRequest.start_address + ctxt.writeCount);
                  --  myWriteValue.write_value := writeRequest.values(writeCount) (36,17)
                  ctxt.myWriteValue.write_value := ctxt.writeRequest.values.Data(1 + Integer (ctxt.writeCount));
                  --  MemWrite(myWriteValue) (38,19)
                  RI_0_MemWrite(ctxt.myWriteValue);
                  --  writeCount := (writeCount + 1) (40,17)
                  ctxt.writeCount := (ctxt.writeCount + 1);
                  --  NEXT_STATE WRITING (42,22) at 1177, 1054
                  trId := -1;
                  ctxt.State := asn1SccWRITING;
                  goto Continuous_Signals;
               when 2 =>
                  --  turn := remoteProcID (46,17)
                  ctxt.turn := ctxt.remoteProcID;
                  --  SetTurn(turn) (48,19)
                  RI_0_SetTurn(ctxt.turn);
                  --  WantToEnter(false) (50,19)
                  tmp39 := false;
                  RI_0_WantToEnter(tmp39);
                  --  WriteAck (52,19)
                  RI_0_WriteAck;
                  --  NEXT_STATE IDLE (54,22) at 841, 1044
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 3 =>
                  --  writeln('proc ', myProcID, ' write request') (61,17)
                  Put ("proc ");
                  Put (asn1SccT_ProcID'Image (ctxt.myProcID));
                  Put (" write request");
                  New_Line;
                  --  writeCount := 0 (63,17)
                  ctxt.writeCount := 0;
                  --  writeSize := length(writeRequest.values) (65,17)
                  ctxt.writeSize := Asn1UInt (ctxt.writeRequest.values.Length);
                  --  WantToEnter(true) (67,19)
                  tmp53 := true;
                  RI_0_WantToEnter(tmp53);
                  --  DECISION remoteWantsToEnter (-1,-1)
                  --  ANSWER true (71,17)
                  if (ctxt.remoteWantsToEnter) = true then
                     --  NEXT_STATE WAIT_NOCONTENTION (73,30) at 358, 718
                     trId := -1;
                     ctxt.State := asn1SccWAIT_NOCONTENTION;
                     goto Continuous_Signals;
                     --  ANSWER false (75,17)
                  elsif (ctxt.remoteWantsToEnter) = false then
                     --  NEXT_STATE WRITING (77,30) at 589, 718
                     trId := -1;
                     ctxt.State := asn1SccWRITING;
                     goto Continuous_Signals;
                  end if;
               when 4 =>
                  --  NEXT_STATE - (85,22) at 989, 174
                  trId := -1;
                  goto Continuous_Signals;
               when 5 =>
                  --  DECISION turn = myProcID (92,26)
                  --  ANSWER true (94,17)
                  if ((ctxt.turn = ctxt.myProcID)) then
                     --  WantToEnter(true) (96,27)
                     tmp67 := true;
                     RI_0_WantToEnter(tmp67);
                     --  writeCount := 0 (98,25)
                     ctxt.writeCount := 0;
                     --  NEXT_STATE WRITING (100,30) at 106, 1177
                     trId := -1;
                     ctxt.State := asn1SccWRITING;
                     goto Continuous_Signals;
                     --  ANSWER false (102,17)
                  else
                     --  WantToEnter(false) (104,27)
                     tmp72 := false;
                     RI_0_WantToEnter(tmp72);
                     --  NEXT_STATE WAIT_TURN (106,30) at 481, 1120
                     trId := -1;
                     ctxt.State := asn1SccWAIT_TURN;
                     goto Continuous_Signals;
                  end if;
               when 6 =>
                  --  DECISION turn = myProcID (114,26)
                  --  ANSWER true (116,17)
                  if ((ctxt.turn = ctxt.myProcID)) then
                     --  WantToEnter(true) (118,27)
                     tmp79 := true;
                     RI_0_WantToEnter(tmp79);
                     --  writeCount := 0 (120,25)
                     ctxt.writeCount := 0;
                     --  NEXT_STATE WRITING (122,30) at 851, 617
                     trId := -1;
                     ctxt.State := asn1SccWRITING;
                     goto Continuous_Signals;
                     --  ANSWER false (124,17)
                  else
                     --  COMMENT supposedly
                     --  unreachable (128,20)
                     --  turnError := true (126,25)
                     ctxt.turnError := true;
                     --  NEXT_STATE WAIT_TURN (131,30) at 1185, 566
                     trId := -1;
                     ctxt.State := asn1SccWAIT_TURN;
                     goto Continuous_Signals;
                  end if;
               when CS_Only =>
                  trId := -1;
                  goto Continuous_Signals;
               when others =>
                  null;
            end case;
            <<Continuous_Signals>>
            --  Process continuous signals
            if ctxt.Init_Done then
               Check_Queue (Message_Pending);
            end if;
            if Message_Pending or trId /= -1 then
               goto Next_Transition;
            end if;
            if ctxt.State = asn1Sccwait_nocontention then
               --  Priority: 1
               --  DECISION remoteWantsToEnter = false (90,36)
               --  ANSWER true (None,None)
               if ((ctxt.remoteWantsToEnter = false)) then
                  trId := 5;
               end if;
            end if;
            if ctxt.State = asn1Sccwriting then
               --  Priority: 1
               --  DECISION writeCount < writeSize (31,28)
               --  ANSWER true (None,None)
               if ((ctxt.writeCount < ctxt.writeSize)) then
                  trId := 1;
                  --  Priority: 1
                  --  DECISION writeCount >= writeSize (44,28)
                  --  ANSWER true (None,None)
               elsif ((ctxt.writeCount >= ctxt.writeSize)) then
                  trId := 2;
               end if;
            end if;
            <<Next_Transition>>
         end loop;
      end Execute_Transition;
      

   procedure Startup is
      begin
         Execute_Transition (0);
         ctxt.Init_Done := True;
      end Startup;
   begin
      Startup;
end Lock;