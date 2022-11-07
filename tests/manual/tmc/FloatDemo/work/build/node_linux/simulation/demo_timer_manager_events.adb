with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Demo_Timer_Manager_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccDemo_Timer_Manager_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Env,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Env,
                 when others => asn1sccDemo_Timer_Manager_Event_Init);
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
                  Demo_Timer_Manager_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Tick_PRESENT =>
                  Demo_Timer_Manager_PI.Tick (Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Sensor_Trigger_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccEnv;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Sensor_Trigger_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccSensor then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Sensor_Trigger_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccSensor =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Sensor_PRESENT,
                                Sensor =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Trigger_PRESENT,
                                               Trigger =>
                                                  (null record))));
                
                            --  Add to the message queue of the receiving function
                            --  if the receiving queue is not full (otherwise behaviour TBD)
                            if Global_State.Sensor_Queue.Length < asn1SccEvents_Ty_length_index (Sensor_Queue_Size) then
                               Global_State.Sensor_Queue.Length :=
                                  Global_State.Sensor_Queue.Length + 1;
                               Global_State.Sensor_Queue.Data
                                 (Global_State.Sensor_Queue.Length) := Input_Event;
                            else
                               Put_Line ("[X] Error: Message dropped because input queue of Sensor is full");
                            end if;
                          when others =>
                             Put_Line ("[X] Invalid destination for message Sensor_Trigger");
                      end case;
                      <<Sensor_Trigger_Next_Dest>>
                   end loop;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccDemo_Timer_Manager_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("demo_Timer_Manager: continuous signals");
               when Tick_PRESENT =>
                 Put ("demo_Timer_Manager: INPUT Tick");
                  Put_Line ("()");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Sensor_Trigger_PRESENT =>
                 Put ("demo_Timer_Manager: OUTPUT Sensor_Trigger");
                  Put_Line ("()");
            end case;
      end case;
   end Print_Event;



end Demo_Timer_Manager_Events;
