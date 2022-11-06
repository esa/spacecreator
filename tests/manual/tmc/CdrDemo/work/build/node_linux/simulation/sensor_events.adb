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
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Reading_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccSensor;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Reading_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccModemanager then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Reading_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccModemanager =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Modemanager_PRESENT,
                                Modemanager =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Reading_PRESENT,
                                               Reading =>
                                                  (x =>  Event.Output_Event.Event.Sensor.Msg_Out.Reading.x))));
                
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
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Reading_PRESENT =>
                 Put ("Sensor: OUTPUT Reading");
                  Put_Line (" (" & Dataitem_Pkg.Image (Event.Msg_Out.Reading.X) & ")");
            end case;
      end case;
   end Print_Event;



end Sensor_Events;
