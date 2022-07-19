with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

package body Ponger_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccPonger_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Ponger,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Ponger,
                 when others => asn1sccPonger_Event_Init);
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
                  Ponger_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Ping_PRESENT =>
                  declare
                     Param : asn1SccMyinteger := Evt.Msg_In.Ping.Pingarg;
                  begin
                     Ponger_PI.Ping (Pingarg => Param, Global_State => Global_State);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Pong_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccPonger;
                   Input_Event.Input_Event.Dest   := asn1sccPinger;
                   Input_Event.Input_Event.Event  :=
                      (Kind => Pinger_PRESENT,
                       Pinger =>
                          (Kind   => Msg_In_PRESENT,
                           Msg_In => (Kind => Pong_PRESENT,
                                      Pong =>
                                         (pongArg =>  Event.Output_Event.Event.Ponger.Msg_Out.Pong.pongArg))));
                   --  Add to the message queue of the receiving function
                   --  if the receiving queue is not full (otherwise behaviour TBD)
                   if Global_State.Pinger_Queue.Length < asn1SccEvents_Ty_length_index (Pinger_Queue_Size) then
                      Global_State.Pinger_Queue.Length :=
                         Global_State.Pinger_Queue.Length + 1;
                      Global_State.Pinger_Queue.Data
                        (Global_State.Pinger_Queue.Length) := Input_Event;
                   else
                      Put_Line ("[X] Error: Message dropped because input queue of Pinger is full");
                   end if;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccPonger_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Ponger: continuous signals");
               when Ping_PRESENT =>
                 Put ("Ponger: INPUT Ping");
                  Put_Line (" (" & Myinteger_Pkg.Image (Event.Msg_In.Ping.Pingarg) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Pong_PRESENT =>
                 Put ("Ponger: OUTPUT Pong");
                  Put_Line (" (" & Myinteger_Pkg.Image (Event.Msg_Out.Pong.Pongarg) & ")");
            end case;
      end case;
   end Print_Event;



end Ponger_Events;
