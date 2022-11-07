with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Sensor_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccSensor_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Sensor,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Sensor,
                 when others => asn1sccSensor_Event_Init);
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
                  Sensor_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Tc_PRESENT =>
                  Sensor_PI.Tc (Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
               when Trigger_PRESENT =>  --  Timer expiration
                  Sensor_PI.Trigger (Global_State => Global_State);
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                --  Synchronous calls are not intercepted for the moment
                --  when Reset_Trigger_PRESENT =>
                --   Put_Line ("[RESET_trigger] PROCEDURE CALL OF RESET_trigger");
                --  Synchronous calls are not intercepted for the moment
                --  when Set_Trigger_PRESENT =>
                --   Put_Line ("[SET_trigger] PROCEDURE CALL OF SET_trigger");
                when Reading_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccSensor;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Reading_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccControlsystem then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Reading_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccControlsystem =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Controlsystem_PRESENT,
                                Controlsystem =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Reading_PRESENT,
                                               Reading =>
                                                  (signalValue =>  Event.Output_Event.Event.Sensor.Msg_Out.Reading.signalValue))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Controlsystem_Queue.Length < asn1SccEvents_Ty_length_index (Controlsystem_Queue_Size) then
                               Global_State.Controlsystem_Queue.Length :=
                                  Global_State.Controlsystem_Queue.Length + 1;
                               Global_State.Controlsystem_Queue.Data
                                 (Global_State.Controlsystem_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Controlsystem is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Reading");
                      end case;
                      <<Reading_Next_Dest>>
                   end loop;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccSensor_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Sensor: continuous signals");
               when Tc_PRESENT =>
                 Put ("Sensor: INPUT Tc");
                  Put_Line ("()");
               when Trigger_PRESENT => -- Timer expiration
                 Put ("Sensor: TIMEOUT Trigger");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Reading_PRESENT =>
                 Put ("Sensor: OUTPUT Reading");
                  Put_Line (" (" & Number_Pkg.Image (Event.Msg_Out.Reading.Signalvalue) & ")");
                --  Synchronous calls are not intercepted for the moment
                --  when Reset_Trigger_PRESENT =>
                --     Put_Line ("Sensor : CALL RESET_trigger");
                --  Synchronous calls are not intercepted for the moment
                --  when Set_Trigger_PRESENT =>
                --     Put_Line ("Sensor : CALL SET_trigger");
            end case;
      end case;
   end Print_Event;



end Sensor_Events;
