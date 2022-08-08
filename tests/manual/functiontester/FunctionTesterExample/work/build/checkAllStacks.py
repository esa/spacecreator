#!/usr/bin/env python3

import os
import sys

# This system contains the following nodes:
# node_linux


def report_node_linux():

   error_sentinel = [False]
   functions = []

   
   def report_partition_demo():
       print("[-] Checking stack usage of tasks inside demo...")
   
       def add_task(task_name):
           task_name_job = task_name + "_job"
           functions.append(task_name_job)
   
       functions[:] = []
       add_task("controller_trigger")
   
       stack_usage = {}
       elf_binary = "../binaries/demo"
       shift_offset = 0
       # On Linux, a constant named PTHREAD_STACK_MIN gives the minimum stack size for a thread
       # in addition to the user space stack. We have to add this offest to the computed value:
       shift_offset = 16384
       if not os.path.exists(elf_binary):
           print("[x] Failed to locate ELF binary:", elf_binary)
           print("[x] Aborting...")
           sys.exit(1)
       for line in os.popen("checkStackUsageGCCBased.py " + elf_binary + " ./node_linux " +
              '"' + '" "'.join(functions) + '"'):
           data = line.strip().split()
           try:
               if len(data) == 3:
                   stack_usage[data[1].strip()] = int(data[0][:-1]), data[2]
           except:
               pass
       pohic_stack_of = {
           
           "controller_trigger" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
       }
   
       def check_task(task_name):
           task_name_job = task_name + "_job"
   
           if task_name not in pohic_stack_of:
               print("[!] WARNING: Unable to check stack usage of", task_name+"_job")
           elif task_name_job not in stack_usage:
               print("[!] WARNING: ", task_name_job,
                     " is either recursive or missing in", elf_binary)
           else:
               reserved_stack = pohic_stack_of[task_name]
               actual_stack, path = stack_usage[task_name_job]
               actual_stack += shift_offset
               print("[-] Stack usage of %32s" % task_name, "is %7d /%7d" % (
                    actual_stack, reserved_stack))
               if reserved_stack < actual_stack:
                   print("[x] ERROR: Stack usage of", task_name, "is", actual_stack,
                         "which is higher than", reserved_stack)
                   print("[x] ERROR: Call chain is: ", path)
                   error_sentinel[0] = True
       check_task("controller_trigger")
   

   report_partition_demo()
   if error_sentinel[0]:
       sys.exit(1)

def check_all():
    report_node_linux()
    return

if __name__ == '__main__':
    ''' Run main application '''
    check_all()
