with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Actuator_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccActuator_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Actuator,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Actuator,
                 when others => asn1sccActuator_Event_Init);
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
                  Actuator_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Ctrl_PRESENT =>
                  declare
                     Param : asn1SccTinteger := Evt.Msg_In.Ctrl.X;
                  begin
                     Actuator_PI.Ctrl (X => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Feedback_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccActuator;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Feedback_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccModemanager then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Feedback_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccModemanager =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Modemanager_PRESENT,
                                Modemanager =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Feedback_PRESENT,
                                               Feedback =>
                                                  (x =>  Event.Output_Event.Event.Actuator.Msg_Out.Feedback.x))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Modemanager_Queue.Length < asn1SccEvents_Ty_length_index (Modemanager_Queue_Size) then
                               Global_State.Modemanager_Queue.Length :=
                                  Global_State.Modemanager_Queue.Length + 1;
                               Global_State.Modemanager_Queue.Data
                                 (Global_State.Modemanager_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Modemanager is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Feedback");
                      end case;
                      <<Feedback_Next_Dest>>
                   end loop;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccActuator_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Actuator: continuous signals");
               when Ctrl_PRESENT =>
                 Put ("Actuator: INPUT Ctrl");
                  Put_Line (" (" & Tinteger_Pkg.Image (Event.Msg_In.Ctrl.X) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Feedback_PRESENT =>
                 Put ("Actuator: OUTPUT Feedback");
                  Put_Line (" (" & Tinteger_Pkg.Image (Event.Msg_Out.Feedback.X) & ")");
            end case;
      end case;
   end Print_Event;



end Actuator_Events;
