--  DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
--  Generated by TASTE (Kazoo template templates/skeletons/ada-ri-source/function.tmplt)
--  2022-09-19 12:13:51
with Basic_Types;                use Basic_Types;
with Bilevel_Types;              use Bilevel_Types;
with Uart_Dlc;                   use Uart_Dlc;
with Taste_Basictypes;           use Taste_Basictypes;
with Uart_Protocol_Dlc_Dataview; use Uart_Protocol_Dlc_Dataview;
with adaasn1rtl;
with Interfaces.C.Strings; use Interfaces.C.Strings;

package Rtdlcr_RI is

   procedure Dlcrxerror (P1 : in out asn1SccT_Uart_Dlc_Rxerrorcode);
   
   
   procedure Dlcrxsdu (P1 : in out asn1SccT_Uart_Dlc_Sdu);

   --  Timer FrameTimer Set and Reset functions
   procedure Set_Frametimer (Val : in out asn1SccT_UInt32);

   procedure Reset_Frametimer;

   procedure Check_Queue (Res : out adaasn1rtl.Asn1Boolean)
      with Import, Convention => C, Link_Name => "rtdlcr_check_queue";

   --  Return the SDL state as a char * for the MSC tracing function
   function To_C_Pointer (State_As_String : String) return Chars_Ptr is
      (New_String (State_As_String));
end Rtdlcr_RI;
