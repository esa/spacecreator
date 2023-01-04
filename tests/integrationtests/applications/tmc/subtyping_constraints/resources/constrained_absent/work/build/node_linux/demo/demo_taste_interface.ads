with Interfaces.C;
with PolyORB_HI_Generated.Deployment;
use  PolyORB_HI_Generated.Deployment;
with PolyORB_HI_Generated.Types;
use  PolyORB_HI_Generated.Types;
with Function_2;
pragma Unreferenced (Function_2);

package Demo_TASTE_Interface is
   --  generated from templates/concurrency_view/ada_wrappers_source/partition.tmplt
   
   --  Package wrapping function Function_1 : manages the call to functions in vm_if.c
   --  Provides mutex to handle protected, cyclic and sporadic provided interfaces
   --  Exports the interface functions called by invoke_ri (vm_...)
   --  generated from templates/concurrency_view/ada_wrappers_source/block.tmplt
   package Function_1 is
   
      --  This package interfaces with the user code directly
      --  It may contain a protected object if a PI requires mutual exclusion
   
      procedure Init;
   
      --  Protected provided interfaces (incl. Sporadic and cyclics)
      --  from templates/concurrency_view/ada_wrappers_source/pi.tmplt)
      protected Function_1_Protected is
         
         procedure Pi_2
            (P1 : Interfaces.C.char_array; P1_Size : Integer);
         procedure Poll;
      end Function_1_Protected;
   
      --  Required interfaces (templates/concurrency_view/ada_wrappers_source/ri.tmplt)
      
      procedure Pi_1
         (P1 : Interfaces.C.char_array; P1_Size : Integer)
         with Export, Convention => C, Link_Name => "vm_function_1_pi_1";
   end Function_1;
   
   --  Package wrapping function Function_2 : manages the call to functions in vm_if.c
   --  Provides mutex to handle protected, cyclic and sporadic provided interfaces
   --  Exports the interface functions called by invoke_ri (vm_...)
   --  generated from templates/concurrency_view/ada_wrappers_source/block.tmplt
   package Function_2 is
   
      --  This package interfaces with the user code directly
      --  It may contain a protected object if a PI requires mutual exclusion
   
      procedure Init;
   
      --  Protected provided interfaces (incl. Sporadic and cyclics)
      --  from templates/concurrency_view/ada_wrappers_source/pi.tmplt)
      protected Function_2_Protected is
         
         procedure Pi_1
            (P1 : Interfaces.C.char_array; P1_Size : Integer);
      end Function_2_Protected;
   
      --  Required interfaces (templates/concurrency_view/ada_wrappers_source/ri.tmplt)
      
      procedure Pi_2
         (P1 : Interfaces.C.char_array; P1_Size : Integer)
         with Export, Convention => C, Link_Name => "vm_function_2_pi_2";
   
     -- Support for continuous signal for SDL function Function_2
     procedure Function_2_Check_Queue (Pending_Message : out Boolean)
        with Export, Convention => C, Link_Name => "function_2_check_queue";
   end Function_2;
   
   
   
   --  generated from templates/concurrency_view/ada_wrappers_source/thread.tmplt
   procedure Function_1_Pi_2_Init renames Function_1.Init;
   
   procedure Function_1_Pi_2_Pi_2
      (dummy_Entity : Entity_Type; Param_Buffer : MyChoice_Buffer_Impl);
   
   
   --  generated from templates/concurrency_view/ada_wrappers_source/thread.tmplt
   procedure Function_1_Poll_Init renames Function_1.Init;
   
   procedure Function_1_Poll_Poll
      (dummy_Entity : Entity_Type);
   
   
   --  generated from templates/concurrency_view/ada_wrappers_source/thread.tmplt
   procedure Function_2_Pi_1_Init renames Function_2.Init;
   
   procedure Function_2_Pi_1_Pi_1
      (dummy_Entity : Entity_Type; Param_Buffer : MySeqOf_Buffer_Impl);
   
end Demo_TASTE_Interface;
