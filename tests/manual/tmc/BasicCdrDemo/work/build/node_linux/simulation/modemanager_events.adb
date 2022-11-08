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
               when Feedback_PRESENT =>
                  declare
                     Param : asn1SccTinteger := Evt.Msg_In.Feedback.X;
                  begin
                     Modemanager_PI.Feedback (X => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
               when Reading_PRESENT =>
                  declare
                     Param : asn1SccTinteger := Evt.Msg_In.Reading.X;
                  begin
                     Modemanager_PI.Reading (X => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
               when Tc_PRESENT =>
                  declare
                     Param : asn1SccTconfig := Evt.Msg_In.Tc.Cfg;
                  begin
                     Modemanager_PI.Tc (Cfg => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Ctrl_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccModemanager;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Ctrl_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccActuator then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Ctrl_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccActuator =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Actuator_PRESENT,
                                Actuator =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Ctrl_PRESENT,
                                               Ctrl =>
                                                  (x =>  Event.Output_Event.Event.Modemanager.Msg_Out.Ctrl.x))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Actuator_Queue.Length < asn1SccEvents_Ty_length_index (Actuator_Queue_Size) then
                               Global_State.Actuator_Queue.Length :=
                                  Global_State.Actuator_Queue.Length + 1;
                               Global_State.Actuator_Queue.Data
                                 (Global_State.Actuator_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Actuator is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Ctrl");
                      end case;
                      <<Ctrl_Next_Dest>>
                   end loop;
                when Tm_PRESENT =>
                   null;
                   --  Message is sent to the environment
                   --  (the model checker sends it to the observers before it is dropped here)
                   --  Put_Line ("[-] Message to environment: Tm");
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
               when Feedback_PRESENT =>
                 Put ("ModeManager: INPUT Feedback");
                  Put_Line (" (" & Tinteger_Pkg.Image (Event.Msg_In.Feedback.X) & ")");
               when Reading_PRESENT =>
                 Put ("ModeManager: INPUT Reading");
                  Put_Line (" (" & Tinteger_Pkg.Image (Event.Msg_In.Reading.X) & ")");
               when Tc_PRESENT =>
                 Put ("ModeManager: INPUT Tc");
                  Put_Line (" (" & Tconfig_Pkg.Image (Event.Msg_In.Tc.Cfg) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Ctrl_PRESENT =>
                 Put ("ModeManager: OUTPUT Ctrl");
                  Put_Line (" (" & Tinteger_Pkg.Image (Event.Msg_Out.Ctrl.X) & ")");
                when Tm_PRESENT =>
                 Put ("ModeManager: OUTPUT Tm");
                  Put_Line (" (" & Treport_Pkg.Image (Event.Msg_Out.Tm.Reprt) & ")");
            end case;
      end case;
   end Print_Event;



end Modemanager_Events;
