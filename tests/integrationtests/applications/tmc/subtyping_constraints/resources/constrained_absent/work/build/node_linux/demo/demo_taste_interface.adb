with PolyORB_HI.Utils;
use  PolyORB_HI.Utils;
with PolyORB_HI_Generated.Activity,
     PolyORB_HI.Errors;
use  PolyORB_HI_Generated.Activity;

package body Demo_TASTE_Interface is
   --  generated from templates/concurrency_view/ada_wrappers_body/partition.tmplt
   
   --  The package body comes templates/concurrency_view/ada_wrappers_body/block.tmplt
   package body Function_1 is
   
      --  Initialization function is implemented in C (in vm_if.c)
      procedure Init is
         procedure C_Init
            with Import, Convention => C, Link_Name => "init_function_1";
      begin
         C_Init;
      end Init;
   
      --  Protected provided interfaces (incl. Sporadic and cyclics)
      --  from templates/concurrency_view/ada_wrappers_body/pi.tmplt)
      protected body Function_1_Protected is
         
         procedure Pi_2
            (P1 : Interfaces.C.char_array; P1_Size : Integer) is
         
            procedure C_Pi_2
               (P1 : Interfaces.C.char_array; P1_Size : Integer)
         
              with Import, Convention => C, Link_Name => "function_1_PI_2";
         begin
            C_Pi_2
               (P1, P1_Size);
         end Pi_2;
         procedure Poll is
            procedure C_Poll
              with Import, Convention => C, Link_Name => "function_1_Poll";
         begin
            --  PI has no parameter - could directly call user code (add _PI_ above)
            C_Poll;
         end Poll;
      end Function_1_Protected;
      --  Unprotected provided interfaces
      --  from templates/concurrency_view/ada_wrappers_body/pi.tmplt
      
      --  End if unprotected interfaces
   
      --  Required interfaces (templates/concurrency_view/ada_wrappers_body/ri.tmplt)
      
      procedure Pi_1
         (P1 : Interfaces.C.char_array; P1_Size : Integer)
      is
      begin
         --  Sporadic call
         case Get_Task_Id is
            when demo_function_1_PI_2_k =>
               declare
                  Value : function_1_PI_2_Thread_function_1_PI_2_others_Interface
                     (function_1_PI_2_Thread_function_1_PI_2_others_Port_Type'(OUTPORT_Function_2_PI_1));
                  Err   : PolyORB_Hi.Errors.Error_Kind with Unreferenced;
                  use type PolyORB_HI.Errors.Error_Kind;
               begin
                  for I in 1 .. p1_Size loop
                     Value.OUTPORT_Function_2_PI_1_DATA.Buffer (I) :=
                        PolyORB_HI_Generated.Types.Stream_Element_Buffer (p1 (Interfaces.C.size_t (I - 1)));
                  end loop;
                  Value.OUTPORT_Function_2_PI_1_DATA.Length := PolyORB_HI_Generated.Types.Unsigned_32 (p1_Size);
                  Put_Value (demo_function_1_PI_2_k, Value);
                  Send_Output (demo_function_1_PI_2_k,
                               function_1_PI_2_Thread_function_1_PI_2_others_Port_Type'(OUTPORT_Function_2_PI_1),
                               Err);
                 --  TODO : Report the error if any
               end;
            when demo_function_1_Poll_k =>
               declare
                  Value : function_1_Poll_Thread_function_1_Poll_others_Interface
                     (function_1_Poll_Thread_function_1_Poll_others_Port_Type'(OUTPORT_Function_2_PI_1));
                  Err   : PolyORB_Hi.Errors.Error_Kind with Unreferenced;
                  use type PolyORB_HI.Errors.Error_Kind;
               begin
                  for I in 1 .. p1_Size loop
                     Value.OUTPORT_Function_2_PI_1_DATA.Buffer (I) :=
                        PolyORB_HI_Generated.Types.Stream_Element_Buffer (p1 (Interfaces.C.size_t (I - 1)));
                  end loop;
                  Value.OUTPORT_Function_2_PI_1_DATA.Length := PolyORB_HI_Generated.Types.Unsigned_32 (p1_Size);
                  Put_Value (demo_function_1_Poll_k, Value);
                  Send_Output (demo_function_1_Poll_k,
                               function_1_Poll_Thread_function_1_Poll_others_Port_Type'(OUTPORT_Function_2_PI_1),
                               Err);
                 --  TODO : Report the error if any
               end;
            when others => null;
         end case;
      end;
   
   end Function_1;
   --  The package body comes templates/concurrency_view/ada_wrappers_body/block.tmplt
   package body Function_2 is
   
      --  Initialization function is implemented in C (in vm_if.c)
      procedure Init is
         procedure C_Init
            with Import, Convention => C, Link_Name => "init_function_2";
      begin
         C_Init;
      end Init;
   
      --  Protected provided interfaces (incl. Sporadic and cyclics)
      --  from templates/concurrency_view/ada_wrappers_body/pi.tmplt)
      protected body Function_2_Protected is
         
         procedure Pi_1
            (P1 : Interfaces.C.char_array; P1_Size : Integer) is
         
            procedure C_Pi_1
               (P1 : Interfaces.C.char_array; P1_Size : Integer)
         
              with Import, Convention => C, Link_Name => "function_2_PI_1";
         begin
            C_Pi_1
               (P1, P1_Size);
         end Pi_1;
      end Function_2_Protected;
      --  Unprotected provided interfaces
      --  from templates/concurrency_view/ada_wrappers_body/pi.tmplt
      
      --  End if unprotected interfaces
   
      --  Required interfaces (templates/concurrency_view/ada_wrappers_body/ri.tmplt)
      
      procedure Pi_2
         (P1 : Interfaces.C.char_array; P1_Size : Integer)
      is
      begin
         --  Sporadic call
         case Get_Task_Id is
            when demo_function_2_PI_1_k =>
               declare
                  Value : function_2_PI_1_Thread_function_2_PI_1_others_Interface
                     (function_2_PI_1_Thread_function_2_PI_1_others_Port_Type'(OUTPORT_Function_1_PI_2));
                  Err   : PolyORB_Hi.Errors.Error_Kind with Unreferenced;
                  use type PolyORB_HI.Errors.Error_Kind;
               begin
                  for I in 1 .. p1_Size loop
                     Value.OUTPORT_Function_1_PI_2_DATA.Buffer (I) :=
                        PolyORB_HI_Generated.Types.Stream_Element_Buffer (p1 (Interfaces.C.size_t (I - 1)));
                  end loop;
                  Value.OUTPORT_Function_1_PI_2_DATA.Length := PolyORB_HI_Generated.Types.Unsigned_32 (p1_Size);
                  Put_Value (demo_function_2_PI_1_k, Value);
                  Send_Output (demo_function_2_PI_1_k,
                               function_2_PI_1_Thread_function_2_PI_1_others_Port_Type'(OUTPORT_Function_1_PI_2),
                               Err);
                 --  TODO : Report the error if any
               end;
            when others => null;
         end case;
      end;
   
     procedure Function_2_Check_Queue (Pending_Message : out Boolean) is
     begin
        -- Check all incoming queues for a pending message
        Pending_Message := False;
        if Get_Count (
          Entity => Demo_Function_2_Pi_1_K,
          Port   =>  Function_2_Pi_1_Thread_Function_2_Pi_1_others_Port_Type'(INPORT_Pi_1)) > 0
      then
         Pending_Message := True;
         return;
      end if;
     end Function_2_Check_Queue;
   
   end Function_2;
   
   procedure Function_1_Pi_2_Pi_2
      (dummy_Entity : Entity_Type; Param_Buffer : MyChoice_Buffer_Impl)
    is
      P1_AdaBuffer : Interfaces.C.char_array (1 .. Interfaces.C.size_t (Param_Buffer.Length)) with Import;
      for P1_AdaBuffer'Address use Param_Buffer'Address;
   begin
      Function_1.Function_1_Protected.Pi_2
         (P1_AdaBuffer, P1_AdaBuffer'Length);
   end Function_1_Pi_2_Pi_2;
   procedure Function_1_Poll_Poll
      (dummy_Entity : Entity_Type)
   is
   begin
      Function_1.Function_1_Protected.Poll;
   end Function_1_Poll_Poll;
   procedure Function_2_Pi_1_Pi_1
      (dummy_Entity : Entity_Type; Param_Buffer : MySeqOf_Buffer_Impl)
    is
      P1_AdaBuffer : Interfaces.C.char_array (1 .. Interfaces.C.size_t (Param_Buffer.Length)) with Import;
      for P1_AdaBuffer'Address use Param_Buffer'Address;
   begin
      Function_2.Function_2_Protected.Pi_1
         (P1_AdaBuffer, P1_AdaBuffer'Length);
   end Function_2_Pi_1_Pi_1;
end Demo_TASTE_Interface;
