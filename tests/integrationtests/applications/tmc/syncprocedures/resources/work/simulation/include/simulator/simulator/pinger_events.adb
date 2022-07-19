with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

package body Pinger_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccPinger_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Pinger,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Pinger,
                 when others => asn1sccPinger_Event_Init);
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
                  Pinger_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Pong_PRESENT =>
                  declare
                     Param : asn1SccMyinteger := Evt.Msg_In.Pong.Pongarg;
                  begin
                     Pinger_PI.Pong (Pongarg => Param, Global_State => Global_State);
                  end;
               when Tick_PRESENT =>
                  Pinger_PI.Tick (Global_State => Global_State);
            end case;
         when Msg_OUT_Present =>
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Ping_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccPinger;
                   Input_Event.Input_Event.Dest   := asn1sccPonger;
                   Input_Event.Input_Event.Event  :=
                      (Kind => Ponger_PRESENT,
                       Ponger =>
                          (Kind   => Msg_In_PRESENT,
                           Msg_In => (Kind => Ping_PRESENT,
                                      Ping =>
                                         (pingArg =>  Event.Output_Event.Event.Pinger.Msg_Out.Ping.pingArg))));
                   --  Add to the message queue of the receiving function
                   --  if the receiving queue is not full (otherwise behaviour TBD)
                   if Global_State.Ponger_Queue.Length < asn1SccEvents_Ty_length_index (Ponger_Queue_Size) then
                      Global_State.Ponger_Queue.Length :=
                         Global_State.Ponger_Queue.Length + 1;
                      Global_State.Ponger_Queue.Data
                        (Global_State.Ponger_Queue.Length) := Input_Event;
                   else
                      Put_Line ("[X] Error: Message dropped because input queue of Ponger is full");
                   end if;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccPinger_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Pinger: continuous signals");
               when Pong_PRESENT =>
                 Put ("Pinger: INPUT Pong");
                  Put_Line (" (" & Myinteger_Pkg.Image (Event.Msg_In.Pong.Pongarg) & ")");
               when Tick_PRESENT =>
                 Put ("Pinger: INPUT Tick");
                  Put_Line ("()");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Ping_PRESENT =>
                 Put ("Pinger: OUTPUT Ping");
                  Put_Line (" (" & Myinteger_Pkg.Image (Event.Msg_Out.Ping.Pingarg) & ")");
            end case;
      end case;
   end Print_Event;



end Pinger_Events;
