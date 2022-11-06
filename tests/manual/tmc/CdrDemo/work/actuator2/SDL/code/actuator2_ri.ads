--  DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
--  Generated by TASTE (kazoo/templates/skeletons/ada-ri-source/function.tmplt)
with Cdrdemo_Dataview; use Cdrdemo_Dataview;
with Taste_Basictypes; use Taste_Basictypes;
with System_Dataview;  use System_Dataview;
with adaasn1rtl;
with Interfaces.C.Strings; use Interfaces.C.Strings;
with System_Dataview; use System_Dataview;  --  contains definition of the PID type

package Actuator2_RI is

   procedure Actuatortm (Tm : in out asn1SccActuatorstatus; Dest_PID : asn1sccPID := asn1sccEnv);

   procedure Check_Queue (Res : out adaasn1rtl.Asn1Boolean)
      with Import, Convention => C, Link_Name => "actuator2_check_queue";

   procedure Get_Sender (Sender : out asn1sccPID; Dest_PID : asn1SccPID := asn1sccEnv)
      with Import, Convention => C, Link_Name => "actuator2_get_sender";

   --  Return the SDL state as a char * for the MSC tracing function
   function To_C_Pointer (State_As_String : String) return Chars_Ptr is
      (New_String (State_As_String));
end Actuator2_RI;
