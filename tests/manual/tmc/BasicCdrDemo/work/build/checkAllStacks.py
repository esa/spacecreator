#!/usr/bin/env python3

import os
import sys
from pathlib import Path

# This system contains the following nodes:
# node_linux

def name_converter(task_name: str):
    task_name_job = "Process_Package_" + task_name.title() + "_Job"
    return task_name_job


def report_node_linux():

   error_sentinel = [False]
   functions = []

   
   def report_partition_demo():
   
       print("[-] Checking callgraph files' presence in demo...")
   
       ci_files =[file.name.strip(' \n\'\"') for file in Path('.').rglob('*.ci')]
   
       if len(ci_files) == 0:
           print(f"[!]\033[93m WARNING: No *.ci files found in {os.path.abspath('.')}, using old stack analyzer.\033[00m")
           version = 9     # GCC == 9.X
       else:
           version = 10    # GCC >= 10.X
   
       print("[-] Checking stack usage of tasks inside demo...")
   
       def add_task(task_name):
           task_name_job = name_converter(task_name)
           functions.append(task_name_job)
   
       functions[:] = []
       add_task("actuator_ctrl")
       add_task("egse_poll")
       add_task("egse_tm")
       add_task("modemanager_feedback")
       add_task("modemanager_reading")
       add_task("modemanager_tc")
       add_task("sensor_poll")
   
       stack_usage = {}
       if version == 9:
           elf_binary = "../binaries/demo"
       else:
           elf_binary = ''  # binary not needed for new stack analyzer
   
       shift_offset = 0
       # On Linux, a constant named PTHREAD_STACK_MIN gives the minimum stack size for a thread
       # in addition to the user space stack. We have to add this offest to the computed value:
       shift_offset = 16384
       if (version == 9) and (not os.path.exists(elf_binary)):
           print("[x]\033[91m Failed to locate ELF binary:", elf_binary, "\033[00m")
           print("[x]\033[91m Aborting...\033[00m")
           sys.exit(1)
   
       for line in os.popen(f"FindStackUsageMain.py --gcc={version} --binary={elf_binary} --path=./node_linux --roots={','.join(functions)} --verbose"):
           data = line.strip().split(';;')
           try:
               if len(data) == 3:
                   stack_usage[data[1].split(':')[-1].strip()] = int(data[0]), data[2]
           except:
               pass
       pohic_stack_of = {
           
           "actuator_ctrl" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
           "egse_poll" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
           "egse_tm" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
           "modemanager_feedback" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
           "modemanager_reading" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
           "modemanager_tc" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
           "sensor_poll" : 8388608,  # Linux threads default to "ulimit -s" (8192K by default)
           
       }
   
       def check_task(task_name, version):
           task_name_job = name_converter(task_name)
           if version == 9:
               # To avoid exhausting cmd line length of c++filt,
               # work in batches of 100 symbols.
               keys = list(stack_usage.keys())
               noMangling = []
               subset_of_keys, rest_of_keys = keys[:100], keys[100:]
               while subset_of_keys:
                   batch = '"' + '" "'.join(subset_of_keys) + '"'
                   for result in os.popen(f'c++filt {batch}').readlines():
                       noMangling.append(result.strip('()\n'))
                   subset_of_keys, rest_of_keys = rest_of_keys[:100], rest_of_keys[100:]
               for idx, key in enumerate(list(keys)):
                   stack_usage[noMangling[idx]] = stack_usage.pop(key)
   
           elif version >= 10:
               pass
           else:
               raise ValueError(f"Invalid GCC Version: {version} ; Must be >= 9")
   
           if task_name not in pohic_stack_of:
               print("[!]\033[93m WARNING: Unable to check stack usage of", task_name_job, "\033[00m")
               return
   
           elif task_name_job not in stack_usage:
               print("[!]\033[93m WARNING: ", task_name_job, " is either recursive or missing\033[00m")
               return
   
           reserved_stack = pohic_stack_of[task_name]
           actual_stack, path = stack_usage[task_name_job]
           actual_stack += shift_offset
           if reserved_stack < actual_stack:
               print("[x]\033[91m ERROR: Stack usage of", task_name_job, "is", actual_stack,
                       "which is higher than", reserved_stack, "\033[00m")
               print("[x]\033[91m ERROR: Call chain is: ", path, "\033[00m")
               error_sentinel[0] = True
           else:
               print("[-]\033[92m Stack usage of %32s" % task_name_job, "is %7d /%7d \033[00m" % (
                   actual_stack, reserved_stack))
   
       check_task("actuator_ctrl", version)
       check_task("egse_poll", version)
       check_task("egse_tm", version)
       check_task("modemanager_feedback", version)
       check_task("modemanager_reading", version)
       check_task("modemanager_tc", version)
       check_task("sensor_poll", version)
   

   report_partition_demo()
   if error_sentinel[0]:
       sys.exit(1)

def check_all():
    report_node_linux()
    return

if __name__ == '__main__':
    ''' Run main application '''
    check_all()
