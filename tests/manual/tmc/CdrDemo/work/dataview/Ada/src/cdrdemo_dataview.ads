pragma Style_Checks (Off);
--  Code automatically generated by asn1scc tool
pragma Warnings (Off, "no entities of ""Interfaces"" are referenced");
with Interfaces;
pragma Warnings (On, "no entities of ""Interfaces"" are referenced");

pragma Warnings (Off, "use clause for package ""Interfaces"" has no effect");
use Interfaces;
pragma Warnings (On, "use clause for package ""Interfaces"" has no effect");

with adaasn1rtl;


pragma Warnings (Off, "unit ""System"" is not referenced");
with System;
pragma Warnings (On, "unit ""System"" is not referenced");

pragma Warnings (Off, "use clause for package ""adaasn1rtl"" has no effect");
use adaasn1rtl;
pragma Warnings (On, "use clause for package ""adaasn1rtl"" has no effect");

pragma Warnings (Off, "use clause for type");
pragma Warnings (Off, "is already use-visible through package use clause at line");
use type adaasn1rtl.OctetBuffer;
use type adaasn1rtl.BitArray;
use type adaasn1rtl.Asn1UInt;
use type adaasn1rtl.Asn1Int;
use type adaasn1rtl.BIT;
pragma Warnings (On, "use clause for type");
pragma Warnings (On, "is already use-visible through package use clause at line");



package CDRDEMO_DATAVIEW with SPARK_Mode
is


subtype asn1SccDummy is adaasn1rtl.Asn1UInt range 0 .. 0;


function asn1SccDummy_Equal(val1, val2 : asn1SccDummy) return Boolean;

ERR_DUMMY:constant Integer := 1; -- (0..0)
function asn1SccDummy_IsConstraintValid(val : asn1SccDummy) return adaasn1rtl.ASN1_RESULT;

function asn1SccDummy_Init  return asn1SccDummy is
(0);
subtype asn1SccDataItem is adaasn1rtl.Asn1UInt range 0 .. 255;


function asn1SccDataItem_Equal(val1, val2 : asn1SccDataItem) return Boolean;

ERR_DATAITEM:constant Integer := 6; -- (0..255)
function asn1SccDataItem_IsConstraintValid(val : asn1SccDataItem) return adaasn1rtl.ASN1_RESULT;

function asn1SccDataItem_Init  return asn1SccDataItem is
(0);
-- asn1SccActuatorConfiguration --------------------------------------------

type asn1SccActuatorConfiguration is record 
    threshold1 : asn1SccDataItem;
    threshold2 : asn1SccDataItem;
end record;


function asn1SccActuatorConfiguration_Equal(val1, val2 : asn1SccActuatorConfiguration) return Boolean;

ERR_ACTUATORCONFIGURATION:constant Integer := 46; -- 
ERR_ACTUATORCONFIGURATION_THRESHOLD1_2:constant Integer := 31; -- 
ERR_ACTUATORCONFIGURATION_THRESHOLD2_2:constant Integer := 41; -- 
function asn1SccActuatorConfiguration_IsConstraintValid(val : asn1SccActuatorConfiguration) return adaasn1rtl.ASN1_RESULT;

function asn1SccActuatorConfiguration_Init  return asn1SccActuatorConfiguration is
((threshold1 => 0, threshold2 => 0));
-- asn1SccSystemCommand --------------------------------------------

subtype asn1SccSystemCommand_index_range is Integer range 0..2;

type asn1SccSystemCommand_selection is (goToIdle_PRESENT, goToOperational_PRESENT, doReport_PRESENT);
for asn1SccSystemCommand_selection use
    (goToIdle_PRESENT => 1, goToOperational_PRESENT => 2, doReport_PRESENT => 3);
for asn1SccSystemCommand_selection'Size use 32;

type asn1SccSystemCommand(kind : asn1SccSystemCommand_selection:= goToIdle_PRESENT) is 
record
    case kind is
    when goToIdle_PRESENT =>
            goToIdle: asn1SccDummy;
    when goToOperational_PRESENT =>
            goToOperational: asn1SccActuatorConfiguration;
    when doReport_PRESENT =>
            doReport: asn1SccDummy;
    end case;
end record;

for asn1SccSystemCommand use 
record
    kind at 0 range 0..31;
end record;

function asn1SccSystemCommand_Equal(val1, val2 : asn1SccSystemCommand) return Boolean;

ERR_SYSTEMCOMMAND:constant Integer := 101; -- 
ERR_SYSTEMCOMMAND_GOTOIDLE_2:constant Integer := 56; -- 
ERR_SYSTEMCOMMAND_GOTOOPERATIONAL_2:constant Integer := 86; -- 
ERR_SYSTEMCOMMAND_DOREPORT_2:constant Integer := 96; -- 
function asn1SccSystemCommand_IsConstraintValid(val : asn1SccSystemCommand) return adaasn1rtl.ASN1_RESULT;

function asn1SccSystemCommand_Init  return asn1SccSystemCommand is
((kind => goToIdle_PRESENT, goToIdle => 0));
subtype asn1SccActuatorIndex_index_range is Integer range 0..1;
type asn1SccActuatorIndex is (asn1Sccactuator1, asn1Sccactuator2) with Size => adaasn1rtl.Enumerated_Size;
for asn1SccActuatorIndex use
    (asn1Sccactuator1 => 0, asn1Sccactuator2 => 1);

function asn1SccActuatorIndex_Equal(val1, val2 : asn1SccActuatorIndex) return Boolean;

ERR_ACTUATORINDEX:constant Integer := 11; -- actuator1 | actuator2
function asn1SccActuatorIndex_IsConstraintValid(val : asn1SccActuatorIndex) return adaasn1rtl.ASN1_RESULT;

function asn1SccActuatorIndex_Init  return asn1SccActuatorIndex is
(asn1Sccactuator1);
subtype asn1SccActuatorCommand_index_range is Integer range 0..2;
type asn1SccActuatorCommand is (asn1SccactuatorArm, asn1SccactuatorExecute, asn1SccactuatorStandby) with Size => adaasn1rtl.Enumerated_Size;
for asn1SccActuatorCommand use
    (asn1SccactuatorArm => 0, asn1SccactuatorExecute => 1, asn1SccactuatorStandby => 2);

function asn1SccActuatorCommand_Equal(val1, val2 : asn1SccActuatorCommand) return Boolean;

ERR_ACTUATORCOMMAND:constant Integer := 16; -- actuatorArm | actuatorExecute | actuatorStandby
function asn1SccActuatorCommand_IsConstraintValid(val : asn1SccActuatorCommand) return adaasn1rtl.ASN1_RESULT;

function asn1SccActuatorCommand_Init  return asn1SccActuatorCommand is
(asn1SccactuatorArm);
subtype asn1SccActuatorStatus_index_range is Integer range 0..2;
type asn1SccActuatorStatus is (asn1SccactuatorArmed, asn1SccactuatorExecuting, asn1SccactuatorStandingBy) with Size => adaasn1rtl.Enumerated_Size;
for asn1SccActuatorStatus use
    (asn1SccactuatorArmed => 0, asn1SccactuatorExecuting => 1, asn1SccactuatorStandingBy => 2);

function asn1SccActuatorStatus_Equal(val1, val2 : asn1SccActuatorStatus) return Boolean;

ERR_ACTUATORSTATUS:constant Integer := 21; -- actuatorArmed | actuatorExecuting | actuatorStandingBy
function asn1SccActuatorStatus_IsConstraintValid(val : asn1SccActuatorStatus) return adaasn1rtl.ASN1_RESULT;

function asn1SccActuatorStatus_Init  return asn1SccActuatorStatus is
(asn1SccactuatorArmed);
-- asn1SccSystemTelemetry --------------------------------------------

type asn1SccSystemTelemetry is record 
    reading : asn1SccDataItem;
    status1 : asn1SccActuatorStatus;
    status2 : asn1SccActuatorStatus;
end record;


function asn1SccSystemTelemetry_Equal(val1, val2 : asn1SccSystemTelemetry) return Boolean;

ERR_SYSTEMTELEMETRY:constant Integer := 136; -- 
ERR_SYSTEMTELEMETRY_READING_2:constant Integer := 111; -- 
ERR_SYSTEMTELEMETRY_STATUS1_2:constant Integer := 121; -- 
ERR_SYSTEMTELEMETRY_STATUS2_2:constant Integer := 131; -- 
function asn1SccSystemTelemetry_IsConstraintValid(val : asn1SccSystemTelemetry) return adaasn1rtl.ASN1_RESULT;

function asn1SccSystemTelemetry_Init  return asn1SccSystemTelemetry is
((reading => 0, status1 => asn1SccactuatorArmed, status2 => asn1SccactuatorArmed));
pragma Warnings (Off, "there are no others");
 
pragma Warnings (On, "there are no others");
private
   --# hide CDRDEMO_DATAVIEW;


end CDRDEMO_DATAVIEW;