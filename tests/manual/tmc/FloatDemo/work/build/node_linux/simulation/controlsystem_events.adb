with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Controlsystem_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccControlsystem_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Controlsystem,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Controlsystem,
                 when others => asn1sccControlsystem_Event_Init);
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
                  Controlsystem_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Configuration_PRESENT =>
                  declare
                     Param : asn1SccPidconfig := Evt.Msg_In.Configuration.Cfg;
                  begin
                     Controlsystem_PI.Configuration (Cfg => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
               when Reading_PRESENT =>
                  declare
                     Param : asn1SccNumber := Evt.Msg_In.Reading.Signalvalue;
                  begin
                     Controlsystem_PI.Reading (Signalvalue => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Controlsignal_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccControlsystem;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Controlsignal_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccActuator then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Controlsignal_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccActuator =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Actuator_PRESENT,
                                Actuator =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Controlsignal_PRESENT,
                                               Controlsignal =>
                                                  (controlValue =>  Event.Output_Event.Event.Controlsystem.Msg_Out.Controlsignal.controlValue))));
                
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
                             Put_Line ("[X] Invalid destination for message Controlsignal");
                      end case;
                      <<Controlsignal_Next_Dest>>
                   end loop;
                when Tm_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccControlsystem;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Tm_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccHost then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Tm_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccHost =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Host_PRESENT,
                                Host =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Tm_PRESENT,
                                               Tm =>
                                                  (null record))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Host_Queue.Length < asn1SccEvents_Ty_length_index (Host_Queue_Size) then
                               Global_State.Host_Queue.Length :=
                                  Global_State.Host_Queue.Length + 1;
                               Global_State.Host_Queue.Data
                                 (Global_State.Host_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Host is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Tm");
                      end case;
                      <<Tm_Next_Dest>>
                   end loop;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccControlsystem_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("ControlSystem: continuous signals");
               when Configuration_PRESENT =>
                 Put ("ControlSystem: INPUT Configuration");
                  Put_Line (" (" & Pidconfig_Pkg.Image (Event.Msg_In.Configuration.Cfg) & ")");
               when Reading_PRESENT =>
                 Put ("ControlSystem: INPUT Reading");
                  Put_Line (" (" & Number_Pkg.Image (Event.Msg_In.Reading.Signalvalue) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Controlsignal_PRESENT =>
                 Put ("ControlSystem: OUTPUT Controlsignal");
                  Put_Line (" (" & Number_Pkg.Image (Event.Msg_Out.Controlsignal.Controlvalue) & ")");
                when Tm_PRESENT =>
                 Put ("ControlSystem: OUTPUT Tm");
                  Put_Line ("()");
            end case;
      end case;
   end Print_Event;



end Controlsystem_Events;
