with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Modemanager_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccModemanager_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Modemanager,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Modemanager,
                 when others => asn1sccModemanager_Event_Init);
      --  To transform outputs into a corresponding input message to place
      --  in the recipient queue (with a possibly different name):
      Input_Event : asn1SccObservable_Event (Kind => Input_Event_PRESENT);
   begin
      -- This function executes provided interface events only
      if Event.Kind in No_Event_Present .. System_Startup_Present then
         return;
      end if;
      case Evt.Kind is
         when Msg_IN_Present =>
            case Evt.Msg_In.Kind is
               when Input_None_PRESENT =>
                  CS_Barrier := False;   -- Force Check_Queue to pass
                  Modemanager_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Actuatortm1_PRESENT =>
                  declare
                     Param : asn1SccActuatorstatus := Evt.Msg_In.Actuatortm1.Tm;
                  begin
                     Modemanager_PI.Actuatortm1 (Tm => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
               when Actuatortm2_PRESENT =>
                  declare
                     Param : asn1SccActuatorstatus := Evt.Msg_In.Actuatortm2.Tm;
                  begin
                     Modemanager_PI.Actuatortm2 (Tm => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
               when Reading_PRESENT =>
                  declare
                     Param : asn1SccDataitem := Evt.Msg_In.Reading.X;
                  begin
                     Modemanager_PI.Reading (X => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
               when Systemtc_PRESENT =>
                  declare
                     Param : asn1SccSystemcommand := Evt.Msg_In.Systemtc.Tc;
                  begin
                     Modemanager_PI.Systemtc (Tc => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Actuatortc1_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccModemanager;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Actuatortc1_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccActuator1 then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Actuatortc1_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccActuator1 =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Actuator1_PRESENT,
                                Actuator1 =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Actuatortc_PRESENT,
                                               Actuatortc =>
                                                  (tc =>  Event.Output_Event.Event.Modemanager.Msg_Out.Actuatortc1.tc))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Actuator1_Queue.Length < asn1SccEvents_Ty_length_index (Actuator1_Queue_Size) then
                               Global_State.Actuator1_Queue.Length :=
                                  Global_State.Actuator1_Queue.Length + 1;
                               Global_State.Actuator1_Queue.Data
                                 (Global_State.Actuator1_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Actuator1 is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Actuatortc1");
                      end case;
                      <<Actuatortc1_Next_Dest>>
                   end loop;
                when Actuatortc2_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccModemanager;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Actuatortc2_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccActuator2 then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Actuatortc2_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccActuator2 =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Actuator2_PRESENT,
                                Actuator2 =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Actuatortc_PRESENT,
                                               Actuatortc =>
                                                  (tc =>  Event.Output_Event.Event.Modemanager.Msg_Out.Actuatortc2.tc))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Actuator2_Queue.Length < asn1SccEvents_Ty_length_index (Actuator2_Queue_Size) then
                               Global_State.Actuator2_Queue.Length :=
                                  Global_State.Actuator2_Queue.Length + 1;
                               Global_State.Actuator2_Queue.Data
                                 (Global_State.Actuator2_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Actuator2 is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Actuatortc2");
                      end case;
                      <<Actuatortc2_Next_Dest>>
                   end loop;
                when Systemtm_PRESENT =>
                   null;
                   --  Message is sent to the environment
                   --  (the model checker sends it to the observers before it is dropped here)
                   --  Put_Line ("[-] Message to environment: Systemtm");
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccModemanager_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("ModeManager: continuous signals");
               when Actuatortm1_PRESENT =>
                 Put ("ModeManager: INPUT Actuatortm1");
                  Put_Line (" (" & Actuatorstatus_Pkg.Image (Event.Msg_In.Actuatortm1.Tm) & ")");
               when Actuatortm2_PRESENT =>
                 Put ("ModeManager: INPUT Actuatortm2");
                  Put_Line (" (" & Actuatorstatus_Pkg.Image (Event.Msg_In.Actuatortm2.Tm) & ")");
               when Reading_PRESENT =>
                 Put ("ModeManager: INPUT Reading");
                  Put_Line (" (" & Dataitem_Pkg.Image (Event.Msg_In.Reading.X) & ")");
               when Systemtc_PRESENT =>
                 Put ("ModeManager: INPUT Systemtc");
                  Put_Line (" (" & Systemcommand_Pkg.Image (Event.Msg_In.Systemtc.Tc) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Actuatortc1_PRESENT =>
                 Put ("ModeManager: OUTPUT Actuatortc1");
                  Put_Line (" (" & Actuatorcommand_Pkg.Image (Event.Msg_Out.Actuatortc1.Tc) & ")");
                when Actuatortc2_PRESENT =>
                 Put ("ModeManager: OUTPUT Actuatortc2");
                  Put_Line (" (" & Actuatorcommand_Pkg.Image (Event.Msg_Out.Actuatortc2.Tc) & ")");
                when Systemtm_PRESENT =>
                 Put ("ModeManager: OUTPUT Systemtm");
                  Put_Line (" (" & Systemtelemetry_Pkg.Image (Event.Msg_Out.Systemtm.Tm) & ")");
            end case;
      end case;
   end Print_Event;



end Modemanager_Events;
