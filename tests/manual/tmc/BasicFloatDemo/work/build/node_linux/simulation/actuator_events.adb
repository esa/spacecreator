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
               when Controlsignal_PRESENT =>
                  declare
                     Param : asn1SccT_Float := Evt.Msg_In.Controlsignal.X;
                  begin
                     Actuator_PI.Controlsignal (X => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Reporting_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccActuator;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Reporting_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccSystemcontrol then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Reporting_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccSystemcontrol =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Systemcontrol_PRESENT,
                                Systemcontrol =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Reporting_PRESENT,
                                               Reporting =>
                                                  (null record))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Systemcontrol_Queue.Length < asn1SccEvents_Ty_length_index (Systemcontrol_Queue_Size) then
                               Global_State.Systemcontrol_Queue.Length :=
                                  Global_State.Systemcontrol_Queue.Length + 1;
                               Global_State.Systemcontrol_Queue.Data
                                 (Global_State.Systemcontrol_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Systemcontrol is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Reporting");
                      end case;
                      <<Reporting_Next_Dest>>
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
               when Controlsignal_PRESENT =>
                 Put ("Actuator: INPUT Controlsignal");
                  Put_Line (" (" & T_Float_Pkg.Image (Event.Msg_In.Controlsignal.X) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Reporting_PRESENT =>
                 Put ("Actuator: OUTPUT Reporting");
                  Put_Line ("()");
            end case;
      end case;
   end Print_Event;



end Actuator_Events;
