with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Egse_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccEgse_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Egse,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Egse,
                 when others => asn1sccEgse_Event_Init);
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
               when Systemtm_PRESENT =>
                  declare
                     Param : asn1SccSystemtelemetry := Evt.Msg_In.Systemtm.Tm;
                  begin
                     Egse_PI.Systemtm (Tm => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Systemtc_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccEgse;
                   for PID in asn1SccPID loop
                      if PID = asn1sccEnv then
                         goto Systemtc_Next_Dest;
                      elsif Event.Output_Event.Dest = PID then
                         --  One destination
                         Input_Event.Input_Event.Dest := Event.Output_Event.Dest;
                      elsif PID in asn1SccModemanager then
                         --  Multicast to all possible destinations
                         Input_Event.Input_Event.Dest := PID;
                      else
                         goto Systemtc_Next_Dest;
                      end if;
                      case Input_Event.Input_Event.Dest is
                         when asn1sccModemanager =>
                            Input_Event.Input_Event.Event  :=
                               (Kind => Modemanager_PRESENT,
                                Modemanager =>
                                   (Kind   => Msg_In_PRESENT,
                                    Msg_In => (Kind => Systemtc_PRESENT,
                                               Systemtc =>
                                                  (tc =>  Event.Output_Event.Event.Egse.Msg_Out.Systemtc.tc))));
                
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
                             Put_Line ("[X] Invalid destination for message Systemtc");
                      end case;
                      <<Systemtc_Next_Dest>>
                   end loop;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccEgse_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Systemtm_PRESENT =>
                 Put ("EGSE: INPUT Systemtm");
                  Put_Line (" (" & Systemtelemetry_Pkg.Image (Event.Msg_In.Systemtm.Tm) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Systemtc_PRESENT =>
                 Put ("EGSE: OUTPUT Systemtc");
                  Put_Line (" (" & Systemcommand_Pkg.Image (Event.Msg_Out.Systemtc.Tc) & ")");
            end case;
      end case;
   end Print_Event;

   procedure Exhaust_Interfaces
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean)) is
   begin
     --  Put_Line ("Exhaust interfaces of EGSE");
      Exhaust_Systemtc (Global_State, Callback);
   end Exhaust_Interfaces;

   procedure Exhaust_Systemtc
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean))
   is
       Iterator     : Systemcommand_Pkg.Instance;
       Param        : asn1SccSystemcommand;
       Original_State : constant asn1SccSystem_State := Global_State;
       Event          : asn1SccObservable_Event
                         (Kind => Output_Event_PRESENT);
       Limit_Reached  : Boolean;
   begin
      --  Put_Line ("Exhausting interface Systemtc");
      --  Create an event to store as an edge of the state graph
      Event.Output_Event.Source := asn1SccEgse;
      Event.Output_Event.Dest   := asn1SccModemanager;
      Event.Output_Event.Event  :=
         (Kind => Egse_PRESENT,
          Egse =>
             (Kind   => Msg_OUT_PRESENT,
              Msg_Out => (Kind => Systemtc_PRESENT, others => <>)));

      for Each of Iterator loop
         --  Iterate exhaustively over the interface parameter
         Param := Systemcommand_Pkg.To_ASN1 (Each);
         Event.Output_Event.Event.Egse.Msg_Out.Systemtc.tc := Param;
         --  the Callback will call the observers, then execute the event
         --  (which may have been altered by the observers), and then
         --  process all the new events possibly generated during this execution
         --  If a new state is generated at the end, it will be added to the graph
         Callback (Event, Limit_Reached);
         --  Restore previous state
         Global_State := Original_State;
         exit when Limit_Reached;
      end loop;
   end Exhaust_Systemtc;


end Egse_Events;
