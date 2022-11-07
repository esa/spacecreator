#ifndef PAN_H
#define PAN_H

#define SpinVersion	"Spin Version 6.5.2 -- 9 June 2022 +  N7S Custom - Work-in-Progress"
#define PanSource	"system.pml"

#define G_long	8
#define G_int	4

#define ulong	unsigned long
#define ushort	unsigned short
#ifdef WIN64
	#define ONE_L	(1L)
/*	#define long	long long */
#else
	#define ONE_L	(1L)
#endif

#ifdef BFS_PAR
	#define NRUNS	0
	#ifndef BFS
		#define BFS
	#endif
	#ifndef PUTPID
		#define PUTPID
	#endif

	#if !defined(USE_TDH) && !defined(NO_TDH)
		#define USE_TDH
	#endif
	#if defined(USE_TDH) && !defined(NO_HC)
		#define HC /* default for USE_TDH */
	#endif
	#ifndef BFS_MAXPROCS
		#define BFS_MAXPROCS	64	/* max nr of cores to use */
	#endif
	#define BFS_GLOB	0	/* global lock */
	#define BFS_ORD		1	/* used with -DCOLLAPSE */
	#define BFS_MEM		2	/* malloc from shared heap */
	#define BFS_PRINT	3	/* protect printfs */
	#define BFS_STATE	4	/* hashtable */

	#define BFS_INQ 	2	/* state is in q */

	#ifdef BFS_FIFO
	  #define BFS_ID(a,b)	(BFS_STATE + (int) ((a)*BFS_MAXPROCS+(b)))
	  #define BFS_MAXLOCKS	(BFS_STATE + (BFS_MAXPROCS*BFS_MAXPROCS))
	#else
	  #ifndef BFS_W
		#define BFS_W	10
	  #endif
	  #define BFS_MASK	((1<<BFS_W) - 1)
	  #define BFS_ID	(BFS_STATE + (int) (j1_spin & (BFS_MASK)))
	  #define BFS_MAXLOCKS	(BFS_STATE + (1<<BFS_W))
	#endif
	#undef NCORE
	extern int Cores, who_am_i;
	#ifndef SAFETY
	  #if !defined(BFS_STAGGER) && !defined(BFS_DISK)
		#define BFS_STAGGER	64 /* randomizer, was 16 */
	  #endif
	  #ifndef L_BOUND
		#define L_BOUND 	10 /* default */
	  #endif
	  extern int L_bound;
	#endif
	#if defined(BFS_DISK) && defined(BFS_STAGGER)
		#error BFS_DISK and BFS_STAGGER are not compatible
	#endif
#endif

#if defined(BFS)
	#ifndef SAFETY
		#define SAFETY
	#endif
	#ifndef XUSAFE
		#define XUSAFE
	#endif
#endif
#ifndef uchar
	#define uchar	unsigned char
#endif
#ifndef uint
	#define uint	unsigned int
#endif
#define DELTA	500
#ifdef MA
	#if NCORE>1 && !defined(SEP_STATE)
		#define SEP_STATE
	#endif
	#if MA==1
		#undef MA
		#define MA	100
	#endif
#endif
#ifdef W_XPT
	#if W_XPT==1
		#undef W_XPT
		#define W_XPT 1000000
	#endif
#endif
#ifndef NFAIR
	#define NFAIR	2	/* must be >= 2 */
#endif
#define HAS_PRIORITY	7
#define HAS_CODE	1
#if defined(RANDSTORE) && !defined(RANDSTOR)
	#define RANDSTOR	RANDSTORE
#endif
#define HAS_ENABLED	1
#ifndef NOREDUCE
	#define NOREDUCE	1
#endif
#define MERGED	1
#if !defined(HAS_LAST) && defined(BCS)
	#define HAS_LAST	1 /* use it, but */
	#ifndef STORE_LAST
		#define NO_LAST	1 /* don't store it */
	#endif
#endif
#if defined(BCS) && defined(BITSTATE)
	#ifndef NO_CTX
		#define STORE_CTX	1
	#endif
#endif
#ifdef NP
	#define HAS_NP	2
	#define VERI	8	/* np_ */
#endif
#if defined(NOCLAIM) && defined(NP)
	#undef NOCLAIM
#endif

typedef struct S_F_MAP {
	char *fnm;
	int from;
	int upto;
} S_F_MAP;

#define _nstates7	204	/* :init: */
#define minseq7	366
#define maxseq7	568
#define _endstate7	203

#define _nstates6	62	/* Sensor_trigger */
#define minseq6	305
#define maxseq6	365
#define _endstate6	61

#define _nstates5	22	/* Sensor_tc */
#define minseq5	284
#define maxseq5	304
#define _endstate5	21

#define _nstates4	22	/* Host_tm */
#define minseq4	263
#define maxseq4	283
#define _endstate4	21

#define _nstates3	73	/* Controlsystem_reading */
#define minseq3	191
#define maxseq3	262
#define _endstate3	72

#define _nstates2	82	/* Controlsystem_configuration */
#define minseq2	110
#define maxseq2	190
#define _endstate2	81

#define _nstates1	97	/* Actuator_controlSignal */
#define minseq1	14
#define maxseq1	109
#define _endstate1	96

#define _nstates0	15	/* timer_manager_proc */
#define minseq0	0
#define maxseq0	13
#define _endstate0	14

extern short src_ln7[];
extern short src_ln6[];
extern short src_ln5[];
extern short src_ln4[];
extern short src_ln3[];
extern short src_ln2[];
extern short src_ln1[];
extern short src_ln0[];
extern S_F_MAP src_file7[];
extern S_F_MAP src_file6[];
extern S_F_MAP src_file5[];
extern S_F_MAP src_file4[];
extern S_F_MAP src_file3[];
extern S_F_MAP src_file2[];
extern S_F_MAP src_file1[];
extern S_F_MAP src_file0[];

#define T_ID	unsigned short
#define _T5	203
#define _T2	204
#define WS		8 /* word size in bytes */
#define SYNC	0
#define ASYNC	10

#ifndef NCORE
	#ifdef DUAL_CORE
		#define NCORE	2
	#elif QUAD_CORE
		#define NCORE	4
	#else
		#define NCORE	1
	#endif
#endif

struct Actuator_Context { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	float setting;
	float inputcontrol;
};
struct Actuator_Event_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Actuator_Event_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct AggregateTimerData_actuator { /* user defined type */
	unsigned dummy : 1;
};
struct AggregateTimerData_controlsystem { /* user defined type */
	unsigned dummy : 1;
};
struct AggregateTimerData_dummy_entry { /* user defined type */
	unsigned dummy : 1;
};
struct AggregateTimerData_host { /* user defined type */
	unsigned dummy : 1;
};
struct AggregateTimerData_sensor_trigger { /* user defined type */
	unsigned timer_enabled : 1;
	int interval;
};
struct Controlsystem_Context_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Controlsystem_Event_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Controlsystem_Event_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Controlsystem_Event_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Controlsystem_Event_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Controlsystem_Event_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_Timer_Manager_Event_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_Timer_Manager_Event_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_Timer_Manager_Event_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Demo_timer_manager_Context { /* user defined type */
	int state;
};
struct Events_Ty_elem_input_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Events_Ty_elem_input_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Events_Ty_elem_input_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_input_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Events_Ty_elem_no_event { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Events_Ty_elem_output_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Events_Ty_elem_output_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_output_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Events_Ty_elem_system_startup { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Events_Ty_elem_unhandled_input_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Events_Ty_elem_unhandled_input_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Function_Event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Function_Event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Function_Event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Function_Event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Function_Event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Function_Event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Function_Event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Host_Context_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Host_Event_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Host_Event_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Host_Event_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Host_Event_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct IPidConfig { /* user defined type */
	int kp;
	int ki;
	int kd;
};
struct Interface_Event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Interface_Event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Interface_Event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Interface_Event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Interface_Event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Interface_Event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Interface_Event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Named_Function_Event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Named_Function_Event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Named_Function_Event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Named_Function_Event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Named_Function_Event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Named_Function_Event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Named_Function_Event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Named_Function_Event_id { /* user defined type */
	uchar data[80];
	int length;
};
struct Observable_Event_input_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Observable_Event_input_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Observable_Event_input_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Observable_Event_input_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Observable_Event_input_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Observable_Event_input_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_input_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Observable_Event_no_event { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Observable_Event_output_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Observable_Event_output_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Observable_Event_output_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Observable_Event_output_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Observable_Event_output_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_output_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct Observable_Event_system_startup { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct Observable_Event_unhandled_input_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Observable_Event_unhandled_input_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Observable_Event_unhandled_input_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct PidConfig { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct Sensor_Context { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	float x;
};
struct Sensor_Event_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct Sensor_Event_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct Sensor_Event_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct Sensor_Event_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_actuator { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	float setting;
	float inputcontrol;
};
struct System_State_actuator_queue_elem_input_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_actuator_queue_elem_input_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_input_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_actuator_queue_elem_no_event { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_actuator_queue_elem_output_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_output_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_actuator_queue_elem_system_startup { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_controlsystem_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_controlsystem_queue_elem_no_event { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_controlsystem_queue_elem_system_startup { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_demo_timer_manager { /* user defined type */
	int state;
};
struct System_State_host_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_input_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_host_queue_elem_input_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_input_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_input_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_host_queue_elem_no_event { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_host_queue_elem_output_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_output_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_output_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_host_queue_elem_system_startup { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_host_queue_elem_unhandled_input_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_sensor { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	float x;
};
struct System_State_sensor_queue_elem_input_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_sensor_queue_elem_input_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_input_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_sensor_queue_elem_no_event { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_sensor_queue_elem_output_event_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_output_event_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_sensor_queue_elem_system_startup { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_reading { /* user defined type */
	float signalvalue;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal { /* user defined type */
	float controlvalue;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_in_tick { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_in_tm { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg { /* user defined type */
	float kp;
	float ki;
	float kd;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_input_none { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_tc { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_trigger { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_out_reading { /* user defined type */
	float signalvalue;
};
struct System_State_timers_actuator { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_timers_controlsystem { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_timers_dummy_entry { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_timers_host { /* user defined type */
	unsigned dummy : 1;
};
struct System_State_timers_sensor_trigger { /* user defined type */
	unsigned timer_enabled : 1;
	int interval;
};
struct T_Null_Record { /* user defined type */
	unsigned dummy : 1;
};
struct TimerData { /* user defined type */
	unsigned timer_enabled : 1;
	int interval;
};
union Actuator_Event_msg_in_data { /* user defined type */
	struct Actuator_Event_msg_in_input_none input_none;
	struct Actuator_Event_msg_in_controlsignal controlsignal;
};
struct AggregateTimerData_sensor { /* user defined type */
	struct AggregateTimerData_sensor_trigger trigger;
};
struct Controlsystem_Context { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	struct Controlsystem_Context_cfg cfg;
	float inputvalue;
	float outputvalue;
	float i;
	float d;
	float previous;
};
struct Controlsystem_Event_msg_in_configuration { /* user defined type */
	struct Controlsystem_Event_msg_in_configuration_cfg cfg;
};
union Controlsystem_Event_msg_in_data { /* user defined type */
	struct Controlsystem_Event_msg_in_input_none input_none;
	struct Controlsystem_Event_msg_in_configuration configuration;
	struct Controlsystem_Event_msg_in_reading reading;
};
union Controlsystem_Event_msg_out_data { /* user defined type */
	struct Controlsystem_Event_msg_out_controlsignal controlsignal;
	struct Controlsystem_Event_msg_out_tm tm;
};
union Demo_Timer_Manager_Event_msg_in_data { /* user defined type */
	struct Demo_Timer_Manager_Event_msg_in_input_none input_none;
	struct Demo_Timer_Manager_Event_msg_in_tick tick;
};
union Demo_Timer_Manager_Event_msg_out_data { /* user defined type */
	struct Demo_Timer_Manager_Event_msg_out_sensor_trigger sensor_trigger;
};
union Demo_Timer_Manager_Periodic_Events_elem_event_msg_in_data { /* user defined type */
	struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_in_input_none input_none;
	struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_in_tick tick;
};
union Demo_Timer_Manager_Periodic_Events_elem_event_msg_out_data { /* user defined type */
	struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_out_sensor_trigger sensor_trigger;
};
union Events_Ty_elem_input_event_event_actuator_msg_in_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_actuator_msg_in_input_none input_none;
	struct Events_Ty_elem_input_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Events_Ty_elem_input_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Events_Ty_elem_input_event_event_controlsystem_msg_in_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_controlsystem_msg_in_input_none input_none;
	struct Events_Ty_elem_input_event_event_controlsystem_msg_in_configuration configuration;
	struct Events_Ty_elem_input_event_event_controlsystem_msg_in_reading reading;
};
union Events_Ty_elem_input_event_event_controlsystem_msg_out_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Events_Ty_elem_input_event_event_controlsystem_msg_out_tm tm;
};
union Events_Ty_elem_input_event_event_env_msg_in_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_env_msg_in_input_none input_none;
	struct Events_Ty_elem_input_event_event_env_msg_in_tick tick;
};
union Events_Ty_elem_input_event_event_env_msg_out_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Events_Ty_elem_input_event_event_host_msg_in_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_host_msg_in_input_none input_none;
	struct Events_Ty_elem_input_event_event_host_msg_in_tm tm;
};
struct Events_Ty_elem_input_event_event_host_msg_out_configuration { /* user defined type */
	struct Events_Ty_elem_input_event_event_host_msg_out_configuration_cfg cfg;
};
union Events_Ty_elem_input_event_event_host_msg_out_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_host_msg_out_configuration configuration;
	struct Events_Ty_elem_input_event_event_host_msg_out_tc tc;
};
union Events_Ty_elem_input_event_event_sensor_msg_in_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_sensor_msg_in_input_none input_none;
	struct Events_Ty_elem_input_event_event_sensor_msg_in_tc tc;
	struct Events_Ty_elem_input_event_event_sensor_msg_in_trigger trigger;
};
union Events_Ty_elem_input_event_event_sensor_msg_out_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_sensor_msg_out_reading reading;
};
union Events_Ty_elem_output_event_event_actuator_msg_in_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_actuator_msg_in_input_none input_none;
	struct Events_Ty_elem_output_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Events_Ty_elem_output_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Events_Ty_elem_output_event_event_controlsystem_msg_in_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_controlsystem_msg_in_input_none input_none;
	struct Events_Ty_elem_output_event_event_controlsystem_msg_in_configuration configuration;
	struct Events_Ty_elem_output_event_event_controlsystem_msg_in_reading reading;
};
union Events_Ty_elem_output_event_event_controlsystem_msg_out_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Events_Ty_elem_output_event_event_controlsystem_msg_out_tm tm;
};
union Events_Ty_elem_output_event_event_env_msg_in_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_env_msg_in_input_none input_none;
	struct Events_Ty_elem_output_event_event_env_msg_in_tick tick;
};
union Events_Ty_elem_output_event_event_env_msg_out_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Events_Ty_elem_output_event_event_host_msg_in_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_host_msg_in_input_none input_none;
	struct Events_Ty_elem_output_event_event_host_msg_in_tm tm;
};
struct Events_Ty_elem_output_event_event_host_msg_out_configuration { /* user defined type */
	struct Events_Ty_elem_output_event_event_host_msg_out_configuration_cfg cfg;
};
union Events_Ty_elem_output_event_event_host_msg_out_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_host_msg_out_configuration configuration;
	struct Events_Ty_elem_output_event_event_host_msg_out_tc tc;
};
union Events_Ty_elem_output_event_event_sensor_msg_in_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_sensor_msg_in_input_none input_none;
	struct Events_Ty_elem_output_event_event_sensor_msg_in_tc tc;
	struct Events_Ty_elem_output_event_event_sensor_msg_in_trigger trigger;
};
union Events_Ty_elem_output_event_event_sensor_msg_out_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_sensor_msg_out_reading reading;
};
union Events_Ty_elem_unhandled_input_event_actuator_msg_in_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_actuator_msg_in_input_none input_none;
	struct Events_Ty_elem_unhandled_input_event_actuator_msg_in_controlsignal controlsignal;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_input_none input_none;
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_configuration configuration;
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_reading reading;
};
union Events_Ty_elem_unhandled_input_event_controlsystem_msg_out_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_out_tm tm;
};
union Events_Ty_elem_unhandled_input_event_env_msg_in_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_env_msg_in_input_none input_none;
	struct Events_Ty_elem_unhandled_input_event_env_msg_in_tick tick;
};
union Events_Ty_elem_unhandled_input_event_env_msg_out_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Events_Ty_elem_unhandled_input_event_host_msg_in_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_host_msg_in_input_none input_none;
	struct Events_Ty_elem_unhandled_input_event_host_msg_in_tm tm;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_out_configuration { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_host_msg_out_configuration_cfg cfg;
};
union Events_Ty_elem_unhandled_input_event_host_msg_out_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_host_msg_out_configuration configuration;
	struct Events_Ty_elem_unhandled_input_event_host_msg_out_tc tc;
};
union Events_Ty_elem_unhandled_input_event_sensor_msg_in_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_sensor_msg_in_input_none input_none;
	struct Events_Ty_elem_unhandled_input_event_sensor_msg_in_tc tc;
	struct Events_Ty_elem_unhandled_input_event_sensor_msg_in_trigger trigger;
};
union Events_Ty_elem_unhandled_input_event_sensor_msg_out_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_sensor_msg_out_reading reading;
};
union Function_Event_actuator_msg_in_data { /* user defined type */
	struct Function_Event_actuator_msg_in_input_none input_none;
	struct Function_Event_actuator_msg_in_controlsignal controlsignal;
};
struct Function_Event_controlsystem_msg_in_configuration { /* user defined type */
	struct Function_Event_controlsystem_msg_in_configuration_cfg cfg;
};
union Function_Event_controlsystem_msg_in_data { /* user defined type */
	struct Function_Event_controlsystem_msg_in_input_none input_none;
	struct Function_Event_controlsystem_msg_in_configuration configuration;
	struct Function_Event_controlsystem_msg_in_reading reading;
};
union Function_Event_controlsystem_msg_out_data { /* user defined type */
	struct Function_Event_controlsystem_msg_out_controlsignal controlsignal;
	struct Function_Event_controlsystem_msg_out_tm tm;
};
union Function_Event_env_msg_in_data { /* user defined type */
	struct Function_Event_env_msg_in_input_none input_none;
	struct Function_Event_env_msg_in_tick tick;
};
union Function_Event_env_msg_out_data { /* user defined type */
	struct Function_Event_env_msg_out_sensor_trigger sensor_trigger;
};
union Function_Event_host_msg_in_data { /* user defined type */
	struct Function_Event_host_msg_in_input_none input_none;
	struct Function_Event_host_msg_in_tm tm;
};
struct Function_Event_host_msg_out_configuration { /* user defined type */
	struct Function_Event_host_msg_out_configuration_cfg cfg;
};
union Function_Event_host_msg_out_data { /* user defined type */
	struct Function_Event_host_msg_out_configuration configuration;
	struct Function_Event_host_msg_out_tc tc;
};
union Function_Event_sensor_msg_in_data { /* user defined type */
	struct Function_Event_sensor_msg_in_input_none input_none;
	struct Function_Event_sensor_msg_in_tc tc;
	struct Function_Event_sensor_msg_in_trigger trigger;
};
union Function_Event_sensor_msg_out_data { /* user defined type */
	struct Function_Event_sensor_msg_out_reading reading;
};
struct Host_Context { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	struct Host_Context_cfg cfg;
};
union Host_Event_msg_in_data { /* user defined type */
	struct Host_Event_msg_in_input_none input_none;
	struct Host_Event_msg_in_tm tm;
};
struct Host_Event_msg_out_configuration { /* user defined type */
	struct Host_Event_msg_out_configuration_cfg cfg;
};
union Host_Event_msg_out_data { /* user defined type */
	struct Host_Event_msg_out_configuration configuration;
	struct Host_Event_msg_out_tc tc;
};
union Interface_Event_event_actuator_msg_in_data { /* user defined type */
	struct Interface_Event_event_actuator_msg_in_input_none input_none;
	struct Interface_Event_event_actuator_msg_in_controlsignal controlsignal;
};
struct Interface_Event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Interface_Event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Interface_Event_event_controlsystem_msg_in_data { /* user defined type */
	struct Interface_Event_event_controlsystem_msg_in_input_none input_none;
	struct Interface_Event_event_controlsystem_msg_in_configuration configuration;
	struct Interface_Event_event_controlsystem_msg_in_reading reading;
};
union Interface_Event_event_controlsystem_msg_out_data { /* user defined type */
	struct Interface_Event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Interface_Event_event_controlsystem_msg_out_tm tm;
};
union Interface_Event_event_env_msg_in_data { /* user defined type */
	struct Interface_Event_event_env_msg_in_input_none input_none;
	struct Interface_Event_event_env_msg_in_tick tick;
};
union Interface_Event_event_env_msg_out_data { /* user defined type */
	struct Interface_Event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Interface_Event_event_host_msg_in_data { /* user defined type */
	struct Interface_Event_event_host_msg_in_input_none input_none;
	struct Interface_Event_event_host_msg_in_tm tm;
};
struct Interface_Event_event_host_msg_out_configuration { /* user defined type */
	struct Interface_Event_event_host_msg_out_configuration_cfg cfg;
};
union Interface_Event_event_host_msg_out_data { /* user defined type */
	struct Interface_Event_event_host_msg_out_configuration configuration;
	struct Interface_Event_event_host_msg_out_tc tc;
};
union Interface_Event_event_sensor_msg_in_data { /* user defined type */
	struct Interface_Event_event_sensor_msg_in_input_none input_none;
	struct Interface_Event_event_sensor_msg_in_tc tc;
	struct Interface_Event_event_sensor_msg_in_trigger trigger;
};
union Interface_Event_event_sensor_msg_out_data { /* user defined type */
	struct Interface_Event_event_sensor_msg_out_reading reading;
};
union Named_Function_Event_event_actuator_msg_in_data { /* user defined type */
	struct Named_Function_Event_event_actuator_msg_in_input_none input_none;
	struct Named_Function_Event_event_actuator_msg_in_controlsignal controlsignal;
};
struct Named_Function_Event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Named_Function_Event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Named_Function_Event_event_controlsystem_msg_in_data { /* user defined type */
	struct Named_Function_Event_event_controlsystem_msg_in_input_none input_none;
	struct Named_Function_Event_event_controlsystem_msg_in_configuration configuration;
	struct Named_Function_Event_event_controlsystem_msg_in_reading reading;
};
union Named_Function_Event_event_controlsystem_msg_out_data { /* user defined type */
	struct Named_Function_Event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Named_Function_Event_event_controlsystem_msg_out_tm tm;
};
union Named_Function_Event_event_env_msg_in_data { /* user defined type */
	struct Named_Function_Event_event_env_msg_in_input_none input_none;
	struct Named_Function_Event_event_env_msg_in_tick tick;
};
union Named_Function_Event_event_env_msg_out_data { /* user defined type */
	struct Named_Function_Event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Named_Function_Event_event_host_msg_in_data { /* user defined type */
	struct Named_Function_Event_event_host_msg_in_input_none input_none;
	struct Named_Function_Event_event_host_msg_in_tm tm;
};
struct Named_Function_Event_event_host_msg_out_configuration { /* user defined type */
	struct Named_Function_Event_event_host_msg_out_configuration_cfg cfg;
};
union Named_Function_Event_event_host_msg_out_data { /* user defined type */
	struct Named_Function_Event_event_host_msg_out_configuration configuration;
	struct Named_Function_Event_event_host_msg_out_tc tc;
};
union Named_Function_Event_event_sensor_msg_in_data { /* user defined type */
	struct Named_Function_Event_event_sensor_msg_in_input_none input_none;
	struct Named_Function_Event_event_sensor_msg_in_tc tc;
	struct Named_Function_Event_event_sensor_msg_in_trigger trigger;
};
union Named_Function_Event_event_sensor_msg_out_data { /* user defined type */
	struct Named_Function_Event_event_sensor_msg_out_reading reading;
};
union Observable_Event_input_event_event_actuator_msg_in_data { /* user defined type */
	struct Observable_Event_input_event_event_actuator_msg_in_input_none input_none;
	struct Observable_Event_input_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct Observable_Event_input_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Observable_Event_input_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Observable_Event_input_event_event_controlsystem_msg_in_data { /* user defined type */
	struct Observable_Event_input_event_event_controlsystem_msg_in_input_none input_none;
	struct Observable_Event_input_event_event_controlsystem_msg_in_configuration configuration;
	struct Observable_Event_input_event_event_controlsystem_msg_in_reading reading;
};
union Observable_Event_input_event_event_controlsystem_msg_out_data { /* user defined type */
	struct Observable_Event_input_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Observable_Event_input_event_event_controlsystem_msg_out_tm tm;
};
union Observable_Event_input_event_event_env_msg_in_data { /* user defined type */
	struct Observable_Event_input_event_event_env_msg_in_input_none input_none;
	struct Observable_Event_input_event_event_env_msg_in_tick tick;
};
union Observable_Event_input_event_event_env_msg_out_data { /* user defined type */
	struct Observable_Event_input_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Observable_Event_input_event_event_host_msg_in_data { /* user defined type */
	struct Observable_Event_input_event_event_host_msg_in_input_none input_none;
	struct Observable_Event_input_event_event_host_msg_in_tm tm;
};
struct Observable_Event_input_event_event_host_msg_out_configuration { /* user defined type */
	struct Observable_Event_input_event_event_host_msg_out_configuration_cfg cfg;
};
union Observable_Event_input_event_event_host_msg_out_data { /* user defined type */
	struct Observable_Event_input_event_event_host_msg_out_configuration configuration;
	struct Observable_Event_input_event_event_host_msg_out_tc tc;
};
union Observable_Event_input_event_event_sensor_msg_in_data { /* user defined type */
	struct Observable_Event_input_event_event_sensor_msg_in_input_none input_none;
	struct Observable_Event_input_event_event_sensor_msg_in_tc tc;
	struct Observable_Event_input_event_event_sensor_msg_in_trigger trigger;
};
union Observable_Event_input_event_event_sensor_msg_out_data { /* user defined type */
	struct Observable_Event_input_event_event_sensor_msg_out_reading reading;
};
union Observable_Event_output_event_event_actuator_msg_in_data { /* user defined type */
	struct Observable_Event_output_event_event_actuator_msg_in_input_none input_none;
	struct Observable_Event_output_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct Observable_Event_output_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Observable_Event_output_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Observable_Event_output_event_event_controlsystem_msg_in_data { /* user defined type */
	struct Observable_Event_output_event_event_controlsystem_msg_in_input_none input_none;
	struct Observable_Event_output_event_event_controlsystem_msg_in_configuration configuration;
	struct Observable_Event_output_event_event_controlsystem_msg_in_reading reading;
};
union Observable_Event_output_event_event_controlsystem_msg_out_data { /* user defined type */
	struct Observable_Event_output_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Observable_Event_output_event_event_controlsystem_msg_out_tm tm;
};
union Observable_Event_output_event_event_env_msg_in_data { /* user defined type */
	struct Observable_Event_output_event_event_env_msg_in_input_none input_none;
	struct Observable_Event_output_event_event_env_msg_in_tick tick;
};
union Observable_Event_output_event_event_env_msg_out_data { /* user defined type */
	struct Observable_Event_output_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Observable_Event_output_event_event_host_msg_in_data { /* user defined type */
	struct Observable_Event_output_event_event_host_msg_in_input_none input_none;
	struct Observable_Event_output_event_event_host_msg_in_tm tm;
};
struct Observable_Event_output_event_event_host_msg_out_configuration { /* user defined type */
	struct Observable_Event_output_event_event_host_msg_out_configuration_cfg cfg;
};
union Observable_Event_output_event_event_host_msg_out_data { /* user defined type */
	struct Observable_Event_output_event_event_host_msg_out_configuration configuration;
	struct Observable_Event_output_event_event_host_msg_out_tc tc;
};
union Observable_Event_output_event_event_sensor_msg_in_data { /* user defined type */
	struct Observable_Event_output_event_event_sensor_msg_in_input_none input_none;
	struct Observable_Event_output_event_event_sensor_msg_in_tc tc;
	struct Observable_Event_output_event_event_sensor_msg_in_trigger trigger;
};
union Observable_Event_output_event_event_sensor_msg_out_data { /* user defined type */
	struct Observable_Event_output_event_event_sensor_msg_out_reading reading;
};
union Observable_Event_unhandled_input_event_actuator_msg_in_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_actuator_msg_in_input_none input_none;
	struct Observable_Event_unhandled_input_event_actuator_msg_in_controlsignal controlsignal;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_in_configuration { /* user defined type */
	struct Observable_Event_unhandled_input_event_controlsystem_msg_in_configuration_cfg cfg;
};
union Observable_Event_unhandled_input_event_controlsystem_msg_in_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_controlsystem_msg_in_input_none input_none;
	struct Observable_Event_unhandled_input_event_controlsystem_msg_in_configuration configuration;
	struct Observable_Event_unhandled_input_event_controlsystem_msg_in_reading reading;
};
union Observable_Event_unhandled_input_event_controlsystem_msg_out_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_controlsystem_msg_out_controlsignal controlsignal;
	struct Observable_Event_unhandled_input_event_controlsystem_msg_out_tm tm;
};
union Observable_Event_unhandled_input_event_env_msg_in_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_env_msg_in_input_none input_none;
	struct Observable_Event_unhandled_input_event_env_msg_in_tick tick;
};
union Observable_Event_unhandled_input_event_env_msg_out_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_env_msg_out_sensor_trigger sensor_trigger;
};
union Observable_Event_unhandled_input_event_host_msg_in_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_host_msg_in_input_none input_none;
	struct Observable_Event_unhandled_input_event_host_msg_in_tm tm;
};
struct Observable_Event_unhandled_input_event_host_msg_out_configuration { /* user defined type */
	struct Observable_Event_unhandled_input_event_host_msg_out_configuration_cfg cfg;
};
union Observable_Event_unhandled_input_event_host_msg_out_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_host_msg_out_configuration configuration;
	struct Observable_Event_unhandled_input_event_host_msg_out_tc tc;
};
union Observable_Event_unhandled_input_event_sensor_msg_in_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_sensor_msg_in_input_none input_none;
	struct Observable_Event_unhandled_input_event_sensor_msg_in_tc tc;
	struct Observable_Event_unhandled_input_event_sensor_msg_in_trigger trigger;
};
union Observable_Event_unhandled_input_event_sensor_msg_out_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_sensor_msg_out_reading reading;
};
union Sensor_Event_msg_in_data { /* user defined type */
	struct Sensor_Event_msg_in_input_none input_none;
	struct Sensor_Event_msg_in_tc tc;
	struct Sensor_Event_msg_in_trigger trigger;
};
union Sensor_Event_msg_out_data { /* user defined type */
	struct Sensor_Event_msg_out_reading reading;
};
union System_State_actuator_queue_elem_input_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_actuator_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_input_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_reading reading;
};
union System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out_tm tm;
};
union System_State_actuator_queue_elem_input_event_event_env_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_env_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_input_event_event_env_msg_in_tick tick;
};
union System_State_actuator_queue_elem_input_event_event_env_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_actuator_queue_elem_input_event_event_host_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_host_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_input_event_event_host_msg_in_tm tm;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_actuator_queue_elem_input_event_event_host_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_host_msg_out_configuration configuration;
	struct System_State_actuator_queue_elem_input_event_event_host_msg_out_tc tc;
};
union System_State_actuator_queue_elem_input_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in_tc tc;
	struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in_trigger trigger;
};
union System_State_actuator_queue_elem_input_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_sensor_msg_out_reading reading;
};
union System_State_actuator_queue_elem_output_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_actuator_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_output_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_reading reading;
};
union System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out_tm tm;
};
union System_State_actuator_queue_elem_output_event_event_env_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_env_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_output_event_event_env_msg_in_tick tick;
};
union System_State_actuator_queue_elem_output_event_event_env_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_actuator_queue_elem_output_event_event_host_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_host_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_output_event_event_host_msg_in_tm tm;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_actuator_queue_elem_output_event_event_host_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_host_msg_out_configuration configuration;
	struct System_State_actuator_queue_elem_output_event_event_host_msg_out_tc tc;
};
union System_State_actuator_queue_elem_output_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in_tc tc;
	struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in_trigger trigger;
};
union System_State_actuator_queue_elem_output_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_sensor_msg_out_reading reading;
};
union System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration configuration;
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_reading reading;
};
union System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out_tm tm;
};
union System_State_actuator_queue_elem_unhandled_input_event_env_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_in_tick tick;
};
union System_State_actuator_queue_elem_unhandled_input_event_env_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_actuator_queue_elem_unhandled_input_event_host_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_in_tm tm;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_configuration { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg cfg;
};
union System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_configuration configuration;
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_tc tc;
};
union System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_input_none input_none;
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_tc tc;
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_trigger trigger;
};
union System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_out_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_out_reading reading;
};
struct System_State_controlsystem { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	struct System_State_controlsystem_cfg cfg;
	float inputvalue;
	float outputvalue;
	float i;
	float d;
	float previous;
};
union System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_reading reading;
};
union System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out_tm tm;
};
union System_State_controlsystem_queue_elem_input_event_event_env_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_env_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_input_event_event_env_msg_in_tick tick;
};
union System_State_controlsystem_queue_elem_input_event_event_env_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_controlsystem_queue_elem_input_event_event_host_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_in_tm tm;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_controlsystem_queue_elem_input_event_event_host_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out_configuration configuration;
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out_tc tc;
};
union System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_tc tc;
	struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_trigger trigger;
};
union System_State_controlsystem_queue_elem_input_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_out_reading reading;
};
union System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_reading reading;
};
union System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out_tm tm;
};
union System_State_controlsystem_queue_elem_output_event_event_env_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_env_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_output_event_event_env_msg_in_tick tick;
};
union System_State_controlsystem_queue_elem_output_event_event_env_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_controlsystem_queue_elem_output_event_event_host_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_in_tm tm;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_controlsystem_queue_elem_output_event_event_host_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out_configuration configuration;
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out_tc tc;
};
union System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_tc tc;
	struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_trigger trigger;
};
union System_State_controlsystem_queue_elem_output_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_out_reading reading;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration configuration;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_reading reading;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out_tm tm;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in_tick tick;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in_tm tm;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_configuration { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg cfg;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_configuration configuration;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_tc tc;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_input_none input_none;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_tc tc;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_trigger trigger;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_out_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_out_reading reading;
};
struct System_State_host { /* user defined type */
	int state;
	unsigned init_done : 1;
	int sender;
	struct System_State_host_cfg cfg;
};
union System_State_host_queue_elem_input_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_actuator_msg_in_input_none input_none;
	struct System_State_host_queue_elem_input_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_host_queue_elem_input_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in_reading reading;
};
union System_State_host_queue_elem_input_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_out_tm tm;
};
union System_State_host_queue_elem_input_event_event_env_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_env_msg_in_input_none input_none;
	struct System_State_host_queue_elem_input_event_event_env_msg_in_tick tick;
};
union System_State_host_queue_elem_input_event_event_env_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_host_queue_elem_input_event_event_host_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_host_msg_in_input_none input_none;
	struct System_State_host_queue_elem_input_event_event_host_msg_in_tm tm;
};
struct System_State_host_queue_elem_input_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_host_queue_elem_input_event_event_host_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_host_msg_out_configuration configuration;
	struct System_State_host_queue_elem_input_event_event_host_msg_out_tc tc;
};
union System_State_host_queue_elem_input_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_sensor_msg_in_input_none input_none;
	struct System_State_host_queue_elem_input_event_event_sensor_msg_in_tc tc;
	struct System_State_host_queue_elem_input_event_event_sensor_msg_in_trigger trigger;
};
union System_State_host_queue_elem_input_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_sensor_msg_out_reading reading;
};
union System_State_host_queue_elem_output_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_actuator_msg_in_input_none input_none;
	struct System_State_host_queue_elem_output_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_host_queue_elem_output_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in_reading reading;
};
union System_State_host_queue_elem_output_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_out_tm tm;
};
union System_State_host_queue_elem_output_event_event_env_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_env_msg_in_input_none input_none;
	struct System_State_host_queue_elem_output_event_event_env_msg_in_tick tick;
};
union System_State_host_queue_elem_output_event_event_env_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_host_queue_elem_output_event_event_host_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_host_msg_in_input_none input_none;
	struct System_State_host_queue_elem_output_event_event_host_msg_in_tm tm;
};
struct System_State_host_queue_elem_output_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_host_queue_elem_output_event_event_host_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_host_msg_out_configuration configuration;
	struct System_State_host_queue_elem_output_event_event_host_msg_out_tc tc;
};
union System_State_host_queue_elem_output_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_sensor_msg_in_input_none input_none;
	struct System_State_host_queue_elem_output_event_event_sensor_msg_in_tc tc;
	struct System_State_host_queue_elem_output_event_event_sensor_msg_in_trigger trigger;
};
union System_State_host_queue_elem_output_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_sensor_msg_out_reading reading;
};
union System_State_host_queue_elem_unhandled_input_event_actuator_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_actuator_msg_in_input_none input_none;
	struct System_State_host_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none input_none;
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration configuration;
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_reading reading;
};
union System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out_tm tm;
};
union System_State_host_queue_elem_unhandled_input_event_env_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_env_msg_in_input_none input_none;
	struct System_State_host_queue_elem_unhandled_input_event_env_msg_in_tick tick;
};
union System_State_host_queue_elem_unhandled_input_event_env_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_host_queue_elem_unhandled_input_event_host_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_in_input_none input_none;
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_in_tm tm;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_out_configuration { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg cfg;
};
union System_State_host_queue_elem_unhandled_input_event_host_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_out_configuration configuration;
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_out_tc tc;
};
union System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_input_none input_none;
	struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_tc tc;
	struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_trigger trigger;
};
union System_State_host_queue_elem_unhandled_input_event_sensor_msg_out_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_out_reading reading;
};
union System_State_sensor_queue_elem_input_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_actuator_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_input_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_reading reading;
};
union System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out_tm tm;
};
union System_State_sensor_queue_elem_input_event_event_env_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_env_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_input_event_event_env_msg_in_tick tick;
};
union System_State_sensor_queue_elem_input_event_event_env_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_sensor_queue_elem_input_event_event_host_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_host_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_input_event_event_host_msg_in_tm tm;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_sensor_queue_elem_input_event_event_host_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_host_msg_out_configuration configuration;
	struct System_State_sensor_queue_elem_input_event_event_host_msg_out_tc tc;
};
union System_State_sensor_queue_elem_input_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in_tc tc;
	struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in_trigger trigger;
};
union System_State_sensor_queue_elem_input_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_sensor_msg_out_reading reading;
};
union System_State_sensor_queue_elem_output_event_event_actuator_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_actuator_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_output_event_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_configuration configuration;
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_reading reading;
};
union System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out_tm tm;
};
union System_State_sensor_queue_elem_output_event_event_env_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_env_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_output_event_event_env_msg_in_tick tick;
};
union System_State_sensor_queue_elem_output_event_event_env_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_sensor_queue_elem_output_event_event_host_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_host_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_output_event_event_host_msg_in_tm tm;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_out_configuration { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_host_msg_out_configuration_cfg cfg;
};
union System_State_sensor_queue_elem_output_event_event_host_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_host_msg_out_configuration configuration;
	struct System_State_sensor_queue_elem_output_event_event_host_msg_out_tc tc;
};
union System_State_sensor_queue_elem_output_event_event_sensor_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in_tc tc;
	struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in_trigger trigger;
};
union System_State_sensor_queue_elem_output_event_event_sensor_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_sensor_msg_out_reading reading;
};
union System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in_controlsignal controlsignal;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration_cfg cfg;
};
union System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_configuration configuration;
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_reading reading;
};
union System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out_controlsignal controlsignal;
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out_tm tm;
};
union System_State_sensor_queue_elem_unhandled_input_event_env_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_in_tick tick;
};
union System_State_sensor_queue_elem_unhandled_input_event_env_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_out_sensor_trigger sensor_trigger;
};
union System_State_sensor_queue_elem_unhandled_input_event_host_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_in_tm tm;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_configuration { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_configuration_cfg cfg;
};
union System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_configuration configuration;
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_tc tc;
};
union System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_input_none input_none;
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_tc tc;
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_trigger trigger;
};
union System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_out_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_out_reading reading;
};
struct System_State_timers_sensor { /* user defined type */
	struct System_State_timers_sensor_trigger trigger;
};
struct Actuator_Event_msg_in { /* user defined type */
	union Actuator_Event_msg_in_data data;
	int selection;
};
struct AggregateTimerData { /* user defined type */
	struct AggregateTimerData_actuator actuator;
	struct AggregateTimerData_controlsystem controlsystem;
	struct AggregateTimerData_host host;
	struct AggregateTimerData_sensor sensor;
	struct AggregateTimerData_dummy_entry dummy_entry;
};
struct Controlsystem_Event_msg_in { /* user defined type */
	union Controlsystem_Event_msg_in_data data;
	int selection;
};
struct Controlsystem_Event_msg_out { /* user defined type */
	union Controlsystem_Event_msg_out_data data;
	int selection;
};
struct Demo_Timer_Manager_Event_msg_in { /* user defined type */
	union Demo_Timer_Manager_Event_msg_in_data data;
	int selection;
};
struct Demo_Timer_Manager_Event_msg_out { /* user defined type */
	union Demo_Timer_Manager_Event_msg_out_data data;
	int selection;
};
struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_in { /* user defined type */
	union Demo_Timer_Manager_Periodic_Events_elem_event_msg_in_data data;
	int selection;
};
struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_out { /* user defined type */
	union Demo_Timer_Manager_Periodic_Events_elem_event_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_actuator_msg_in { /* user defined type */
	union Events_Ty_elem_input_event_event_actuator_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_in { /* user defined type */
	union Events_Ty_elem_input_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_controlsystem_msg_out { /* user defined type */
	union Events_Ty_elem_input_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_env_msg_in { /* user defined type */
	union Events_Ty_elem_input_event_event_env_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_env_msg_out { /* user defined type */
	union Events_Ty_elem_input_event_event_env_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_host_msg_in { /* user defined type */
	union Events_Ty_elem_input_event_event_host_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_host_msg_out { /* user defined type */
	union Events_Ty_elem_input_event_event_host_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_sensor_msg_in { /* user defined type */
	union Events_Ty_elem_input_event_event_sensor_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_sensor_msg_out { /* user defined type */
	union Events_Ty_elem_input_event_event_sensor_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_actuator_msg_in { /* user defined type */
	union Events_Ty_elem_output_event_event_actuator_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_in { /* user defined type */
	union Events_Ty_elem_output_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_controlsystem_msg_out { /* user defined type */
	union Events_Ty_elem_output_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_env_msg_in { /* user defined type */
	union Events_Ty_elem_output_event_event_env_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_env_msg_out { /* user defined type */
	union Events_Ty_elem_output_event_event_env_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_host_msg_in { /* user defined type */
	union Events_Ty_elem_output_event_event_host_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_host_msg_out { /* user defined type */
	union Events_Ty_elem_output_event_event_host_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_sensor_msg_in { /* user defined type */
	union Events_Ty_elem_output_event_event_sensor_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_sensor_msg_out { /* user defined type */
	union Events_Ty_elem_output_event_event_sensor_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_actuator_msg_in { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_actuator_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_controlsystem_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_out { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_controlsystem_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_env_msg_in { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_env_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_env_msg_out { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_env_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_in { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_host_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_host_msg_out { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_host_msg_out_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_sensor_msg_in { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_sensor_msg_in_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_sensor_msg_out { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_sensor_msg_out_data data;
	int selection;
};
struct Function_Event_actuator_msg_in { /* user defined type */
	union Function_Event_actuator_msg_in_data data;
	int selection;
};
struct Function_Event_controlsystem_msg_in { /* user defined type */
	union Function_Event_controlsystem_msg_in_data data;
	int selection;
};
struct Function_Event_controlsystem_msg_out { /* user defined type */
	union Function_Event_controlsystem_msg_out_data data;
	int selection;
};
struct Function_Event_env_msg_in { /* user defined type */
	union Function_Event_env_msg_in_data data;
	int selection;
};
struct Function_Event_env_msg_out { /* user defined type */
	union Function_Event_env_msg_out_data data;
	int selection;
};
struct Function_Event_host_msg_in { /* user defined type */
	union Function_Event_host_msg_in_data data;
	int selection;
};
struct Function_Event_host_msg_out { /* user defined type */
	union Function_Event_host_msg_out_data data;
	int selection;
};
struct Function_Event_sensor_msg_in { /* user defined type */
	union Function_Event_sensor_msg_in_data data;
	int selection;
};
struct Function_Event_sensor_msg_out { /* user defined type */
	union Function_Event_sensor_msg_out_data data;
	int selection;
};
struct Host_Event_msg_in { /* user defined type */
	union Host_Event_msg_in_data data;
	int selection;
};
struct Host_Event_msg_out { /* user defined type */
	union Host_Event_msg_out_data data;
	int selection;
};
struct Interface_Event_event_actuator_msg_in { /* user defined type */
	union Interface_Event_event_actuator_msg_in_data data;
	int selection;
};
struct Interface_Event_event_controlsystem_msg_in { /* user defined type */
	union Interface_Event_event_controlsystem_msg_in_data data;
	int selection;
};
struct Interface_Event_event_controlsystem_msg_out { /* user defined type */
	union Interface_Event_event_controlsystem_msg_out_data data;
	int selection;
};
struct Interface_Event_event_env_msg_in { /* user defined type */
	union Interface_Event_event_env_msg_in_data data;
	int selection;
};
struct Interface_Event_event_env_msg_out { /* user defined type */
	union Interface_Event_event_env_msg_out_data data;
	int selection;
};
struct Interface_Event_event_host_msg_in { /* user defined type */
	union Interface_Event_event_host_msg_in_data data;
	int selection;
};
struct Interface_Event_event_host_msg_out { /* user defined type */
	union Interface_Event_event_host_msg_out_data data;
	int selection;
};
struct Interface_Event_event_sensor_msg_in { /* user defined type */
	union Interface_Event_event_sensor_msg_in_data data;
	int selection;
};
struct Interface_Event_event_sensor_msg_out { /* user defined type */
	union Interface_Event_event_sensor_msg_out_data data;
	int selection;
};
struct Named_Function_Event_event_actuator_msg_in { /* user defined type */
	union Named_Function_Event_event_actuator_msg_in_data data;
	int selection;
};
struct Named_Function_Event_event_controlsystem_msg_in { /* user defined type */
	union Named_Function_Event_event_controlsystem_msg_in_data data;
	int selection;
};
struct Named_Function_Event_event_controlsystem_msg_out { /* user defined type */
	union Named_Function_Event_event_controlsystem_msg_out_data data;
	int selection;
};
struct Named_Function_Event_event_env_msg_in { /* user defined type */
	union Named_Function_Event_event_env_msg_in_data data;
	int selection;
};
struct Named_Function_Event_event_env_msg_out { /* user defined type */
	union Named_Function_Event_event_env_msg_out_data data;
	int selection;
};
struct Named_Function_Event_event_host_msg_in { /* user defined type */
	union Named_Function_Event_event_host_msg_in_data data;
	int selection;
};
struct Named_Function_Event_event_host_msg_out { /* user defined type */
	union Named_Function_Event_event_host_msg_out_data data;
	int selection;
};
struct Named_Function_Event_event_sensor_msg_in { /* user defined type */
	union Named_Function_Event_event_sensor_msg_in_data data;
	int selection;
};
struct Named_Function_Event_event_sensor_msg_out { /* user defined type */
	union Named_Function_Event_event_sensor_msg_out_data data;
	int selection;
};
struct Observable_Event_input_event_event_actuator_msg_in { /* user defined type */
	union Observable_Event_input_event_event_actuator_msg_in_data data;
	int selection;
};
struct Observable_Event_input_event_event_controlsystem_msg_in { /* user defined type */
	union Observable_Event_input_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct Observable_Event_input_event_event_controlsystem_msg_out { /* user defined type */
	union Observable_Event_input_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct Observable_Event_input_event_event_env_msg_in { /* user defined type */
	union Observable_Event_input_event_event_env_msg_in_data data;
	int selection;
};
struct Observable_Event_input_event_event_env_msg_out { /* user defined type */
	union Observable_Event_input_event_event_env_msg_out_data data;
	int selection;
};
struct Observable_Event_input_event_event_host_msg_in { /* user defined type */
	union Observable_Event_input_event_event_host_msg_in_data data;
	int selection;
};
struct Observable_Event_input_event_event_host_msg_out { /* user defined type */
	union Observable_Event_input_event_event_host_msg_out_data data;
	int selection;
};
struct Observable_Event_input_event_event_sensor_msg_in { /* user defined type */
	union Observable_Event_input_event_event_sensor_msg_in_data data;
	int selection;
};
struct Observable_Event_input_event_event_sensor_msg_out { /* user defined type */
	union Observable_Event_input_event_event_sensor_msg_out_data data;
	int selection;
};
struct Observable_Event_output_event_event_actuator_msg_in { /* user defined type */
	union Observable_Event_output_event_event_actuator_msg_in_data data;
	int selection;
};
struct Observable_Event_output_event_event_controlsystem_msg_in { /* user defined type */
	union Observable_Event_output_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct Observable_Event_output_event_event_controlsystem_msg_out { /* user defined type */
	union Observable_Event_output_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct Observable_Event_output_event_event_env_msg_in { /* user defined type */
	union Observable_Event_output_event_event_env_msg_in_data data;
	int selection;
};
struct Observable_Event_output_event_event_env_msg_out { /* user defined type */
	union Observable_Event_output_event_event_env_msg_out_data data;
	int selection;
};
struct Observable_Event_output_event_event_host_msg_in { /* user defined type */
	union Observable_Event_output_event_event_host_msg_in_data data;
	int selection;
};
struct Observable_Event_output_event_event_host_msg_out { /* user defined type */
	union Observable_Event_output_event_event_host_msg_out_data data;
	int selection;
};
struct Observable_Event_output_event_event_sensor_msg_in { /* user defined type */
	union Observable_Event_output_event_event_sensor_msg_in_data data;
	int selection;
};
struct Observable_Event_output_event_event_sensor_msg_out { /* user defined type */
	union Observable_Event_output_event_event_sensor_msg_out_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_actuator_msg_in { /* user defined type */
	union Observable_Event_unhandled_input_event_actuator_msg_in_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_in { /* user defined type */
	union Observable_Event_unhandled_input_event_controlsystem_msg_in_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_controlsystem_msg_out { /* user defined type */
	union Observable_Event_unhandled_input_event_controlsystem_msg_out_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_env_msg_in { /* user defined type */
	union Observable_Event_unhandled_input_event_env_msg_in_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_env_msg_out { /* user defined type */
	union Observable_Event_unhandled_input_event_env_msg_out_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_host_msg_in { /* user defined type */
	union Observable_Event_unhandled_input_event_host_msg_in_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_host_msg_out { /* user defined type */
	union Observable_Event_unhandled_input_event_host_msg_out_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_sensor_msg_in { /* user defined type */
	union Observable_Event_unhandled_input_event_sensor_msg_in_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_sensor_msg_out { /* user defined type */
	union Observable_Event_unhandled_input_event_sensor_msg_out_data data;
	int selection;
};
struct Sensor_Event_msg_in { /* user defined type */
	union Sensor_Event_msg_in_data data;
	int selection;
};
struct Sensor_Event_msg_out { /* user defined type */
	union Sensor_Event_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_actuator_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_env_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_env_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_host_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_sensor_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_actuator_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_env_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_env_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_host_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_sensor_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_env_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_env_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_host_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_host_msg_out_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_out { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_env_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_env_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_env_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_env_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_out { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_actuator_msg_in { /* user defined type */
	union System_State_host_queue_elem_input_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_host_queue_elem_input_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_host_queue_elem_input_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_env_msg_in { /* user defined type */
	union System_State_host_queue_elem_input_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_env_msg_out { /* user defined type */
	union System_State_host_queue_elem_input_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_host_msg_in { /* user defined type */
	union System_State_host_queue_elem_input_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_host_msg_out { /* user defined type */
	union System_State_host_queue_elem_input_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_sensor_msg_in { /* user defined type */
	union System_State_host_queue_elem_input_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_sensor_msg_out { /* user defined type */
	union System_State_host_queue_elem_input_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_actuator_msg_in { /* user defined type */
	union System_State_host_queue_elem_output_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_host_queue_elem_output_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_host_queue_elem_output_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_env_msg_in { /* user defined type */
	union System_State_host_queue_elem_output_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_env_msg_out { /* user defined type */
	union System_State_host_queue_elem_output_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_host_msg_in { /* user defined type */
	union System_State_host_queue_elem_output_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_host_msg_out { /* user defined type */
	union System_State_host_queue_elem_output_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_sensor_msg_in { /* user defined type */
	union System_State_host_queue_elem_output_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_sensor_msg_out { /* user defined type */
	union System_State_host_queue_elem_output_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_actuator_msg_in { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_env_msg_in { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_env_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_env_msg_out { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_env_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_in { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_host_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_host_msg_out { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_host_msg_out_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_out { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_actuator_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_env_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_env_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_host_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_sensor_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_actuator_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_env_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_env_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_env_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_env_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_host_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_host_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_host_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_sensor_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_env_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_env_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_host_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_host_msg_out_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_out { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_out_data data;
	int selection;
};
struct System_State_timers { /* user defined type */
	struct System_State_timers_actuator actuator;
	struct System_State_timers_controlsystem controlsystem;
	struct System_State_timers_host host;
	struct System_State_timers_sensor sensor;
	struct System_State_timers_dummy_entry dummy_entry;
};
union Actuator_Event_data { /* user defined type */
	struct Actuator_Event_msg_in msg_in;
};
union Controlsystem_Event_data { /* user defined type */
	struct Controlsystem_Event_msg_in msg_in;
	struct Controlsystem_Event_msg_out msg_out;
};
union Demo_Timer_Manager_Event_data { /* user defined type */
	struct Demo_Timer_Manager_Event_msg_in msg_in;
	struct Demo_Timer_Manager_Event_msg_out msg_out;
};
union Demo_Timer_Manager_Periodic_Events_elem_event_data { /* user defined type */
	struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_in msg_in;
	struct Demo_Timer_Manager_Periodic_Events_elem_event_msg_out msg_out;
};
union Events_Ty_elem_input_event_event_actuator_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_actuator_msg_in msg_in;
};
union Events_Ty_elem_input_event_event_controlsystem_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_controlsystem_msg_in msg_in;
	struct Events_Ty_elem_input_event_event_controlsystem_msg_out msg_out;
};
union Events_Ty_elem_input_event_event_env_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_env_msg_in msg_in;
	struct Events_Ty_elem_input_event_event_env_msg_out msg_out;
};
union Events_Ty_elem_input_event_event_host_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_host_msg_in msg_in;
	struct Events_Ty_elem_input_event_event_host_msg_out msg_out;
};
union Events_Ty_elem_input_event_event_sensor_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_sensor_msg_in msg_in;
	struct Events_Ty_elem_input_event_event_sensor_msg_out msg_out;
};
union Events_Ty_elem_output_event_event_actuator_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_actuator_msg_in msg_in;
};
union Events_Ty_elem_output_event_event_controlsystem_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_controlsystem_msg_in msg_in;
	struct Events_Ty_elem_output_event_event_controlsystem_msg_out msg_out;
};
union Events_Ty_elem_output_event_event_env_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_env_msg_in msg_in;
	struct Events_Ty_elem_output_event_event_env_msg_out msg_out;
};
union Events_Ty_elem_output_event_event_host_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_host_msg_in msg_in;
	struct Events_Ty_elem_output_event_event_host_msg_out msg_out;
};
union Events_Ty_elem_output_event_event_sensor_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_sensor_msg_in msg_in;
	struct Events_Ty_elem_output_event_event_sensor_msg_out msg_out;
};
union Events_Ty_elem_unhandled_input_event_actuator_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_actuator_msg_in msg_in;
};
union Events_Ty_elem_unhandled_input_event_controlsystem_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_in msg_in;
	struct Events_Ty_elem_unhandled_input_event_controlsystem_msg_out msg_out;
};
union Events_Ty_elem_unhandled_input_event_env_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_env_msg_in msg_in;
	struct Events_Ty_elem_unhandled_input_event_env_msg_out msg_out;
};
union Events_Ty_elem_unhandled_input_event_host_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_host_msg_in msg_in;
	struct Events_Ty_elem_unhandled_input_event_host_msg_out msg_out;
};
union Events_Ty_elem_unhandled_input_event_sensor_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_sensor_msg_in msg_in;
	struct Events_Ty_elem_unhandled_input_event_sensor_msg_out msg_out;
};
union Function_Event_actuator_data { /* user defined type */
	struct Function_Event_actuator_msg_in msg_in;
};
union Function_Event_controlsystem_data { /* user defined type */
	struct Function_Event_controlsystem_msg_in msg_in;
	struct Function_Event_controlsystem_msg_out msg_out;
};
union Function_Event_env_data { /* user defined type */
	struct Function_Event_env_msg_in msg_in;
	struct Function_Event_env_msg_out msg_out;
};
union Function_Event_host_data { /* user defined type */
	struct Function_Event_host_msg_in msg_in;
	struct Function_Event_host_msg_out msg_out;
};
union Function_Event_sensor_data { /* user defined type */
	struct Function_Event_sensor_msg_in msg_in;
	struct Function_Event_sensor_msg_out msg_out;
};
union Host_Event_data { /* user defined type */
	struct Host_Event_msg_in msg_in;
	struct Host_Event_msg_out msg_out;
};
union Interface_Event_event_actuator_data { /* user defined type */
	struct Interface_Event_event_actuator_msg_in msg_in;
};
union Interface_Event_event_controlsystem_data { /* user defined type */
	struct Interface_Event_event_controlsystem_msg_in msg_in;
	struct Interface_Event_event_controlsystem_msg_out msg_out;
};
union Interface_Event_event_env_data { /* user defined type */
	struct Interface_Event_event_env_msg_in msg_in;
	struct Interface_Event_event_env_msg_out msg_out;
};
union Interface_Event_event_host_data { /* user defined type */
	struct Interface_Event_event_host_msg_in msg_in;
	struct Interface_Event_event_host_msg_out msg_out;
};
union Interface_Event_event_sensor_data { /* user defined type */
	struct Interface_Event_event_sensor_msg_in msg_in;
	struct Interface_Event_event_sensor_msg_out msg_out;
};
union Named_Function_Event_event_actuator_data { /* user defined type */
	struct Named_Function_Event_event_actuator_msg_in msg_in;
};
union Named_Function_Event_event_controlsystem_data { /* user defined type */
	struct Named_Function_Event_event_controlsystem_msg_in msg_in;
	struct Named_Function_Event_event_controlsystem_msg_out msg_out;
};
union Named_Function_Event_event_env_data { /* user defined type */
	struct Named_Function_Event_event_env_msg_in msg_in;
	struct Named_Function_Event_event_env_msg_out msg_out;
};
union Named_Function_Event_event_host_data { /* user defined type */
	struct Named_Function_Event_event_host_msg_in msg_in;
	struct Named_Function_Event_event_host_msg_out msg_out;
};
union Named_Function_Event_event_sensor_data { /* user defined type */
	struct Named_Function_Event_event_sensor_msg_in msg_in;
	struct Named_Function_Event_event_sensor_msg_out msg_out;
};
union Observable_Event_input_event_event_actuator_data { /* user defined type */
	struct Observable_Event_input_event_event_actuator_msg_in msg_in;
};
union Observable_Event_input_event_event_controlsystem_data { /* user defined type */
	struct Observable_Event_input_event_event_controlsystem_msg_in msg_in;
	struct Observable_Event_input_event_event_controlsystem_msg_out msg_out;
};
union Observable_Event_input_event_event_env_data { /* user defined type */
	struct Observable_Event_input_event_event_env_msg_in msg_in;
	struct Observable_Event_input_event_event_env_msg_out msg_out;
};
union Observable_Event_input_event_event_host_data { /* user defined type */
	struct Observable_Event_input_event_event_host_msg_in msg_in;
	struct Observable_Event_input_event_event_host_msg_out msg_out;
};
union Observable_Event_input_event_event_sensor_data { /* user defined type */
	struct Observable_Event_input_event_event_sensor_msg_in msg_in;
	struct Observable_Event_input_event_event_sensor_msg_out msg_out;
};
union Observable_Event_output_event_event_actuator_data { /* user defined type */
	struct Observable_Event_output_event_event_actuator_msg_in msg_in;
};
union Observable_Event_output_event_event_controlsystem_data { /* user defined type */
	struct Observable_Event_output_event_event_controlsystem_msg_in msg_in;
	struct Observable_Event_output_event_event_controlsystem_msg_out msg_out;
};
union Observable_Event_output_event_event_env_data { /* user defined type */
	struct Observable_Event_output_event_event_env_msg_in msg_in;
	struct Observable_Event_output_event_event_env_msg_out msg_out;
};
union Observable_Event_output_event_event_host_data { /* user defined type */
	struct Observable_Event_output_event_event_host_msg_in msg_in;
	struct Observable_Event_output_event_event_host_msg_out msg_out;
};
union Observable_Event_output_event_event_sensor_data { /* user defined type */
	struct Observable_Event_output_event_event_sensor_msg_in msg_in;
	struct Observable_Event_output_event_event_sensor_msg_out msg_out;
};
union Observable_Event_unhandled_input_event_actuator_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_actuator_msg_in msg_in;
};
union Observable_Event_unhandled_input_event_controlsystem_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_controlsystem_msg_in msg_in;
	struct Observable_Event_unhandled_input_event_controlsystem_msg_out msg_out;
};
union Observable_Event_unhandled_input_event_env_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_env_msg_in msg_in;
	struct Observable_Event_unhandled_input_event_env_msg_out msg_out;
};
union Observable_Event_unhandled_input_event_host_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_host_msg_in msg_in;
	struct Observable_Event_unhandled_input_event_host_msg_out msg_out;
};
union Observable_Event_unhandled_input_event_sensor_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_sensor_msg_in msg_in;
	struct Observable_Event_unhandled_input_event_sensor_msg_out msg_out;
};
union Sensor_Event_data { /* user defined type */
	struct Sensor_Event_msg_in msg_in;
	struct Sensor_Event_msg_out msg_out;
};
union System_State_actuator_queue_elem_input_event_event_actuator_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_actuator_msg_in msg_in;
};
union System_State_actuator_queue_elem_input_event_event_controlsystem_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_in msg_in;
	struct System_State_actuator_queue_elem_input_event_event_controlsystem_msg_out msg_out;
};
union System_State_actuator_queue_elem_input_event_event_env_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_env_msg_in msg_in;
	struct System_State_actuator_queue_elem_input_event_event_env_msg_out msg_out;
};
union System_State_actuator_queue_elem_input_event_event_host_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_host_msg_in msg_in;
	struct System_State_actuator_queue_elem_input_event_event_host_msg_out msg_out;
};
union System_State_actuator_queue_elem_input_event_event_sensor_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_sensor_msg_in msg_in;
	struct System_State_actuator_queue_elem_input_event_event_sensor_msg_out msg_out;
};
union System_State_actuator_queue_elem_output_event_event_actuator_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_actuator_msg_in msg_in;
};
union System_State_actuator_queue_elem_output_event_event_controlsystem_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_in msg_in;
	struct System_State_actuator_queue_elem_output_event_event_controlsystem_msg_out msg_out;
};
union System_State_actuator_queue_elem_output_event_event_env_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_env_msg_in msg_in;
	struct System_State_actuator_queue_elem_output_event_event_env_msg_out msg_out;
};
union System_State_actuator_queue_elem_output_event_event_host_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_host_msg_in msg_in;
	struct System_State_actuator_queue_elem_output_event_event_host_msg_out msg_out;
};
union System_State_actuator_queue_elem_output_event_event_sensor_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_sensor_msg_in msg_in;
	struct System_State_actuator_queue_elem_output_event_event_sensor_msg_out msg_out;
};
union System_State_actuator_queue_elem_unhandled_input_event_actuator_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_actuator_msg_in msg_in;
};
union System_State_actuator_queue_elem_unhandled_input_event_controlsystem_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_in msg_in;
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem_msg_out msg_out;
};
union System_State_actuator_queue_elem_unhandled_input_event_env_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_in msg_in;
	struct System_State_actuator_queue_elem_unhandled_input_event_env_msg_out msg_out;
};
union System_State_actuator_queue_elem_unhandled_input_event_host_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_in msg_in;
	struct System_State_actuator_queue_elem_unhandled_input_event_host_msg_out msg_out;
};
union System_State_actuator_queue_elem_unhandled_input_event_sensor_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_in msg_in;
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_input_event_event_actuator_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_actuator_msg_in msg_in;
};
union System_State_controlsystem_queue_elem_input_event_event_controlsystem_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_input_event_event_env_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_env_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_input_event_event_env_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_input_event_event_host_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_input_event_event_host_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_input_event_event_sensor_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_input_event_event_sensor_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_output_event_event_actuator_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_actuator_msg_in msg_in;
};
union System_State_controlsystem_queue_elem_output_event_event_controlsystem_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_output_event_event_env_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_env_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_output_event_event_env_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_output_event_event_host_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_output_event_event_host_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_output_event_event_sensor_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_output_event_event_sensor_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_actuator_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator_msg_in msg_in;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_env_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_env_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_host_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host_msg_out msg_out;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_sensor_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_in msg_in;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor_msg_out msg_out;
};
union System_State_host_queue_elem_input_event_event_actuator_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_actuator_msg_in msg_in;
};
union System_State_host_queue_elem_input_event_event_controlsystem_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_in msg_in;
	struct System_State_host_queue_elem_input_event_event_controlsystem_msg_out msg_out;
};
union System_State_host_queue_elem_input_event_event_env_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_env_msg_in msg_in;
	struct System_State_host_queue_elem_input_event_event_env_msg_out msg_out;
};
union System_State_host_queue_elem_input_event_event_host_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_host_msg_in msg_in;
	struct System_State_host_queue_elem_input_event_event_host_msg_out msg_out;
};
union System_State_host_queue_elem_input_event_event_sensor_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_sensor_msg_in msg_in;
	struct System_State_host_queue_elem_input_event_event_sensor_msg_out msg_out;
};
union System_State_host_queue_elem_output_event_event_actuator_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_actuator_msg_in msg_in;
};
union System_State_host_queue_elem_output_event_event_controlsystem_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_in msg_in;
	struct System_State_host_queue_elem_output_event_event_controlsystem_msg_out msg_out;
};
union System_State_host_queue_elem_output_event_event_env_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_env_msg_in msg_in;
	struct System_State_host_queue_elem_output_event_event_env_msg_out msg_out;
};
union System_State_host_queue_elem_output_event_event_host_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_host_msg_in msg_in;
	struct System_State_host_queue_elem_output_event_event_host_msg_out msg_out;
};
union System_State_host_queue_elem_output_event_event_sensor_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_sensor_msg_in msg_in;
	struct System_State_host_queue_elem_output_event_event_sensor_msg_out msg_out;
};
union System_State_host_queue_elem_unhandled_input_event_actuator_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_actuator_msg_in msg_in;
};
union System_State_host_queue_elem_unhandled_input_event_controlsystem_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_in msg_in;
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem_msg_out msg_out;
};
union System_State_host_queue_elem_unhandled_input_event_env_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_env_msg_in msg_in;
	struct System_State_host_queue_elem_unhandled_input_event_env_msg_out msg_out;
};
union System_State_host_queue_elem_unhandled_input_event_host_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_in msg_in;
	struct System_State_host_queue_elem_unhandled_input_event_host_msg_out msg_out;
};
union System_State_host_queue_elem_unhandled_input_event_sensor_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_in msg_in;
	struct System_State_host_queue_elem_unhandled_input_event_sensor_msg_out msg_out;
};
union System_State_sensor_queue_elem_input_event_event_actuator_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_actuator_msg_in msg_in;
};
union System_State_sensor_queue_elem_input_event_event_controlsystem_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_in msg_in;
	struct System_State_sensor_queue_elem_input_event_event_controlsystem_msg_out msg_out;
};
union System_State_sensor_queue_elem_input_event_event_env_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_env_msg_in msg_in;
	struct System_State_sensor_queue_elem_input_event_event_env_msg_out msg_out;
};
union System_State_sensor_queue_elem_input_event_event_host_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_host_msg_in msg_in;
	struct System_State_sensor_queue_elem_input_event_event_host_msg_out msg_out;
};
union System_State_sensor_queue_elem_input_event_event_sensor_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_sensor_msg_in msg_in;
	struct System_State_sensor_queue_elem_input_event_event_sensor_msg_out msg_out;
};
union System_State_sensor_queue_elem_output_event_event_actuator_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_actuator_msg_in msg_in;
};
union System_State_sensor_queue_elem_output_event_event_controlsystem_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_in msg_in;
	struct System_State_sensor_queue_elem_output_event_event_controlsystem_msg_out msg_out;
};
union System_State_sensor_queue_elem_output_event_event_env_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_env_msg_in msg_in;
	struct System_State_sensor_queue_elem_output_event_event_env_msg_out msg_out;
};
union System_State_sensor_queue_elem_output_event_event_host_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_host_msg_in msg_in;
	struct System_State_sensor_queue_elem_output_event_event_host_msg_out msg_out;
};
union System_State_sensor_queue_elem_output_event_event_sensor_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_sensor_msg_in msg_in;
	struct System_State_sensor_queue_elem_output_event_event_sensor_msg_out msg_out;
};
union System_State_sensor_queue_elem_unhandled_input_event_actuator_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_actuator_msg_in msg_in;
};
union System_State_sensor_queue_elem_unhandled_input_event_controlsystem_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_in msg_in;
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem_msg_out msg_out;
};
union System_State_sensor_queue_elem_unhandled_input_event_env_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_in msg_in;
	struct System_State_sensor_queue_elem_unhandled_input_event_env_msg_out msg_out;
};
union System_State_sensor_queue_elem_unhandled_input_event_host_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_in msg_in;
	struct System_State_sensor_queue_elem_unhandled_input_event_host_msg_out msg_out;
};
union System_State_sensor_queue_elem_unhandled_input_event_sensor_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_in msg_in;
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor_msg_out msg_out;
};
struct Actuator_Event { /* user defined type */
	union Actuator_Event_data data;
	int selection;
};
struct Controlsystem_Event { /* user defined type */
	union Controlsystem_Event_data data;
	int selection;
};
struct Demo_Timer_Manager_Event { /* user defined type */
	union Demo_Timer_Manager_Event_data data;
	int selection;
};
struct Demo_Timer_Manager_Periodic_Events_elem_event { /* user defined type */
	union Demo_Timer_Manager_Periodic_Events_elem_event_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_actuator { /* user defined type */
	union Events_Ty_elem_input_event_event_actuator_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_controlsystem { /* user defined type */
	union Events_Ty_elem_input_event_event_controlsystem_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_env { /* user defined type */
	union Events_Ty_elem_input_event_event_env_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_host { /* user defined type */
	union Events_Ty_elem_input_event_event_host_data data;
	int selection;
};
struct Events_Ty_elem_input_event_event_sensor { /* user defined type */
	union Events_Ty_elem_input_event_event_sensor_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_actuator { /* user defined type */
	union Events_Ty_elem_output_event_event_actuator_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_controlsystem { /* user defined type */
	union Events_Ty_elem_output_event_event_controlsystem_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_env { /* user defined type */
	union Events_Ty_elem_output_event_event_env_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_host { /* user defined type */
	union Events_Ty_elem_output_event_event_host_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event_sensor { /* user defined type */
	union Events_Ty_elem_output_event_event_sensor_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_actuator { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_actuator_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_controlsystem { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_controlsystem_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_env { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_env_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_host { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_host_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event_sensor { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_sensor_data data;
	int selection;
};
struct Function_Event_actuator { /* user defined type */
	union Function_Event_actuator_data data;
	int selection;
};
struct Function_Event_controlsystem { /* user defined type */
	union Function_Event_controlsystem_data data;
	int selection;
};
struct Function_Event_env { /* user defined type */
	union Function_Event_env_data data;
	int selection;
};
struct Function_Event_host { /* user defined type */
	union Function_Event_host_data data;
	int selection;
};
struct Function_Event_sensor { /* user defined type */
	union Function_Event_sensor_data data;
	int selection;
};
struct Host_Event { /* user defined type */
	union Host_Event_data data;
	int selection;
};
struct Interface_Event_event_actuator { /* user defined type */
	union Interface_Event_event_actuator_data data;
	int selection;
};
struct Interface_Event_event_controlsystem { /* user defined type */
	union Interface_Event_event_controlsystem_data data;
	int selection;
};
struct Interface_Event_event_env { /* user defined type */
	union Interface_Event_event_env_data data;
	int selection;
};
struct Interface_Event_event_host { /* user defined type */
	union Interface_Event_event_host_data data;
	int selection;
};
struct Interface_Event_event_sensor { /* user defined type */
	union Interface_Event_event_sensor_data data;
	int selection;
};
struct Named_Function_Event_event_actuator { /* user defined type */
	union Named_Function_Event_event_actuator_data data;
	int selection;
};
struct Named_Function_Event_event_controlsystem { /* user defined type */
	union Named_Function_Event_event_controlsystem_data data;
	int selection;
};
struct Named_Function_Event_event_env { /* user defined type */
	union Named_Function_Event_event_env_data data;
	int selection;
};
struct Named_Function_Event_event_host { /* user defined type */
	union Named_Function_Event_event_host_data data;
	int selection;
};
struct Named_Function_Event_event_sensor { /* user defined type */
	union Named_Function_Event_event_sensor_data data;
	int selection;
};
struct Observable_Event_input_event_event_actuator { /* user defined type */
	union Observable_Event_input_event_event_actuator_data data;
	int selection;
};
struct Observable_Event_input_event_event_controlsystem { /* user defined type */
	union Observable_Event_input_event_event_controlsystem_data data;
	int selection;
};
struct Observable_Event_input_event_event_env { /* user defined type */
	union Observable_Event_input_event_event_env_data data;
	int selection;
};
struct Observable_Event_input_event_event_host { /* user defined type */
	union Observable_Event_input_event_event_host_data data;
	int selection;
};
struct Observable_Event_input_event_event_sensor { /* user defined type */
	union Observable_Event_input_event_event_sensor_data data;
	int selection;
};
struct Observable_Event_output_event_event_actuator { /* user defined type */
	union Observable_Event_output_event_event_actuator_data data;
	int selection;
};
struct Observable_Event_output_event_event_controlsystem { /* user defined type */
	union Observable_Event_output_event_event_controlsystem_data data;
	int selection;
};
struct Observable_Event_output_event_event_env { /* user defined type */
	union Observable_Event_output_event_event_env_data data;
	int selection;
};
struct Observable_Event_output_event_event_host { /* user defined type */
	union Observable_Event_output_event_event_host_data data;
	int selection;
};
struct Observable_Event_output_event_event_sensor { /* user defined type */
	union Observable_Event_output_event_event_sensor_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_actuator { /* user defined type */
	union Observable_Event_unhandled_input_event_actuator_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_controlsystem { /* user defined type */
	union Observable_Event_unhandled_input_event_controlsystem_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_env { /* user defined type */
	union Observable_Event_unhandled_input_event_env_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_host { /* user defined type */
	union Observable_Event_unhandled_input_event_host_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event_sensor { /* user defined type */
	union Observable_Event_unhandled_input_event_sensor_data data;
	int selection;
};
struct Sensor_Event { /* user defined type */
	union Sensor_Event_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_actuator { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_actuator_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_controlsystem { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_controlsystem_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_env { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_env_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_host { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_host_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event_sensor { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_sensor_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_actuator { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_actuator_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_controlsystem { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_controlsystem_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_env { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_env_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_host { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_host_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event_sensor { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_sensor_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_actuator { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_actuator_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_controlsystem_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_env { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_env_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_host { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_host_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event_sensor { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_sensor_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_actuator { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_actuator_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_controlsystem { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_controlsystem_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_env { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_env_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_host { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_host_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event_sensor { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_sensor_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_actuator { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_actuator_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_controlsystem { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_controlsystem_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_env { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_env_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_host { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_host_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event_sensor { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_sensor_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_actuator_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_env { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_env_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_host { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_host_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_sensor_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_actuator { /* user defined type */
	union System_State_host_queue_elem_input_event_event_actuator_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_controlsystem { /* user defined type */
	union System_State_host_queue_elem_input_event_event_controlsystem_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_env { /* user defined type */
	union System_State_host_queue_elem_input_event_event_env_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_host { /* user defined type */
	union System_State_host_queue_elem_input_event_event_host_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event_sensor { /* user defined type */
	union System_State_host_queue_elem_input_event_event_sensor_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_actuator { /* user defined type */
	union System_State_host_queue_elem_output_event_event_actuator_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_controlsystem { /* user defined type */
	union System_State_host_queue_elem_output_event_event_controlsystem_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_env { /* user defined type */
	union System_State_host_queue_elem_output_event_event_env_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_host { /* user defined type */
	union System_State_host_queue_elem_output_event_event_host_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event_sensor { /* user defined type */
	union System_State_host_queue_elem_output_event_event_sensor_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_actuator { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_actuator_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_controlsystem { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_controlsystem_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_env { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_env_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_host { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_host_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event_sensor { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_sensor_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_actuator { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_actuator_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_controlsystem { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_controlsystem_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_env { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_env_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_host { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_host_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event_sensor { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_sensor_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_actuator { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_actuator_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_controlsystem { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_controlsystem_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_env { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_env_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_host { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_host_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event_sensor { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_sensor_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_actuator { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_actuator_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_controlsystem_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_env { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_env_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_host { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_host_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event_sensor { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_sensor_data data;
	int selection;
};
struct Demo_Timer_Manager_Periodic_Events_elem { /* user defined type */
	int period;
	struct Demo_Timer_Manager_Periodic_Events_elem_event event;
};
union Events_Ty_elem_input_event_event_data { /* user defined type */
	struct Events_Ty_elem_input_event_event_actuator actuator;
	struct Events_Ty_elem_input_event_event_controlsystem controlsystem;
	struct Events_Ty_elem_input_event_event_env env;
	struct Events_Ty_elem_input_event_event_host host;
	struct Events_Ty_elem_input_event_event_sensor sensor;
};
union Events_Ty_elem_output_event_event_data { /* user defined type */
	struct Events_Ty_elem_output_event_event_actuator actuator;
	struct Events_Ty_elem_output_event_event_controlsystem controlsystem;
	struct Events_Ty_elem_output_event_event_env env;
	struct Events_Ty_elem_output_event_event_host host;
	struct Events_Ty_elem_output_event_event_sensor sensor;
};
union Events_Ty_elem_unhandled_input_event_data { /* user defined type */
	struct Events_Ty_elem_unhandled_input_event_actuator actuator;
	struct Events_Ty_elem_unhandled_input_event_controlsystem controlsystem;
	struct Events_Ty_elem_unhandled_input_event_env env;
	struct Events_Ty_elem_unhandled_input_event_host host;
	struct Events_Ty_elem_unhandled_input_event_sensor sensor;
};
union Function_Event_data { /* user defined type */
	struct Function_Event_actuator actuator;
	struct Function_Event_controlsystem controlsystem;
	struct Function_Event_env env;
	struct Function_Event_host host;
	struct Function_Event_sensor sensor;
};
union Interface_Event_event_data { /* user defined type */
	struct Interface_Event_event_actuator actuator;
	struct Interface_Event_event_controlsystem controlsystem;
	struct Interface_Event_event_env env;
	struct Interface_Event_event_host host;
	struct Interface_Event_event_sensor sensor;
};
union Named_Function_Event_event_data { /* user defined type */
	struct Named_Function_Event_event_actuator actuator;
	struct Named_Function_Event_event_controlsystem controlsystem;
	struct Named_Function_Event_event_env env;
	struct Named_Function_Event_event_host host;
	struct Named_Function_Event_event_sensor sensor;
};
union Observable_Event_input_event_event_data { /* user defined type */
	struct Observable_Event_input_event_event_actuator actuator;
	struct Observable_Event_input_event_event_controlsystem controlsystem;
	struct Observable_Event_input_event_event_env env;
	struct Observable_Event_input_event_event_host host;
	struct Observable_Event_input_event_event_sensor sensor;
};
union Observable_Event_output_event_event_data { /* user defined type */
	struct Observable_Event_output_event_event_actuator actuator;
	struct Observable_Event_output_event_event_controlsystem controlsystem;
	struct Observable_Event_output_event_event_env env;
	struct Observable_Event_output_event_event_host host;
	struct Observable_Event_output_event_event_sensor sensor;
};
union Observable_Event_unhandled_input_event_data { /* user defined type */
	struct Observable_Event_unhandled_input_event_actuator actuator;
	struct Observable_Event_unhandled_input_event_controlsystem controlsystem;
	struct Observable_Event_unhandled_input_event_env env;
	struct Observable_Event_unhandled_input_event_host host;
	struct Observable_Event_unhandled_input_event_sensor sensor;
};
union System_State_actuator_queue_elem_input_event_event_data { /* user defined type */
	struct System_State_actuator_queue_elem_input_event_event_actuator actuator;
	struct System_State_actuator_queue_elem_input_event_event_controlsystem controlsystem;
	struct System_State_actuator_queue_elem_input_event_event_env env;
	struct System_State_actuator_queue_elem_input_event_event_host host;
	struct System_State_actuator_queue_elem_input_event_event_sensor sensor;
};
union System_State_actuator_queue_elem_output_event_event_data { /* user defined type */
	struct System_State_actuator_queue_elem_output_event_event_actuator actuator;
	struct System_State_actuator_queue_elem_output_event_event_controlsystem controlsystem;
	struct System_State_actuator_queue_elem_output_event_event_env env;
	struct System_State_actuator_queue_elem_output_event_event_host host;
	struct System_State_actuator_queue_elem_output_event_event_sensor sensor;
};
union System_State_actuator_queue_elem_unhandled_input_event_data { /* user defined type */
	struct System_State_actuator_queue_elem_unhandled_input_event_actuator actuator;
	struct System_State_actuator_queue_elem_unhandled_input_event_controlsystem controlsystem;
	struct System_State_actuator_queue_elem_unhandled_input_event_env env;
	struct System_State_actuator_queue_elem_unhandled_input_event_host host;
	struct System_State_actuator_queue_elem_unhandled_input_event_sensor sensor;
};
union System_State_controlsystem_queue_elem_input_event_event_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_input_event_event_actuator actuator;
	struct System_State_controlsystem_queue_elem_input_event_event_controlsystem controlsystem;
	struct System_State_controlsystem_queue_elem_input_event_event_env env;
	struct System_State_controlsystem_queue_elem_input_event_event_host host;
	struct System_State_controlsystem_queue_elem_input_event_event_sensor sensor;
};
union System_State_controlsystem_queue_elem_output_event_event_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_output_event_event_actuator actuator;
	struct System_State_controlsystem_queue_elem_output_event_event_controlsystem controlsystem;
	struct System_State_controlsystem_queue_elem_output_event_event_env env;
	struct System_State_controlsystem_queue_elem_output_event_event_host host;
	struct System_State_controlsystem_queue_elem_output_event_event_sensor sensor;
};
union System_State_controlsystem_queue_elem_unhandled_input_event_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_unhandled_input_event_actuator actuator;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_controlsystem controlsystem;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_env env;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_host host;
	struct System_State_controlsystem_queue_elem_unhandled_input_event_sensor sensor;
};
union System_State_host_queue_elem_input_event_event_data { /* user defined type */
	struct System_State_host_queue_elem_input_event_event_actuator actuator;
	struct System_State_host_queue_elem_input_event_event_controlsystem controlsystem;
	struct System_State_host_queue_elem_input_event_event_env env;
	struct System_State_host_queue_elem_input_event_event_host host;
	struct System_State_host_queue_elem_input_event_event_sensor sensor;
};
union System_State_host_queue_elem_output_event_event_data { /* user defined type */
	struct System_State_host_queue_elem_output_event_event_actuator actuator;
	struct System_State_host_queue_elem_output_event_event_controlsystem controlsystem;
	struct System_State_host_queue_elem_output_event_event_env env;
	struct System_State_host_queue_elem_output_event_event_host host;
	struct System_State_host_queue_elem_output_event_event_sensor sensor;
};
union System_State_host_queue_elem_unhandled_input_event_data { /* user defined type */
	struct System_State_host_queue_elem_unhandled_input_event_actuator actuator;
	struct System_State_host_queue_elem_unhandled_input_event_controlsystem controlsystem;
	struct System_State_host_queue_elem_unhandled_input_event_env env;
	struct System_State_host_queue_elem_unhandled_input_event_host host;
	struct System_State_host_queue_elem_unhandled_input_event_sensor sensor;
};
union System_State_sensor_queue_elem_input_event_event_data { /* user defined type */
	struct System_State_sensor_queue_elem_input_event_event_actuator actuator;
	struct System_State_sensor_queue_elem_input_event_event_controlsystem controlsystem;
	struct System_State_sensor_queue_elem_input_event_event_env env;
	struct System_State_sensor_queue_elem_input_event_event_host host;
	struct System_State_sensor_queue_elem_input_event_event_sensor sensor;
};
union System_State_sensor_queue_elem_output_event_event_data { /* user defined type */
	struct System_State_sensor_queue_elem_output_event_event_actuator actuator;
	struct System_State_sensor_queue_elem_output_event_event_controlsystem controlsystem;
	struct System_State_sensor_queue_elem_output_event_event_env env;
	struct System_State_sensor_queue_elem_output_event_event_host host;
	struct System_State_sensor_queue_elem_output_event_event_sensor sensor;
};
union System_State_sensor_queue_elem_unhandled_input_event_data { /* user defined type */
	struct System_State_sensor_queue_elem_unhandled_input_event_actuator actuator;
	struct System_State_sensor_queue_elem_unhandled_input_event_controlsystem controlsystem;
	struct System_State_sensor_queue_elem_unhandled_input_event_env env;
	struct System_State_sensor_queue_elem_unhandled_input_event_host host;
	struct System_State_sensor_queue_elem_unhandled_input_event_sensor sensor;
};
struct Demo_Timer_Manager_Periodic_Events { /* user defined type */
	struct Demo_Timer_Manager_Periodic_Events_elem data[1];
	int length;
};
struct Events_Ty_elem_input_event_event { /* user defined type */
	union Events_Ty_elem_input_event_event_data data;
	int selection;
};
struct Events_Ty_elem_output_event_event { /* user defined type */
	union Events_Ty_elem_output_event_event_data data;
	int selection;
};
struct Events_Ty_elem_unhandled_input_event { /* user defined type */
	union Events_Ty_elem_unhandled_input_event_data data;
	int selection;
};
struct Function_Event { /* user defined type */
	union Function_Event_data data;
	int selection;
};
struct Interface_Event_event { /* user defined type */
	union Interface_Event_event_data data;
	int selection;
};
struct Named_Function_Event_event { /* user defined type */
	union Named_Function_Event_event_data data;
	int selection;
};
struct Observable_Event_input_event_event { /* user defined type */
	union Observable_Event_input_event_event_data data;
	int selection;
};
struct Observable_Event_output_event_event { /* user defined type */
	union Observable_Event_output_event_event_data data;
	int selection;
};
struct Observable_Event_unhandled_input_event { /* user defined type */
	union Observable_Event_unhandled_input_event_data data;
	int selection;
};
struct System_State_actuator_queue_elem_input_event_event { /* user defined type */
	union System_State_actuator_queue_elem_input_event_event_data data;
	int selection;
};
struct System_State_actuator_queue_elem_output_event_event { /* user defined type */
	union System_State_actuator_queue_elem_output_event_event_data data;
	int selection;
};
struct System_State_actuator_queue_elem_unhandled_input_event { /* user defined type */
	union System_State_actuator_queue_elem_unhandled_input_event_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_input_event_event { /* user defined type */
	union System_State_controlsystem_queue_elem_input_event_event_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_output_event_event { /* user defined type */
	union System_State_controlsystem_queue_elem_output_event_event_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem_unhandled_input_event { /* user defined type */
	union System_State_controlsystem_queue_elem_unhandled_input_event_data data;
	int selection;
};
struct System_State_host_queue_elem_input_event_event { /* user defined type */
	union System_State_host_queue_elem_input_event_event_data data;
	int selection;
};
struct System_State_host_queue_elem_output_event_event { /* user defined type */
	union System_State_host_queue_elem_output_event_event_data data;
	int selection;
};
struct System_State_host_queue_elem_unhandled_input_event { /* user defined type */
	union System_State_host_queue_elem_unhandled_input_event_data data;
	int selection;
};
struct System_State_sensor_queue_elem_input_event_event { /* user defined type */
	union System_State_sensor_queue_elem_input_event_event_data data;
	int selection;
};
struct System_State_sensor_queue_elem_output_event_event { /* user defined type */
	union System_State_sensor_queue_elem_output_event_event_data data;
	int selection;
};
struct System_State_sensor_queue_elem_unhandled_input_event { /* user defined type */
	union System_State_sensor_queue_elem_unhandled_input_event_data data;
	int selection;
};
struct Events_Ty_elem_input_event { /* user defined type */
	int source;
	int dest;
	struct Events_Ty_elem_input_event_event event;
};
struct Events_Ty_elem_output_event { /* user defined type */
	int source;
	int dest;
	struct Events_Ty_elem_output_event_event event;
};
struct Events_Ty_elem_unhandled_input { /* user defined type */
	int source;
	int dest;
	struct Events_Ty_elem_unhandled_input_event event;
};
struct Interface_Event { /* user defined type */
	int source;
	int dest;
	struct Interface_Event_event event;
};
struct Named_Function_Event { /* user defined type */
	struct Named_Function_Event_id id;
	struct Named_Function_Event_event event;
};
struct Observable_Event_input_event { /* user defined type */
	int source;
	int dest;
	struct Observable_Event_input_event_event event;
};
struct Observable_Event_output_event { /* user defined type */
	int source;
	int dest;
	struct Observable_Event_output_event_event event;
};
struct Observable_Event_unhandled_input { /* user defined type */
	int source;
	int dest;
	struct Observable_Event_unhandled_input_event event;
};
struct System_State_actuator_queue_elem_input_event { /* user defined type */
	int source;
	int dest;
	struct System_State_actuator_queue_elem_input_event_event event;
};
struct System_State_actuator_queue_elem_output_event { /* user defined type */
	int source;
	int dest;
	struct System_State_actuator_queue_elem_output_event_event event;
};
struct System_State_actuator_queue_elem_unhandled_input { /* user defined type */
	int source;
	int dest;
	struct System_State_actuator_queue_elem_unhandled_input_event event;
};
struct System_State_controlsystem_queue_elem_input_event { /* user defined type */
	int source;
	int dest;
	struct System_State_controlsystem_queue_elem_input_event_event event;
};
struct System_State_controlsystem_queue_elem_output_event { /* user defined type */
	int source;
	int dest;
	struct System_State_controlsystem_queue_elem_output_event_event event;
};
struct System_State_controlsystem_queue_elem_unhandled_input { /* user defined type */
	int source;
	int dest;
	struct System_State_controlsystem_queue_elem_unhandled_input_event event;
};
struct System_State_host_queue_elem_input_event { /* user defined type */
	int source;
	int dest;
	struct System_State_host_queue_elem_input_event_event event;
};
struct System_State_host_queue_elem_output_event { /* user defined type */
	int source;
	int dest;
	struct System_State_host_queue_elem_output_event_event event;
};
struct System_State_host_queue_elem_unhandled_input { /* user defined type */
	int source;
	int dest;
	struct System_State_host_queue_elem_unhandled_input_event event;
};
struct System_State_sensor_queue_elem_input_event { /* user defined type */
	int source;
	int dest;
	struct System_State_sensor_queue_elem_input_event_event event;
};
struct System_State_sensor_queue_elem_output_event { /* user defined type */
	int source;
	int dest;
	struct System_State_sensor_queue_elem_output_event_event event;
};
struct System_State_sensor_queue_elem_unhandled_input { /* user defined type */
	int source;
	int dest;
	struct System_State_sensor_queue_elem_unhandled_input_event event;
};
union Events_Ty_elem_data { /* user defined type */
	struct Events_Ty_elem_no_event no_event;
	struct Events_Ty_elem_system_startup system_startup;
	struct Events_Ty_elem_input_event input_event;
	struct Events_Ty_elem_output_event output_event;
	struct Events_Ty_elem_unhandled_input unhandled_input;
};
union Observable_Event_data { /* user defined type */
	struct Observable_Event_no_event no_event;
	struct Observable_Event_system_startup system_startup;
	struct Observable_Event_input_event input_event;
	struct Observable_Event_output_event output_event;
	struct Observable_Event_unhandled_input unhandled_input;
};
union System_State_actuator_queue_elem_data { /* user defined type */
	struct System_State_actuator_queue_elem_no_event no_event;
	struct System_State_actuator_queue_elem_system_startup system_startup;
	struct System_State_actuator_queue_elem_input_event input_event;
	struct System_State_actuator_queue_elem_output_event output_event;
	struct System_State_actuator_queue_elem_unhandled_input unhandled_input;
};
union System_State_controlsystem_queue_elem_data { /* user defined type */
	struct System_State_controlsystem_queue_elem_no_event no_event;
	struct System_State_controlsystem_queue_elem_system_startup system_startup;
	struct System_State_controlsystem_queue_elem_input_event input_event;
	struct System_State_controlsystem_queue_elem_output_event output_event;
	struct System_State_controlsystem_queue_elem_unhandled_input unhandled_input;
};
union System_State_host_queue_elem_data { /* user defined type */
	struct System_State_host_queue_elem_no_event no_event;
	struct System_State_host_queue_elem_system_startup system_startup;
	struct System_State_host_queue_elem_input_event input_event;
	struct System_State_host_queue_elem_output_event output_event;
	struct System_State_host_queue_elem_unhandled_input unhandled_input;
};
union System_State_sensor_queue_elem_data { /* user defined type */
	struct System_State_sensor_queue_elem_no_event no_event;
	struct System_State_sensor_queue_elem_system_startup system_startup;
	struct System_State_sensor_queue_elem_input_event input_event;
	struct System_State_sensor_queue_elem_output_event output_event;
	struct System_State_sensor_queue_elem_unhandled_input unhandled_input;
};
struct Events_Ty_elem { /* user defined type */
	union Events_Ty_elem_data data;
	int selection;
};
struct Observable_Event { /* user defined type */
	union Observable_Event_data data;
	int selection;
};
struct System_State_actuator_queue_elem { /* user defined type */
	union System_State_actuator_queue_elem_data data;
	int selection;
};
struct System_State_controlsystem_queue_elem { /* user defined type */
	union System_State_controlsystem_queue_elem_data data;
	int selection;
};
struct System_State_host_queue_elem { /* user defined type */
	union System_State_host_queue_elem_data data;
	int selection;
};
struct System_State_sensor_queue_elem { /* user defined type */
	union System_State_sensor_queue_elem_data data;
	int selection;
};
struct Events_Ty { /* user defined type */
	struct Events_Ty_elem data[10];
	int length;
};
struct System_State_actuator_queue { /* user defined type */
	struct System_State_actuator_queue_elem data[2];
	int length;
};
struct System_State_controlsystem_queue { /* user defined type */
	struct System_State_controlsystem_queue_elem data[2];
	int length;
};
struct System_State_host_queue { /* user defined type */
	struct System_State_host_queue_elem data[2];
	int length;
};
struct System_State_sensor_queue { /* user defined type */
	struct System_State_sensor_queue_elem data[2];
	int length;
};
struct System_State { /* user defined type */
	struct System_State_timers timers;
	struct System_State_actuator_queue actuator_queue;
	struct System_State_controlsystem_queue controlsystem_queue;
	struct System_State_host_queue host_queue;
	struct System_State_sensor_queue sensor_queue;
	struct System_State_actuator actuator;
	struct System_State_controlsystem controlsystem;
	struct System_State_demo_timer_manager demo_timer_manager;
	struct System_State_host host;
	struct System_State_sensor sensor;
};
struct system_state { /* user defined type */
	struct Actuator_Context actuator;
	struct Host_Context host;
	struct Sensor_Context sensor;
	struct Controlsystem_Context controlsystem;
	struct AggregateTimerData timers;
};
#define Pinit	((P7 *)_this)
typedef struct P7 { /* :init: */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	int _1211_8_8_12_transition_id;
	int _1211_8_9_15_transition_id;
	int _1211_8_10_20_transition_id;
	int _1211_8_11_22_transition_id;
} P7;
#define Air7	(sizeof(P7) - Offsetof(P7, _1211_8_11_22_transition_id) - 1*sizeof(int))

#define PSensor_trigger	((P6 *)_this)
typedef struct P6 { /* Sensor_trigger */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	int _1210_7_6_9_transition_id;
} P6;
#define Air6	(sizeof(P6) - Offsetof(P6, _1210_7_6_9_transition_id) - 1*sizeof(int))

#define PSensor_tc	((P5 *)_this)
typedef struct P5 { /* Sensor_tc */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P5;
#define Air5	(sizeof(P5) - 3)

#define PHost_tm	((P4 *)_this)
typedef struct P4 { /* Host_tm */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P4;
#define Air4	(sizeof(P4) - 3)

#define PControlsystem_reading	((P3 *)_this)
typedef struct P3 { /* Controlsystem_reading */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	int _1207_4_3_8_transition_id;
} P3;
#define Air3	(sizeof(P3) - Offsetof(P3, _1207_4_3_8_transition_id) - 1*sizeof(int))

#define PControlsystem_configuration	((P2 *)_this)
typedef struct P2 { /* Controlsystem_configuration */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	int _1206_3_2_6_transition_id;
} P2;
#define Air2	(sizeof(P2) - Offsetof(P2, _1206_3_2_6_transition_id) - 1*sizeof(int))

#define PActuator_controlSignal	((P1 *)_this)
typedef struct P1 { /* Actuator_controlSignal */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
	int _1205_2_1_2_transition_id;
	int _1205_2_1_4_transition_id;
} P1;
#define Air1	(sizeof(P1) - Offsetof(P1, _1205_2_1_4_transition_id) - 1*sizeof(int))

#define Ptimer_manager_proc	((P0 *)_this)
typedef struct P0 { /* timer_manager_proc */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P0;
#define Air0	(sizeof(P0) - 3)

typedef struct P8 { /* np_ */
	unsigned _pid : 8;  /* 0..255 */
	unsigned _t   : 5; /* proctype */
	unsigned _p   : 9; /* state    */
#ifdef HAS_PRIORITY
	unsigned _priority : 8; /* 0..255 */
#endif
} P8;
#define Air8	(sizeof(P8) - 3)

#define Pclaim	P0
#ifndef NCLAIMS
	#define NCLAIMS 1
#endif
#if defined(BFS) && defined(REACH)
	#undef REACH
#endif
#ifdef VERI
	#define BASE	1
#else
	#define BASE	0
#endif
typedef struct Trans {
	short atom;	/* if &2 = atomic trans; if &8 local */
#ifdef HAS_UNLESS
	short escp[HAS_UNLESS];	/* lists the escape states */
	short e_trans;	/* if set, this is an escp-trans */
#endif
	short tpe[2];	/* class of operation (for reduction) */
	short qu[6];	/* for conditional selections: qid's  */
	uchar ty[6];	/* ditto: type's */
#ifdef NIBIS
	short om;	/* completion status of preselects */
#endif
	char *tp;	/* src txt of statement */
	int st;		/* the nextstate */
	int t_id;	/* transition id, unique within proc */
	int forw;	/* index forward transition */
	int back;	/* index return  transition */
	struct Trans *nxt;
} Trans;

#ifdef TRIX
	#define qptr(x)	(channels[x]->body)
	#define pptr(x)	(processes[x]->body)
#else
	#define qptr(x)	(((uchar *)&now)+(int)q_offset[x])
	#define pptr(x)	(((uchar *)&now)+(int)proc_offset[x])
#endif
extern uchar *Pptr(int);
extern uchar *Qptr(int);
#define q_sz(x)	(((Q0 *)qptr(x))->Qlen)

#ifdef TRIX
	#ifdef VECTORSZ
		#undef VECTORSZ
	#endif
	#if WS==4
		#define VECTORSZ	2056	/* ((MAXPROC+MAXQ+4)*sizeof(uchar *)) */
	#else
		#define VECTORSZ	4112	/* the formula causes probs in preprocessing */
	#endif
#else
	#ifndef VECTORSZ
		#define VECTORSZ	1024	/* sv size in bytes */
	#endif
#endif

#define MAXQ   	255
#define MAXPROC	255

#ifdef VERBOSE
	#ifndef CHECK
		#define CHECK
	#endif
	#ifndef DEBUG
		#define DEBUG
	#endif
#endif
#ifdef SAFETY
	#ifndef NOFAIR
		#define NOFAIR
	#endif
#endif
#ifdef NOREDUCE
	#ifndef XUSAFE
		#define XUSAFE
	#endif
	#if !defined(SAFETY) && !defined(MA)
		#define FULLSTACK
	#endif
#else
	#ifdef BITSTATE
		#if defined(SAFETY) && WS<=4
			#define CNTRSTACK
		#else
			#define FULLSTACK
		#endif
	#else
		#define FULLSTACK
	#endif
#endif
#ifdef BITSTATE
	#ifndef NOCOMP
		#define NOCOMP
	#endif
	#if !defined(LC) && defined(SC)
		#define LC
	#endif
#endif
#if defined(COLLAPSE2) || defined(COLLAPSE3) || defined(COLLAPSE4)
	/* accept the above for backward compatibility */
	#define COLLAPSE
#endif
#ifdef HC
	#undef HC
	#define HC4
#endif
#if defined(HC0) || defined(HC1) || defined(HC2) || defined(HC3) || defined(HC4)
	#define HC	4
#endif

typedef struct _Stack  {	 /* for queues and processes */
#if VECTORSZ>32000
	int o_delta;
	#ifndef TRIX
		int o_offset;
		int o_skip;
	#endif
	int o_delqs;
#else
	short o_delta;
	#ifndef TRIX
		short o_offset;
		short o_skip;
	#endif
	short o_delqs;
#endif
	short o_boq;
#ifdef TRIX
	short parent;
	char *b_ptr;
#else
	char *body;
#endif
#ifndef XUSAFE
	char *o_name;
#endif
	struct _Stack *nxt;
	struct _Stack *lst;
} _Stack;

typedef struct Svtack { /* for complete state vector */
#if VECTORSZ>32000
	int o_delta;
	int m_delta;
#else
	short o_delta;	 /* current size of frame */
	short m_delta;	 /* maximum size of frame */
#endif
#if SYNC
	short o_boq;
#endif
#define StackSize	(WS)
	char *body;
	struct Svtack *nxt;
	struct Svtack *lst;
} Svtack;

typedef struct State {
	uchar _nr_pr;
	uchar _nr_qs;
	uchar   _a_t;	/* cycle detection */
#ifndef NOFAIR
	uchar   _cnt[NFAIR];	/* counters, weak fairness */
#endif
#ifndef NOVSZ
#if VECTORSZ<65536
	unsigned short _vsz;
#else
	ulong  _vsz;
#endif
#endif
#ifdef HAS_LAST
	uchar  _last;	/* pid executed in last step */
#endif
#if defined(BITSTATE) && defined(BCS) && defined(STORE_CTX)
	uchar  _ctx;	/* nr of context switches so far */
#endif
#if defined(BFS_PAR) && defined(L_BOUND)
	uchar  _l_bnd;	/* bounded liveness */
	uchar *_l_sds;	/* seed state */
#endif
#ifdef EVENT_TRACE
	#if nstates_event<256
		uchar _event;
	#else
		unsigned short _event;
	#endif
#endif
	uchar Actuator_controlSignal_channel;
	uchar Controlsystem_configuration_channel;
	uchar Controlsystem_reading_channel;
	uchar Host_tm_channel;
	uchar Sensor_tc_channel;
	uchar Sensor_trigger_channel;
	uchar Sensor_lock;
	uchar Actuator_lock;
	uchar Controlsystem_lock;
	uchar Host_lock;
	int inited;
	struct Demo_Timer_Manager_Periodic_Events demo_timer_manager_periodic_events;
	struct Observable_Event nothing;
	struct PidConfig Controlsystem_configuration_signal_parameter;
	struct system_state global_state;
	float Actuator_controlsignal_signal_parameter;
	float Controlsystem_reading_signal_parameter;
#ifdef TRIX
	/* room for 512 proc+chan ptrs, + safety margin */
	char *_ids_[MAXPROC+MAXQ+4];
#else
	uchar sv[VECTORSZ];
#endif
} State;

#ifdef TRIX
typedef struct TRIX_v6 {
	uchar *body; /* aligned */
#ifndef BFS
	short modified;
#endif
	short psize;
	short parent_pid;
	struct TRIX_v6 *nxt;
} TRIX_v6;
#endif

#define HAS_TRACK	0
/* hidden variable: */	uchar Actuator_controlsignal_channel_used;
/* hidden variable: */	uchar Controlsystem_configuration_channel_used;
/* hidden variable: */	uchar Controlsystem_reading_channel_used;
#define FORWARD_MOVES	"pan.m"
#define BACKWARD_MOVES	"pan.b"
#define TRANSITIONS	"pan.t"
#define _NP_	8
#define _nstates8	3 /* np_ */
#define _endstate8	2 /* np_ */

#define _start8	0 /* np_ */
#define _start7	202
#define _start6	1
#define _start5	1
#define _start4	1
#define _start3	1
#define _start2	1
#define _start1	1
#define _start0	1
#ifdef NP
	#define ACCEPT_LAB	1 /* at least 1 in np_ */
#else
	#define ACCEPT_LAB	0 /* user-defined accept labels */
#endif
#ifdef MEMCNT
	#ifdef MEMLIM
		#warning -DMEMLIM takes precedence over -DMEMCNT
		#undef MEMCNT
	#else
		#if MEMCNT<20
			#warning using minimal value -DMEMCNT=20 (=1MB)
			#define MEMLIM	(1)
			#undef MEMCNT
		#else
			#if MEMCNT==20
				#define MEMLIM	(1)
				#undef MEMCNT
			#else
			 #if MEMCNT>=50
				#error excessive value for MEMCNT
			 #else
				#define MEMLIM	(1<<(MEMCNT-20))
			 #endif
			#endif
		#endif
	#endif
#endif
#if NCORE>1 && !defined(MEMLIM)
	#define MEMLIM	(2048)	/* need a default, using 2 GB */
#endif
#define PROG_LAB	0 /* progress labels */
#define NQS	10
typedef struct Q10 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q10;
typedef struct Q9 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q9;
typedef struct Q8 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q8;
typedef struct Q7 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q7;
typedef struct Q6 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q6;
typedef struct Q5 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q5;
typedef struct Q4 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		int fld0;
	} contents[1];
} Q4;
typedef struct Q3 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		float fld0;
	} contents[1];
} Q3;
typedef struct Q2 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		float fld0;
		float fld1;
		float fld2;
	} contents[1];
} Q2;
typedef struct Q1 {
	uchar Qlen;	/* q_size */
	uchar _t;	/* q_type */
	struct {
		float fld0;
	} contents[1];
} Q1;
typedef struct Q0 {	/* generic q */
	uchar Qlen;	/* q_size */
	uchar _t;
} Q0;

/** function prototypes **/
char *emalloc(ulong);
char *Malloc(ulong);
int Boundcheck(int, int, int, int, Trans *);
int addqueue(int, int, int);
/* int atoi(char *); */
/* int abort(void); */
int close(int);
int delproc(int, int);
int endstate(void);
int find_claim(char *);
int h_store(char *, int);
int pan_rand(void);
int q_cond(short, Trans *);
int q_full(int);
int q_len(int);
int q_zero(int);
double qrecv(int, int, int, int);
int unsend(int);
/* void *sbrk(int); */
void spin_assert(int, char *, int, int, Trans *);
#ifdef BFS_PAR
void bfs_printf(const char *, ...);
volatile uchar *sh_pre_malloc(ulong);
#endif
void c_chandump(int);
void c_globals(void);
void c_locals(int, int);
void checkcycles(void);
void crack(int, int, Trans *, short *);
void d_sfh(uchar *, int);
void d_hash(uchar *, int);
void m_hash(uchar *, int);
void s_hash(uchar *, int);
void delq(int);
void dot_crack(int, int, Trans *);
void do_reach(void);
void pan_exit(int);
void exit(int);
#ifdef BFS_PAR
	void bfs_setup_mem(void);
#endif
#ifdef BITSTATE
	void sinit(void);
#else
	void hinit(void);
#endif
void imed(Trans *, int, int, int);
void new_state(void);
void p_restor(int);
void putpeg(int, int);
void putrail(void);
void q_restor(void);
void retrans(int, int, int, short *, uchar *, uchar *);
void select_claim(int);
void settable(void);
void setq_claim(int, int, char *, int, char *);
void sv_restor(void);
void sv_save(void);
void tagtable(int, int, int, short *, uchar *);
void do_dfs(int, int, int, short *, uchar *, uchar *);
void unrecv(int, int, int, double, int);
void usage(FILE *);
void wrap_stats(void);

#ifdef MA
	int g_store(char *, int, uchar);
#endif
#if defined(FULLSTACK) && defined(BITSTATE)
	int  onstack_now(void);
	void onstack_init(void);
	void onstack_put(void);
	void onstack_zap(void);
#endif
#ifndef XUSAFE
	int q_S_check(int, int);
	int q_R_check(int, int);
	extern uchar q_claim[MAXQ+1];
	extern char *q_name[MAXQ+1];
	extern char *p_name[MAXPROC+1];
#endif

#ifndef NO_V_PROVISO
	#define V_PROVISO
#endif
#if !defined(NO_RESIZE) && !defined(AUTO_RESIZE) && !defined(BITSTATE) && !defined(SPACE) && !defined(USE_TDH) && NCORE==1
	#define AUTO_RESIZE
#endif

typedef struct Trail Trail;
typedef struct H_el  H_el;

struct H_el {
	H_el *nxt;
	#ifdef FULLSTACK
		unsigned int tagged;
		#if defined(BITSTATE) && !defined(NOREDUCE) && !defined(SAFETY)
			unsigned int proviso;
		#endif
	#endif
	#if defined(CHECK) || (defined(COLLAPSE) && !defined(FULLSTACK))
		ulong st_id;
	#endif
	#if !defined(SAFETY) || defined(REACH)
		uint D;
	#endif
	#ifdef BCS
		#ifndef CONSERVATIVE
			#define CONSERVATIVE	1 /* good for up to 8 processes */
		#endif
		#ifdef CONSERVATIVE
			#if CONSERVATIVE <= 0 || CONSERVATIVE>32
			#error sensible values for CONSERVATIVE are 1..32 (256/8 = 32)
			#endif
			uchar ctx_pid[CONSERVATIVE];
		#endif
		uchar ctx_low;
	#endif
	#if NCORE>1
		/* could cost 1 extra word: 4 bytes if 32-bit and 8 bytes if 64-bit */
		#ifdef V_PROVISO
			uchar	cpu_id;		/* id of cpu that created the state */
		#endif
	#endif
	#ifdef COLLAPSE
		#if VECTORSZ<65536
			ushort ln;
		#else
			ulong ln;
		#endif
	#endif
	#if defined(AUTO_RESIZE) && !defined(BITSTATE)
		ulong m_K1;
	#endif
	ulong state;
};

#ifdef BFS_PAR
typedef struct BFS_Trail BFS_Trail;
struct BFS_Trail {
	H_el *ostate;
	int   st;
	int   o_tt;
	T_ID  t_id;
	uchar pr;
	uchar o_pm;
	uchar tau;
};
	#if SYNC>0
		#undef BFS_NOTRAIL
	#endif
#endif

#ifdef RHASH
	#ifndef PERMUTED
	#define PERMUTED
	#endif
#endif

struct Trail {
	int   st;	/* current state */
	int   o_tt;
#ifdef PERMUTED
	uint  seed;
	uchar oII;
#endif
	uchar pr;	/* process id */
	uchar tau;	/* 8 bit-flags */
	uchar o_pm;	/* 8 more bit-flags */

	#if 0
	Meaning of bit-flags on tau and o_pm:
	tau&1   -> timeout enabled
	tau&2   -> request to enable timeout 1 level up (in claim)
	tau&4   -> current transition is a  claim move
	tau&8   -> current transition is an atomic move
	tau&16  -> last move was truncated on stack
	tau&32  -> current transition is a preselected move
	tau&64  -> at least one next state is not on the stack
	tau&128 -> current transition is a stutter move
	o_pm&1	-> the current pid moved -- implements else
	o_pm&2	-> this is an acceptance state
	o_pm&4	-> this is a  progress state
	o_pm&8	-> fairness alg rule 1 undo mark
	o_pm&16	-> fairness alg rule 3 undo mark
	o_pm&32	-> fairness alg rule 2 undo mark
	o_pm&64 -> the current proc applied rule2
	o_pm&128 -> a fairness, dummy move - all procs blocked
	#endif

	#ifdef NSUCC
	 uchar n_succ;	/* nr of successor states */
	#endif
	#if defined(FULLSTACK) && defined(MA) && !defined(BFS)
	 uchar proviso;
	#endif
	#ifndef BFS
	 uchar  o_n, o_ot;	/* to save locals */
	#endif
	uchar  o_m;
	#ifdef EVENT_TRACE
		#if nstates_event<256
		 uchar o_event;
		#else
		 unsigned short o_event;
		#endif
	#endif
	#ifndef BFS
		short o_To;
		#if defined(T_RAND) || defined(RANDOMIZE)
		 short oo_i;
		#endif
	#endif
	#if defined(HAS_UNLESS) && !defined(BFS)
	 int e_state;	/* if escape trans - state of origin */
	#endif
	#if (defined(FULLSTACK) && !defined(MA)) || defined(BFS) || (NCORE>1)
	 H_el *ostate;	/* pointer to stored state */
	#endif
	#if defined(CNTRSTACK) && !defined(BFS)
	 long	j6, j7;
	#endif
	Trans *o_t;
	#if !defined(BFS) && !defined(TRIX_ORIG)
	 char *p_bup;
	 char *q_bup;
	#endif
	#ifdef BCS
	 unsigned short sched_limit;
	 unsigned char  bcs; /* phase 1 or 2, or forced 4 */
	 unsigned char  b_pno; /* preferred pid */
	#endif
	#ifdef P_RAND
	 unsigned char p_left;	/* nr of procs left to explore */
	 short p_skip;	/* to find starting point in list */
	#endif
	#ifdef HAS_SORTED
	 short ipt;
	#endif
	#ifdef HAS_PRIORITY
	 short o_priority;
	#endif
	union {
	 double oval;
	 double *ovals;
	} bup;
}; /* end of struct Trail */

#ifdef BFS
	#define Q_PROVISO
	#ifndef INLINE_REV
		#define INLINE_REV
	#endif

typedef struct SV_Hold {
	State *sv;
 #ifndef BFS_PAR
	int  sz;
 #endif
	struct SV_Hold *nxt;
} SV_Hold;
#if !defined(BFS_PAR) || NRUNS>0
	typedef struct EV_Hold {
	 #if !defined(BFS_PAR) || (!defined(NOCOMP) && !defined(HC) && NRUNS>0)
		char *sv;	/* Mask */
	 #endif
	 #if VECTORSZ<65536
		ushort sz;	/* vsize */
	 #else
		ulong  sz;
	 #endif
	 #ifdef BFS_PAR
		uchar owner;
	 #endif
		uchar nrpr;
		uchar nrqs;
		#if !defined(BFS_PAR) || (!defined(TRIX) && NRUNS>0)
			char *po, *qo;
			char *ps, *qs;
		#endif
		struct EV_Hold *nxt;
	} EV_Hold;
#endif
typedef struct BFS_State {
 #ifdef BFS_PAR
	BFS_Trail *t_info;
	State	*osv;
 #else
	Trail	*frame;
	SV_Hold *onow;
 #endif
 #if !defined(BFS_PAR) || NRUNS>0
	EV_Hold *omask;
 #endif
 #if defined(Q_PROVISO) && !defined(NOREDUCE)
	H_el *lstate;
 #endif
 #if !defined(BFS_PAR) || SYNC>0
	short boq;
 #endif
 #ifdef VERBOSE
	ulong nr;
 #endif
 #ifndef BFS_PAR
	struct BFS_State *nxt;
 #endif
} BFS_State;
#endif

void qsend(int, int, double, double, double, int);

#define Addproc(x,y)	addproc(256, y, x)
#define LOCAL	1
#define Q_FULL_F	2
#define Q_EMPT_F	3
#define Q_EMPT_T	4
#define Q_FULL_T	5
#define TIMEOUT_F	6
#define GLOBAL	7
#define BAD	8
#define ALPHA_F	9
#define NTRANS	205
#if defined(BFS_PAR) || NCORE>1
	void e_critical(int);
	void x_critical(int);
	#ifdef BFS_PAR
		void bfs_main(int, int);
		void bfs_report_mem(void);
	#endif
#endif


/* end of PAN_H */
#endif
