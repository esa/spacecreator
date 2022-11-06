with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Actuator1_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccActuator1_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Actuator1,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Actuator1,
                 when others => asn1sccActuator1_Event_Init);
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
                  Actuator1_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Actuatortc_PRESENT =>
                  declare
                     Param : asn1SccActuatorcommand := Evt.Msg_In.Actuatortc.Tc;
                  begin
                     Actuator1_PI.Actuatortc (Tc => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Actuatortm_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccActuator1;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Actuatortm_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccModemanager then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Actuatortm_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccModemanager =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Modemanager_PRESENT,
                                Modemanager =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Actuatortm1_PRESENT,
                                               Actuatortm1 =>
                                                  (tm =>  Event.Output_Event.Event.Actuator1.Msg_Out.Actuatortm.tm))));
                
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
                             Put_Line ("[X] Invalid destination for message Actuatortm");
                      end case;
                      <<Actuatortm_Next_Dest>>
                   end loop;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccActuator1_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Actuator1: continuous signals");
               when Actuatortc_PRESENT =>
                 Put ("Actuator1: INPUT Actuatortc");
                  Put_Line (" (" & Actuatorcommand_Pkg.Image (Event.Msg_In.Actuatortc.Tc) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Actuatortm_PRESENT =>
                 Put ("Actuator1: OUTPUT Actuatortm");
                  Put_Line (" (" & Actuatorstatus_Pkg.Image (Event.Msg_Out.Actuatortm.Tm) & ")");
            end case;
      end case;
   end Print_Event;



end Actuator1_Events;
