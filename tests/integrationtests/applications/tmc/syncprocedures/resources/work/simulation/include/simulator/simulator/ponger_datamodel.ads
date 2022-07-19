pragma Style_Checks (Off);
--  Code automatically generated by asn1scc tool
with adaasn1rtl;
pragma Warnings (Off, "unit ""adaasn1rtl.encoding"" is not referenced");
with adaasn1rtl.encoding;
pragma Warnings (On, "unit ""adaasn1rtl.encoding"" is not referenced");


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


pragma Warnings (Off, "no entities of ""SYNCPROCEDURES_DATAVIEW"" are referenced");
pragma Warnings (Off, "use clause for package ""SYNCPROCEDURES_DATAVIEW"" has no effect");
with SYNCPROCEDURES_DATAVIEW; use SYNCPROCEDURES_DATAVIEW;
pragma Warnings (On, "no entities of ""SYNCPROCEDURES_DATAVIEW"" are referenced");
pragma Warnings (On, "use clause for package ""SYNCPROCEDURES_DATAVIEW"" has no effect");



package Ponger_Datamodel with SPARK_Mode
is


subtype asn1SccPonger_States_index_range is Integer range 0..0;
type asn1SccPonger_States is (asn1Sccwait) with Size => adaasn1rtl.Enumerated_Size;
for asn1SccPonger_States use
    (asn1Sccwait => 0);

function asn1SccPonger_States_Equal(val1, val2 : asn1SccPonger_States) return Boolean;

function asn1SccPonger_States_Init return asn1SccPonger_States;

ERR_PONGER_STATES:constant Integer := 36; 
function asn1SccPonger_States_IsConstraintValid(val : asn1SccPonger_States) return adaasn1rtl.ASN1_RESULT;

ERR_UPER_ENCODE_PONGER_STATES:constant Integer := 37; 
asn1SccPonger_States_REQUIRED_BYTES_FOR_ENCODING:constant Integer := 0;
asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING:constant Integer := 0;

subtype asn1SccPonger_States_uPER_Stream is adaasn1rtl.encoding.Bitstream(1);

procedure asn1SccPonger_States_Encode (val : asn1SccPonger_States; Stream : out asn1SccPonger_States_uPER_Stream; result : OUT adaasn1rtl.ASN1_RESULT);
procedure asn1SccPonger_States_Encode_aux(val : asn1SccPonger_States; bs : in out adaasn1rtl.encoding.Bitstream; result : OUT adaasn1rtl.ASN1_RESULT)
with
    Pre => bs.Current_Bit_Pos < Natural'Last - asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING
      and then bs.Size_In_Bytes < Positive'Last / 8
      and then bs.Current_Bit_Pos + asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING <= bs.Size_In_Bytes * 8,
       Post => 
         (result.Success and bs.Current_Bit_Pos >= bs'Old.Current_Bit_Pos and bs.Current_Bit_Pos <= bs'Old.Current_Bit_Pos + asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING)
       or
         not result.Success
;

ERR_UPER_DECODE_PONGER_STATES:constant Integer := 38; 
procedure asn1SccPonger_States_Decode(val: out asn1SccPonger_States; Stream : in out asn1SccPonger_States_uPER_Stream; result : OUT adaasn1rtl.ASN1_RESULT);
procedure asn1SccPonger_States_Decode_aux(val: out asn1SccPonger_States; bs : in out adaasn1rtl.encoding.Bitstream; result : OUT adaasn1rtl.ASN1_RESULT)
with
    Pre => bs.Current_Bit_Pos < Natural'Last - asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING
      and then bs.Size_In_Bytes < Positive'Last / 8
      and then bs.Current_Bit_Pos + asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING <= bs.Size_In_Bytes * 8,
       Post => 
         (result.Success and bs.Current_Bit_Pos >= bs'Old.Current_Bit_Pos and bs.Current_Bit_Pos <= bs'Old.Current_Bit_Pos + asn1SccPonger_States_REQUIRED_BITS_FOR_ENCODING)
       or
         not result.Success
;
-- asn1SccPonger_Context --------------------------------------------

type asn1SccPonger_Context is record 
    state : asn1SccPonger_States;
    init_done : adaasn1rtl.Asn1Boolean;
    pingarg : SYNCPROCEDURES_DATAVIEW.asn1SccMyInteger;
end record;


function asn1SccPonger_Context_Equal(val1, val2 : asn1SccPonger_Context) return Boolean;

function asn1SccPonger_Context_Init return asn1SccPonger_Context;

ERR_PONGER_CONTEXT:constant Integer := 66; 
ERR_PONGER_CONTEXT_STATE_2:constant Integer := 46; 
ERR_PONGER_CONTEXT_INIT_DONE:constant Integer := 51; 
ERR_PONGER_CONTEXT_PINGARG_2:constant Integer := 61; 
function asn1SccPonger_Context_IsConstraintValid(val : asn1SccPonger_Context) return adaasn1rtl.ASN1_RESULT;

ERR_UPER_ENCODE_PONGER_CONTEXT:constant Integer := 67; 
ERR_UPER_ENCODE_PONGER_CONTEXT_STATE_2:constant Integer := 47; 
ERR_UPER_ENCODE_PONGER_CONTEXT_INIT_DONE:constant Integer := 52; 
ERR_UPER_ENCODE_PONGER_CONTEXT_PINGARG_2:constant Integer := 62; 
asn1SccPonger_Context_REQUIRED_BYTES_FOR_ENCODING:constant Integer := 2;
asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING:constant Integer := 15;

subtype asn1SccPonger_Context_uPER_Stream is adaasn1rtl.encoding.Bitstream(asn1SccPonger_Context_REQUIRED_BYTES_FOR_ENCODING);

procedure asn1SccPonger_Context_Encode (val : asn1SccPonger_Context; Stream : out asn1SccPonger_Context_uPER_Stream; result : OUT adaasn1rtl.ASN1_RESULT);
procedure asn1SccPonger_Context_Encode_aux(val : asn1SccPonger_Context; bs : in out adaasn1rtl.encoding.Bitstream; result : OUT adaasn1rtl.ASN1_RESULT)
with
    Pre => bs.Current_Bit_Pos < Natural'Last - asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING
      and then bs.Size_In_Bytes < Positive'Last / 8
      and then bs.Current_Bit_Pos + asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING <= bs.Size_In_Bytes * 8,
       Post => 
         (result.Success and bs.Current_Bit_Pos >= bs'Old.Current_Bit_Pos and bs.Current_Bit_Pos <= bs'Old.Current_Bit_Pos + asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING)
       or
         not result.Success
;

ERR_UPER_DECODE_PONGER_CONTEXT:constant Integer := 68; 
ERR_UPER_DECODE_PONGER_CONTEXT_STATE_2:constant Integer := 48; 
ERR_UPER_DECODE_PONGER_CONTEXT_INIT_DONE:constant Integer := 53; 
ERR_UPER_DECODE_PONGER_CONTEXT_PINGARG_2:constant Integer := 63; 
procedure asn1SccPonger_Context_Decode(val: out asn1SccPonger_Context; Stream : in out asn1SccPonger_Context_uPER_Stream; result : OUT adaasn1rtl.ASN1_RESULT);
procedure asn1SccPonger_Context_Decode_aux(val: out asn1SccPonger_Context; bs : in out adaasn1rtl.encoding.Bitstream; result : OUT adaasn1rtl.ASN1_RESULT)
with
    Pre => bs.Current_Bit_Pos < Natural'Last - asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING
      and then bs.Size_In_Bytes < Positive'Last / 8
      and then bs.Current_Bit_Pos + asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING <= bs.Size_In_Bytes * 8,
       Post => 
         (result.Success and bs.Current_Bit_Pos >= bs'Old.Current_Bit_Pos and bs.Current_Bit_Pos <= bs'Old.Current_Bit_Pos + asn1SccPonger_Context_REQUIRED_BITS_FOR_ENCODING)
       or
         not result.Success
;
pragma Warnings (Off, "there are no others");
 
pragma Warnings (On, "there are no others");
private
   --# hide Ponger_Datamodel;


end Ponger_Datamodel;