with Interfaces.C.Extensions;
use Interfaces.C.Extensions;

package Calculate_QGenC is

procedure Ada_Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(pBuffer : Interfaces.C.char_array);
procedure Ada_Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(pBuffer : Interfaces.C.char_array);
procedure Ada_Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(pBuffer : Interfaces.C.char_array);
procedure Ada_Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(pBuffer : in Interfaces.C.char_array; bytesWritten : out Integer);
procedure Ada_Execute_Calculate_QGenC;

end Calculate_QGenC;
