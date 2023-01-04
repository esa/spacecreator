with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

package body Function_1_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccFunction_1_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Function_1,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Function_1,
                 when others => asn1sccFunction_1_Event_Init);
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
               when Pi_2_PRESENT =>
                  declare
                     Param : asn1SccMychoice := Evt.Msg_In.Pi_2.P1;
                  begin
                     Function_1_PI.Pi_2 (P1 => Param, Global_State => Global_State);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Pi_1_PRESENT =>
                   Input_Event.Input_Event.Source := asn1sccFunction_1;
                   Input_Event.Input_Event.Dest   := asn1sccFunction_2;
                   Input_Event.Input_Event.Event  :=
                      (Kind => Function_2_PRESENT,
                       Function_2 =>
                          (Kind   => Msg_In_PRESENT,
                           Msg_In => (Kind => Pi_1_PRESENT,
                                      Pi_1 =>
                                         (p1 =>  Event.Output_Event.Event.Function_1.Msg_Out.Pi_1.p1))));
                   --  Add to the message queue of the receiving function
                   --  if the receiving queue is not full (otherwise behaviour TBD)
                   if Global_State.Function_2_Queue.Length < asn1SccEvents_Ty_length_index (Function_2_Queue_Size) then
                      Global_State.Function_2_Queue.Length :=
                         Global_State.Function_2_Queue.Length + 1;
                      Global_State.Function_2_Queue.Data
                        (Global_State.Function_2_Queue.Length) := Input_Event;
                   else
                      Put_Line ("[X] Error: Message dropped because input queue of Function_2 is full");
                   end if;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccFunction_1_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Pi_2_PRESENT =>
                 Put ("Function_1: INPUT Pi_2");
                  Put_Line (" (" & Mychoice_Pkg.Image (Event.Msg_In.Pi_2.P1) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Pi_1_PRESENT =>
                 Put ("Function_1: OUTPUT Pi_1");
                  Put_Line (" (" & Myseqof_Pkg.Image (Event.Msg_Out.Pi_1.P1) & ")");
            end case;
      end case;
   end Print_Event;

   procedure Exhaust_Interfaces
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean)) is
   begin
     --  Put_Line ("Exhaust interfaces of Function_1");
      Exhaust_Pi_1 (Global_State, Callback);
   end Exhaust_Interfaces;

   procedure Exhaust_Pi_1
      (Global_State : in out asn1SccSystem_State;
       Callback     : access procedure (Event         : asn1SccObservable_Event;
                                        Limit_Reached : out Boolean))
   is
       Iterator     : Myseqof_Pkg.Instance;
       Param        : asn1SccMyseqof;
       Original_State : constant asn1SccSystem_State := Global_State;
       Event          : asn1SccObservable_Event
                         (Kind => Output_Event_PRESENT);
       Limit_Reached  : Boolean;
   begin
      --  Put_Line ("Exhausting interface Pi_1");
      --  Create an event to store as an edge of the state graph
      Event.Output_Event.Source := asn1SccFunction_1;
      Event.Output_Event.Dest   := asn1SccFunction_2;
      Event.Output_Event.Event  :=
         (Kind => Function_1_PRESENT,
          Function_1 =>
             (Kind   => Msg_OUT_PRESENT,
              Msg_Out => (Kind => Pi_1_PRESENT, others => <>)));

      for Each of Iterator loop
         --  Iterate exhaustively over the interface parameter
         Param := Myseqof_Pkg.To_ASN1 (Each);
         Event.Output_Event.Event.Function_1.Msg_Out.Pi_1.p1 := Param;
         --  the Callback will call the observers, then execute the event
         --  (which may have been altered by the observers), and then
         --  process all the new events possibly generated during this execution
         --  If a new state is generated at the end, it will be added to the graph
         Callback (Event, Limit_Reached);
         --  Restore previous state
         Global_State := Original_State;
         exit when Limit_Reached;
      end loop;
   end Exhaust_Pi_1;


end Function_1_Events;
