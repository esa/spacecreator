with Text_IO; use Text_IO;

with ASN1_Ada_Iterators.Iterators;         use ASN1_Ada_Iterators.Iterators;

--  source: kazoo/templates/concurrency_view/simulation-adb/block.tmplt
package body Actuator_Events is

   procedure Process_Event
     (Event        : asn1sccObservable_Event;
      Global_State : in out asn1SccSystem_State)
   is
      Evt : constant asn1sccActuator_Event :=
              (case Event.Kind is
                 when Input_Event_Present =>
                    Event.Input_Event.Event.Actuator,
                 when Output_Event_Present =>
                    Event.Output_Event.Event.Actuator,
                 when others => asn1sccActuator_Event_Init);
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
                  Actuator_PI.Continuous_Signals (Global_State);
                  CS_Barrier := True;
               when Controlsignal_PRESENT =>
                  declare
                     Param : asn1SccNumber := Evt.Msg_In.Controlsignal.Controlvalue;
                  begin
                     Actuator_PI.Controlsignal (Controlvalue => Param, Global_State => Global_State, Sender_PID => Event.Input_Event.Source);
                  end;
            end case;
      end case;
   end Process_Event;

   procedure Print_Event (Event : asn1sccActuator_Event) is
   begin
      case Event.Kind is
         when Msg_IN_Present =>
            case Event.Msg_In.Kind is
               when Input_None_PRESENT =>
                  Put_Line ("Actuator: continuous signals");
               when Controlsignal_PRESENT =>
                 Put ("Actuator: INPUT Controlsignal");
                  Put_Line (" (" & Number_Pkg.Image (Event.Msg_In.Controlsignal.Controlvalue) & ")");
            end case;
      end case;
   end Print_Event;



end Actuator_Events;
