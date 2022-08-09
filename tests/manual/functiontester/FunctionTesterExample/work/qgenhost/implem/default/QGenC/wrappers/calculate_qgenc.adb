with Interfaces.C.Extensions;
use Interfaces.C.Extensions;

package body Calculate_QGenC is

procedure Ada_Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(pBuffer : Interfaces.C.char_array) is
    procedure C_Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(pBuffer : Interfaces.C.char_array);
    pragma Import(C, C_Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam, "Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam");
begin
    C_Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(pBuffer);
end Ada_Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam;

procedure Ada_Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(pBuffer : Interfaces.C.char_array) is
    procedure C_Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(pBuffer : Interfaces.C.char_array);
    pragma Import(C, C_Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam, "Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam");
begin
    C_Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(pBuffer);
end Ada_Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam;

procedure Ada_Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(pBuffer : Interfaces.C.char_array) is
    procedure C_Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(pBuffer : Interfaces.C.char_array);
    pragma Import(C, C_Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam, "Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam");
begin
    C_Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(pBuffer);
end Ada_Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam;

procedure Ada_Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(pBuffer : in Interfaces.C.char_array; bytesWritten : out Integer) is
    function C_Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(pBuffer : Interfaces.C.char_array) return Integer;
    pragma Import(C, C_Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam, "Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam");
begin
    bytesWritten := C_Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(pBuffer);
end Ada_Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam;

procedure Ada_Execute_Calculate_QGenC is
    procedure C_Execute_Calculate_QGenC;
    pragma Import(C, C_Execute_Calculate_QGenC, "Execute_Calculate_QGenC");
begin
    C_Execute_Calculate_QGenC;
end Ada_Execute_Calculate_QGenC;


end Calculate_QGenC;
