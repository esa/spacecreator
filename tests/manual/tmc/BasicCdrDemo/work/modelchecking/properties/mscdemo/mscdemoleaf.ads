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
with BASICCDRDEMO_DATAVIEW;
use BASICCDRDEMO_DATAVIEW;
with TASTE_BasicTypes;
use TASTE_BasicTypes;
with System_Dataview;
use System_Dataview;
with Simulation_DataView;
use Simulation_DataView;
with Simulation_Timers_DataView;
use Simulation_Timers_DataView;
with adaasn1rtl;
use adaasn1rtl;
with mscdemoleaf_Datamodel; use mscdemoleaf_Datamodel;

with mscdemoleaf_RI;
package mscdemoleaf with Elaborate_Body is
   ctxt : aliased asn1SccMscdemoleaf_Context :=
      (Init_Done => False,
       sender => asn1Sccenv,
      others => <>);
   event : asn1SccObservable_Event;
   self : constant asn1SccPID := asn1Sccenv;
   function To_Actuator_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccActuator_Event_Msg_In_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Actuator_Event_Msg_In_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Actuator_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccActuator_Event_Msg_Out_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Actuator_Event_Msg_Out_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Actuator_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Actuator_Event_selection (Src : Simulation_DataView.asn1SccActuator_Event_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Actuator_Event_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Actuator_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Sensor_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccSensor_Event_Msg_Out_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Sensor_Event_Msg_Out_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Sensor_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Sensor_Event_selection (Src : Simulation_DataView.asn1SccSensor_Event_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Sensor_Event_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Sensor_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Egse_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccEgse_Event_Msg_In_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Egse_Event_Msg_In_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Egse_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Egse_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccEgse_Event_Msg_Out_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Egse_Event_Msg_Out_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Egse_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Egse_Event_selection (Src : Simulation_DataView.asn1SccEgse_Event_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Egse_Event_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Egse_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Modemanager_Event_Msg_In_selection (Src : Simulation_DataView.asn1SccModemanager_Event_Msg_In_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Modemanager_Event_Msg_In_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Modemanager_Event_Msg_In_selection'Enum_Val (Src'Enum_Rep));
   function To_Modemanager_Event_Msg_Out_selection (Src : Simulation_DataView.asn1SccModemanager_Event_Msg_Out_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Modemanager_Event_Msg_Out_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Modemanager_Event_Msg_Out_selection'Enum_Val (Src'Enum_Rep));
   function To_Modemanager_Event_selection (Src : Simulation_DataView.asn1SccModemanager_Event_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Modemanager_Event_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Modemanager_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Function_Event_selection (Src : Simulation_DataView.asn1SccFunction_Event_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Function_Event_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Function_Event_selection'Enum_Val (Src'Enum_Rep));
   function To_Observable_Event_selection (Src : Simulation_DataView.asn1SccObservable_Event_selection) return mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Observable_Event_selection is (mscdemoleaf_Datamodel.asn1Sccmscdemoleaf_Observable_Event_selection'Enum_Val (Src'Enum_Rep));
   function Get_State return Chars_Ptr is (Mscdemoleaf_RI.To_C_Pointer (asn1Sccmscdemoleaf_States'Image (ctxt.State))) with Export, Convention => C, Link_Name => "mscdemoleaf_state";
   procedure Startup with Export, Convention => C, Link_Name => "mscdemoleaf_startup";
   procedure Execute_Transition (Id : Integer);
   CS_Only : constant := 6;
end mscdemoleaf;