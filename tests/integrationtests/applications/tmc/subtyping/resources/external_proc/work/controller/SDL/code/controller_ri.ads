--  DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
--  Generated by TASTE (Kazoo template templates/skeletons/ada-ri-source/function.tmplt)
--  2022-12-08 15:49:22
with Externalproc_Dataview; use Externalproc_Dataview;
with Taste_Basictypes;      use Taste_Basictypes;
with adaasn1rtl;
with Interfaces.C.Strings; use Interfaces.C.Strings;

package Controller_RI is

   procedure Testprotected (X : in out asn1SccMyinteger; Y : out asn1SccMyinteger);
   
   
   procedure Testunprotected (X : in out asn1SccMyinteger; Y : out asn1SccMyinteger);

   procedure Check_Queue (Res : out adaasn1rtl.Asn1Boolean)
      with Import, Convention => C, Link_Name => "controller_check_queue";

   --  Return the SDL state as a char * for the MSC tracing function
   function To_C_Pointer (State_As_String : String) return Chars_Ptr is
      (New_String (State_As_String));
end Controller_RI;
