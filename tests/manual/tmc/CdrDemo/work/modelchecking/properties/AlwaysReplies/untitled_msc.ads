-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with Interfaces,
     Interfaces.C.Strings,
     Ada.Characters.Handling;

use Interfaces,
    Interfaces.C.Strings,
    Ada.Characters.Handling;

with Actuator_Datamodel;
use Actuator_Datamodel;
with Modemanager_Datamodel;
use Modemanager_Datamodel;
with CDRDEMO_DATAVIEW;
use CDRDEMO_DATAVIEW;
with TASTE_BasicTypes;
use TASTE_BasicTypes;
with System_Dataview;
use System_Dataview;
with Empty;
use Empty;
with Simulation_DataView;
use Simulation_DataView;
with Simulation_Timers_DataView;
use Simulation_Timers_DataView;
with adaasn1rtl;
use adaasn1rtl;
with Untitled_MSC_Datamodel; use Untitled_MSC_Datamodel;

with Untitled_MSC_RI;
package Untitled_MSC with Elaborate_Body is
   ctxt : aliased asn1SccUntitled_msc_Context :=
      (Init_Done => False,
       sender => asn1Sccenv,
      others => <>);
   event : asn1SccObservable_Event;
   self : constant asn1SccPID := asn1Sccenv;
   function To_Systemcommand_selection (Src : CDRDEMO_DATAVIEW.asn1SccSystemcommand_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Systemcommand_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Systemcommand_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator1_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccActuator1_Event_Msg_In_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator1_Event_Msg_In_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator1_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator1_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccActuator1_Event_Msg_Out_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator1_Event_Msg_Out_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator1_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator1_Event_selection (Src : Simulation_DataView.asn1SccActuator1_Event_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator1_Event_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator1_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator2_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccActuator2_Event_Msg_In_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator2_Event_Msg_In_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator2_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator2_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccActuator2_Event_Msg_Out_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator2_Event_Msg_Out_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator2_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator2_Event_selection (Src : Simulation_DataView.asn1SccActuator2_Event_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator2_Event_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Actuator2_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Egse_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccEgse_Event_Msg_In_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Egse_Event_Msg_In_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Egse_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Egse_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccEgse_Event_Msg_Out_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Egse_Event_Msg_Out_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Egse_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Egse_Event_selection (Src : Simulation_DataView.asn1SccEgse_Event_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Egse_Event_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Egse_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Modemanager_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccModemanager_Event_Msg_In_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Modemanager_Event_Msg_In_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Modemanager_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Modemanager_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccModemanager_Event_Msg_Out_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Modemanager_Event_Msg_Out_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Modemanager_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Modemanager_Event_selection (Src : Simulation_DataView.asn1SccModemanager_Event_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Modemanager_Event_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Modemanager_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Function_Event_selection (Src : Simulation_DataView.asn1SccFunction_Event_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Function_Event_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Function_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Observable_Event_selection (Src : Simulation_DataView.asn1SccObservable_Event_selection) return Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Observable_Event_selection is (Untitled_MSC_Datamodel.asn1SccUntitled_MSC_Observable_Event_selection'Enum_Val (Src'Enum_Rep));
   function Get_State return Chars_Ptr is (Untitled_Msc_RI.To_C_Pointer (asn1SccUntitled_MSC_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "untitled_msc_state";
   procedure Startup with Export, Convention => C, Link_Name => "Untitled_MSC_startup";
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 5;
end Untitled_MSC;