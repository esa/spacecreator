with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

package body Function_2_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccFunction_2_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Function_2,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Function_2,
                 when others => asn1sccFunction_2_Event_Init);
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
                  Function_2_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Pi_1_PRESENT =>
                  declare
                     Param : asn1SccMyseqof := Evt.Msg_In.Pi_1.P1;
                  begin
                     Function_2_PI.Pi_1 (P1 => Param, Global_State => Global_State);
                  end;
            end case;
         when Msg_OUT_Present =>
            --  Create a input message and place it in the recipient msg queue
            case Evt.Msg_Out.Kind is
                
                when Pi_2_PRESENT =>
                   null;
                   --  message is sent to the environment, which has no input queue
                   --  How to process this event in the simulator is TBD
                   --  (the model checker sends it to the observers before it is dropped here)
                   --  Put_Line ("[-] Message to environment: Pi_2");
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccFunction_2_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Function_2: continuous signals");
               when Pi_1_PRESENT =>
                 Put ("Function_2: INPUT Pi_1");
                  Put_Line (" (" & Myseqof_Pkg.Image (Event.Msg_In.Pi_1.P1) & ")");
            end case;
         when Msg_OUT_Present =>
            case Event.Msg_Out.Kind is
                when Pi_2_PRESENT =>
                 Put ("Function_2: OUTPUT Pi_2");
                  Put_Line (" (" & Mychoice_Pkg.Image (Event.Msg_Out.Pi_2.P1) & ")");
            end case;
      end case;
   end Print_Event;



end Function_2_Events;
