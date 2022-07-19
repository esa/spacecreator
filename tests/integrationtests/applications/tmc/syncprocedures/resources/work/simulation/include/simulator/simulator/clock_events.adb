with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

package body Clock_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccClock_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Clock,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Clock,
                 when others => asn1sccClock_Event_Init);
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
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Tick_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccClock;
                   Input_Event.Input_Event.Dest   := asn1sccPinger;
                   Input_Event.Input_Event.Event  :=
                      (Kind => Pinger_PRESENT,
                       Pinger =>
                          (Kind   => Msg_In_PRESENT,
                           Msg_In => (Kind => Tick_PRESENT,
                                      Tick =>
                                         (null record))));
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

   procedure Print_Event (Event : asn1sccClock_Event) is
   begin
      case Event.Kind is
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Tick_PRESENT =>
                 Put ("Clock: OUTPUT Tick");
                  Put_Line ("()");
            end case;
      end case;
   end Print_Event;

   procedure Exhaust_Interfaces
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean)) is
   begin
     --  Put_Line ("Exhaust interfaces of Clock");
      Exhaust_Tick (Global_State, Callback);
   end Exhaust_Interfaces;

   procedure Exhaust_Tick
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean))
   is
       Original_State : constant asn1SccSystem_State := Global_State;
       Event          : asn1SccObservable_Event
                         (Kind => Output_Event_PRESENT);
       Limit_Reached  : Boolean;
   begin
      --  Put_Line ("Exhausting interface Tick");
      --  Create an event to store as an edge of the state graph
      Event.Output_Event.Source := asn1SccClock;
      Event.Output_Event.Dest   := asn1SccPinger;
      Event.Output_Event.Event  :=
         (Kind => Clock_PRESENT,
          Clock =>
             (Kind   => Msg_OUT_PRESENT,
              Msg_Out => (Kind => Tick_PRESENT, others => <>)));

      Callback (Event, Limit_Reached);

      --  Restore previous state
      Global_State := Original_State;
   end Exhaust_Tick;


end Clock_Events;
