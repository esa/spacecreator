#define rand	pan_rand
#define pthread_equal(a,b)	((a)==(b))
#if defined(HAS_CODE) && defined(VERBOSE)
	#ifdef BFS_PAR
		bfs_printf("Pr: %d Tr: %d\n", II, t->forw);
	#else
		cpu_printf("Pr: %d Tr: %d\n", II, t->forw);
	#endif
#endif
	switch (t->forw) {
	default: Uerror("bad forward move");
	case 0:	/* if without executable clauses */
		continue;
	case 1: /* generic 'goto' or 'skip' */
		IfNotBlocked
		_m = 3; goto P999;
	case 2: /* generic 'else' */
		IfNotBlocked
		if (trpt->o_pm&1) continue;
		_m = 3; goto P999;

		 /* PROC :init: */
	case 3: // STATE 13 - dataview.pml:19091 - [D_STEP19091]
		IfNotBlocked

		reached[7][13] = 1;
		reached[7][t->st] = 1;
		reached[7][tt] = 1;

		if (TstOnly) return 1;


		reached[7][13] = 1;
		reached[7][t->st] = 1;
		reached[7][tt] = 1;

		sv_save();
		S_374_0: /* 2 */
S_373_0: /* 2 */
S_369_0: /* 2 */
S_366_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.demo_timer_manager_periodic_events.data[0].period = 10;
#ifdef VAR_RANGES
		logval("demo_timer_manager_periodic_events.data[0].period", now.demo_timer_manager_periodic_events.data[0].period);
#endif
		;
S_368_0: /* 2 */
S_367_0: /* 2 */
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.demo_timer_manager_periodic_events.data[0].period>=0)&&(now.demo_timer_manager_periodic_events.data[0].period<=500000)), "((demo_timer_manager_periodic_events.data[0].period>=0)&&(demo_timer_manager_periodic_events.data[0].period<=500000))", II, tt, t);
		goto S_370_0;
S_370_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.demo_timer_manager_periodic_events.data[0].event.selection = 1;
#ifdef VAR_RANGES
		logval("demo_timer_manager_periodic_events.data[0].event.selection", now.demo_timer_manager_periodic_events.data[0].event.selection);
#endif
		;
S_371_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.demo_timer_manager_periodic_events.data[0].event.data.msg_in.selection = 2;
#ifdef VAR_RANGES
		logval("demo_timer_manager_periodic_events.data[0].event.data.msg_in.selection", now.demo_timer_manager_periodic_events.data[0].event.data.msg_in.selection);
#endif
		;
S_372_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.demo_timer_manager_periodic_events.length = 1;
#ifdef VAR_RANGES
		logval("demo_timer_manager_periodic_events.length", now.demo_timer_manager_periodic_events.length);
#endif
		;
		goto S_377_0;
S_377_0: /* 2 */
S_376_0: /* 2 */
S_375_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.nothing.selection = 1;
#ifdef VAR_RANGES
		logval("nothing.selection", now.nothing.selection);
#endif
		;
		goto S_423_0;
S_423_0: /* 1 */

#if defined(C_States) && (HAS_TRACK==1)
		c_update((uchar *) &(now.c_state[0]));
#endif
		_m = 3; goto P999;

	case 4: // STATE 15 - dataview.pml:8596 - [global_state.sensor.sender = 4] (0:0:1 - 1)
		IfNotBlocked
		reached[7][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.sensor.sender;
		now.global_state.sensor.sender = 4;
#ifdef VAR_RANGES
		logval("global_state.sensor.sender", now.global_state.sensor.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 5: // STATE 16 - dataview.pml:8601 - [assert((((((global_state.sensor.sender==0)||(global_state.sensor.sender==1))||(global_state.sensor.sender==2))||(global_state.sensor.sender==3))||(global_state.sensor.sender==4)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][16] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((now.global_state.sensor.sender==0)||(now.global_state.sensor.sender==1))||(now.global_state.sensor.sender==2))||(now.global_state.sensor.sender==3))||(now.global_state.sensor.sender==4)), "(((((global_state.sensor.sender==0)||(global_state.sensor.sender==1))||(global_state.sensor.sender==2))||(global_state.sensor.sender==3))||(global_state.sensor.sender==4))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 6: // STATE 19 - sensor.pml:4 - [transition_id = 0] (0:54:2 - 1)
		IfNotBlocked
		reached[7][19] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_8_12_transition_id;
		((P7 *)_this)->_1211_8_8_12_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_8_12_transition_id);
#endif
		;
		/* merge: transition_id = 0(54, 20, 54) */
		reached[7][20] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1211_8_8_12_transition_id;
		((P7 *)_this)->_1211_8_8_12_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_8_12_transition_id);
#endif
		;
		/* merge: .(goto)(0, 55, 54) */
		reached[7][55] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 7: // STATE 21 - sensor.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][21] = 1;
		if (!((((P7 *)_this)->_1211_8_8_12_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_8_12_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_8_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_8_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 8: // STATE 23 - sensor.pml:9 - [((transition_id==0))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][23] = 1;
		if (!((((P7 *)_this)->_1211_8_8_12_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_8_12_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_8_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_8_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 9: // STATE 24 - dataview.pml:8496 - [global_state.sensor.x = 1.000000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.sensor.x;
		now.global_state.sensor.x = 1.000000;
#ifdef VAR_RANGES
		logval("global_state.sensor.x", now.global_state.sensor.x);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 10: // STATE 25 - dataview.pml:8501 - [assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.sensor.x>= -(1000))&&(now.global_state.sensor.x<=1000)), "((global_state.sensor.x>= -(1000))&&(global_state.sensor.x<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 11: // STATE 28 - system.pml:35 - [global_state.timers.sensor.trigger.interval = 1000] (0:53:4 - 1)
		IfNotBlocked
		reached[7][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(4);
		(trpt+1)->bup.ovals[0] = now.global_state.timers.sensor.trigger.interval;
		now.global_state.timers.sensor.trigger.interval = 1000;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.interval", now.global_state.timers.sensor.trigger.interval);
#endif
		;
		/* merge: global_state.timers.sensor.trigger.timer_enabled = 1(53, 29, 53) */
		reached[7][29] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.timers.sensor.trigger.timer_enabled;
		now.global_state.timers.sensor.trigger.timer_enabled = 1;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.timer_enabled", ((int)now.global_state.timers.sensor.trigger.timer_enabled));
#endif
		;
		/* merge: printf('set_timer Sensor trigger %d\\n',1000)(53, 30, 53) */
		reached[7][30] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("set_timer Sensor trigger %d\n", 1000);
		/* merge: transition_id = -(1)(53, 32, 53) */
		reached[7][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((P7 *)_this)->_1211_8_8_12_transition_id;
		((P7 *)_this)->_1211_8_8_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_8_12_transition_id);
#endif
		;
		/* merge: global_state.sensor.state = 0(53, 33, 53) */
		reached[7][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = now.global_state.sensor.state;
		now.global_state.sensor.state = 0;
#ifdef VAR_RANGES
		logval("global_state.sensor.state", now.global_state.sensor.state);
#endif
		;
		/* merge: goto continuous_signals(0, 34, 53) */
		reached[7][34] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 12: // STATE 35 - sensor.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][35] = 1;
		if (!((((P7 *)_this)->_1211_8_8_12_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_8_12_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_8_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_8_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 13: // STATE 38 - dataview.pml:8496 - [global_state.sensor.x = (global_state.sensor.x*2.000000)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.sensor.x;
		now.global_state.sensor.x = (now.global_state.sensor.x*2.000000);
#ifdef VAR_RANGES
		logval("global_state.sensor.x", now.global_state.sensor.x);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 14: // STATE 39 - dataview.pml:8501 - [assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][39] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.sensor.x>= -(1000))&&(now.global_state.sensor.x<=1000)), "((global_state.sensor.x>= -(1000))&&(global_state.sensor.x<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 15: // STATE 42 - system.pml:64 - [Controlsystem_reading_channel!global_state.sensor.x] (0:0:0 - 1)
		IfNotBlocked
		reached[7][42] = 1;
		if (q_full(now.Controlsystem_reading_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Controlsystem_reading_channel);
		sprintf(simtmp, "%f,", now.global_state.sensor.x); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Controlsystem_reading_channel, 0, now.global_state.sensor.x, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 16: // STATE 44 - system.pml:35 - [global_state.timers.sensor.trigger.interval = 1000] (0:53:4 - 1)
		IfNotBlocked
		reached[7][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(4);
		(trpt+1)->bup.ovals[0] = now.global_state.timers.sensor.trigger.interval;
		now.global_state.timers.sensor.trigger.interval = 1000;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.interval", now.global_state.timers.sensor.trigger.interval);
#endif
		;
		/* merge: global_state.timers.sensor.trigger.timer_enabled = 1(53, 45, 53) */
		reached[7][45] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.timers.sensor.trigger.timer_enabled;
		now.global_state.timers.sensor.trigger.timer_enabled = 1;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.timer_enabled", ((int)now.global_state.timers.sensor.trigger.timer_enabled));
#endif
		;
		/* merge: printf('set_timer Sensor trigger %d\\n',1000)(53, 46, 53) */
		reached[7][46] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("set_timer Sensor trigger %d\n", 1000);
		/* merge: transition_id = -(1)(53, 48, 53) */
		reached[7][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((P7 *)_this)->_1211_8_8_12_transition_id;
		((P7 *)_this)->_1211_8_8_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_8_12_transition_id);
#endif
		;
		/* merge: global_state.sensor.state = 0(53, 49, 53) */
		reached[7][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = now.global_state.sensor.state;
		now.global_state.sensor.state = 0;
#ifdef VAR_RANGES
		logval("global_state.sensor.state", now.global_state.sensor.state);
#endif
		;
		/* merge: goto continuous_signals(0, 50, 53) */
		reached[7][50] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 17: // STATE 59 - system.pml:250 - [Sensor_lock!1] (0:0:0 - 3)
		IfNotBlocked
		reached[7][59] = 1;
		if (q_full(now.Sensor_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Sensor_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Sensor_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 18: // STATE 60 - dataview.pml:8596 - [global_state.actuator.sender = 4] (0:0:1 - 1)
		IfNotBlocked
		reached[7][60] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.sender;
		now.global_state.actuator.sender = 4;
#ifdef VAR_RANGES
		logval("global_state.actuator.sender", now.global_state.actuator.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 19: // STATE 61 - dataview.pml:8601 - [assert((((((global_state.actuator.sender==0)||(global_state.actuator.sender==1))||(global_state.actuator.sender==2))||(global_state.actuator.sender==3))||(global_state.actuator.sender==4)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][61] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((now.global_state.actuator.sender==0)||(now.global_state.actuator.sender==1))||(now.global_state.actuator.sender==2))||(now.global_state.actuator.sender==3))||(now.global_state.actuator.sender==4)), "(((((global_state.actuator.sender==0)||(global_state.actuator.sender==1))||(global_state.actuator.sender==2))||(global_state.actuator.sender==3))||(global_state.actuator.sender==4))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 20: // STATE 64 - dataview.pml:8496 - [global_state.actuator.setting = 0.000000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.setting;
		now.global_state.actuator.setting = 0.000000;
#ifdef VAR_RANGES
		logval("global_state.actuator.setting", now.global_state.actuator.setting);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 21: // STATE 65 - dataview.pml:8501 - [assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][65] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.setting>= -(1000))&&(now.global_state.actuator.setting<=1000)), "((global_state.actuator.setting>= -(1000))&&(global_state.actuator.setting<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 22: // STATE 68 - actuator.pml:4 - [transition_id = 0] (0:96:2 - 1)
		IfNotBlocked
		reached[7][68] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_9_15_transition_id;
		((P7 *)_this)->_1211_8_9_15_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_9_15_transition_id);
#endif
		;
		/* merge: transition_id = 0(96, 69, 96) */
		reached[7][69] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1211_8_9_15_transition_id;
		((P7 *)_this)->_1211_8_9_15_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_9_15_transition_id);
#endif
		;
		/* merge: .(goto)(0, 97, 96) */
		reached[7][97] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 23: // STATE 70 - actuator.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][70] = 1;
		if (!((((P7 *)_this)->_1211_8_9_15_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_9_15_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_9_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_9_15_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 24: // STATE 72 - actuator.pml:9 - [((transition_id==0))] (95:0:3 - 1)
		IfNotBlocked
		reached[7][72] = 1;
		if (!((((P7 *)_this)->_1211_8_9_15_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_9_15_transition_id */  (trpt+1)->bup.ovals = grab_values(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_9_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_9_15_transition_id = 0;
		/* merge: transition_id = -(1)(95, 73, 95) */
		reached[7][73] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1211_8_9_15_transition_id;
		((P7 *)_this)->_1211_8_9_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_9_15_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(95, 74, 95) */
		reached[7][74] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 75, 95) */
		reached[7][75] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 25: // STATE 76 - actuator.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][76] = 1;
		if (!((((P7 *)_this)->_1211_8_9_15_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_9_15_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_9_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_9_15_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 26: // STATE 79 - dataview.pml:8496 - [global_state.actuator.setting = (1.500000*global_state.actuator.inputcontrol)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][79] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.setting;
		now.global_state.actuator.setting = (1.500000*now.global_state.actuator.inputcontrol);
#ifdef VAR_RANGES
		logval("global_state.actuator.setting", now.global_state.actuator.setting);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 27: // STATE 80 - dataview.pml:8501 - [assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][80] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.setting>= -(1000))&&(now.global_state.actuator.setting<=1000)), "((global_state.actuator.setting>= -(1000))&&(global_state.actuator.setting<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 28: // STATE 83 - actuator.pml:17 - [(((global_state.actuator.setting>10.000000)==1))] (95:0:2 - 1)
		IfNotBlocked
		reached[7][83] = 1;
		if (!(((now.global_state.actuator.setting>10.000000)==1)))
			continue;
		/* merge: transition_id = -(1)(95, 84, 95) */
		reached[7][84] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_9_15_transition_id;
		((P7 *)_this)->_1211_8_9_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_9_15_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 1(95, 85, 95) */
		reached[7][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 86, 95) */
		reached[7][86] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 29: // STATE 87 - actuator.pml:21 - [(((global_state.actuator.setting>10.000000)==0))] (95:0:2 - 1)
		IfNotBlocked
		reached[7][87] = 1;
		if (!(((now.global_state.actuator.setting>10.000000)==0)))
			continue;
		/* merge: transition_id = -(1)(95, 88, 95) */
		reached[7][88] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_9_15_transition_id;
		((P7 *)_this)->_1211_8_9_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_9_15_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(95, 89, 95) */
		reached[7][89] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 95) */
		reached[7][90] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 30: // STATE 101 - system.pml:252 - [Actuator_lock!1] (0:0:0 - 3)
		IfNotBlocked
		reached[7][101] = 1;
		if (q_full(now.Actuator_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 31: // STATE 102 - dataview.pml:8596 - [global_state.controlsystem.sender = 4] (0:0:1 - 1)
		IfNotBlocked
		reached[7][102] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.sender;
		now.global_state.controlsystem.sender = 4;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.sender", now.global_state.controlsystem.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 32: // STATE 103 - dataview.pml:8601 - [assert((((((global_state.controlsystem.sender==0)||(global_state.controlsystem.sender==1))||(global_state.controlsystem.sender==2))||(global_state.controlsystem.sender==3))||(global_state.controlsystem.sender==4)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][103] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((now.global_state.controlsystem.sender==0)||(now.global_state.controlsystem.sender==1))||(now.global_state.controlsystem.sender==2))||(now.global_state.controlsystem.sender==3))||(now.global_state.controlsystem.sender==4)), "(((((global_state.controlsystem.sender==0)||(global_state.controlsystem.sender==1))||(global_state.controlsystem.sender==2))||(global_state.controlsystem.sender==3))||(global_state.controlsystem.sender==4))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 33: // STATE 106 - dataview.pml:8496 - [global_state.controlsystem.i = 0.000000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][106] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.i;
		now.global_state.controlsystem.i = 0.000000;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.i", now.global_state.controlsystem.i);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 34: // STATE 107 - dataview.pml:8501 - [assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][107] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.i>= -(1000))&&(now.global_state.controlsystem.i<=1000)), "((global_state.controlsystem.i>= -(1000))&&(global_state.controlsystem.i<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 35: // STATE 110 - dataview.pml:8496 - [global_state.controlsystem.d = 0.000000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][110] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.d;
		now.global_state.controlsystem.d = 0.000000;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.d", now.global_state.controlsystem.d);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 36: // STATE 111 - dataview.pml:8501 - [assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][111] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.d>= -(1000))&&(now.global_state.controlsystem.d<=1000)), "((global_state.controlsystem.d>= -(1000))&&(global_state.controlsystem.d<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 37: // STATE 114 - dataview.pml:8496 - [global_state.controlsystem.previous = 0.000000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][114] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.previous;
		now.global_state.controlsystem.previous = 0.000000;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.previous", now.global_state.controlsystem.previous);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 38: // STATE 115 - dataview.pml:8501 - [assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][115] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.previous>= -(1000))&&(now.global_state.controlsystem.previous<=1000)), "((global_state.controlsystem.previous>= -(1000))&&(global_state.controlsystem.previous<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 39: // STATE 118 - controlsystem.pml:4 - [transition_id = 0] (0:159:2 - 1)
		IfNotBlocked
		reached[7][118] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_10_20_transition_id;
		((P7 *)_this)->_1211_8_10_20_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_10_20_transition_id);
#endif
		;
		/* merge: transition_id = 0(159, 119, 159) */
		reached[7][119] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1211_8_10_20_transition_id;
		((P7 *)_this)->_1211_8_10_20_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_10_20_transition_id);
#endif
		;
		/* merge: .(goto)(0, 160, 159) */
		reached[7][160] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 40: // STATE 120 - controlsystem.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][120] = 1;
		if (!((((P7 *)_this)->_1211_8_10_20_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_10_20_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_10_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_10_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 41: // STATE 122 - controlsystem.pml:9 - [((transition_id==0))] (158:0:3 - 1)
		IfNotBlocked
		reached[7][122] = 1;
		if (!((((P7 *)_this)->_1211_8_10_20_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_10_20_transition_id */  (trpt+1)->bup.ovals = grab_values(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_10_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_10_20_transition_id = 0;
		/* merge: transition_id = -(1)(158, 123, 158) */
		reached[7][123] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1211_8_10_20_transition_id;
		((P7 *)_this)->_1211_8_10_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_10_20_transition_id);
#endif
		;
		/* merge: global_state.controlsystem.state = 1(158, 124, 158) */
		reached[7][124] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.controlsystem.state;
		now.global_state.controlsystem.state = 1;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.state", now.global_state.controlsystem.state);
#endif
		;
		/* merge: goto continuous_signals(0, 125, 158) */
		reached[7][125] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 42: // STATE 126 - controlsystem.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][126] = 1;
		if (!((((P7 *)_this)->_1211_8_10_20_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_10_20_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_10_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_10_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 43: // STATE 127 - system.pml:74 - [(1)] (158:0:2 - 1)
		IfNotBlocked
		reached[7][127] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(158, 129, 158) */
		reached[7][129] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_10_20_transition_id;
		((P7 *)_this)->_1211_8_10_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_10_20_transition_id);
#endif
		;
		/* merge: global_state.controlsystem.state = 0(158, 130, 158) */
		reached[7][130] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.controlsystem.state;
		now.global_state.controlsystem.state = 0;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.state", now.global_state.controlsystem.state);
#endif
		;
		/* merge: goto continuous_signals(0, 131, 158) */
		reached[7][131] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 44: // STATE 132 - controlsystem.pml:18 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][132] = 1;
		if (!((((P7 *)_this)->_1211_8_10_20_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_10_20_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_10_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_10_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 45: // STATE 135 - dataview.pml:8496 - [global_state.controlsystem.i = (global_state.controlsystem.i+global_state.controlsystem.inputvalue)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][135] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.i;
		now.global_state.controlsystem.i = (now.global_state.controlsystem.i+now.global_state.controlsystem.inputvalue);
#ifdef VAR_RANGES
		logval("global_state.controlsystem.i", now.global_state.controlsystem.i);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 46: // STATE 136 - dataview.pml:8501 - [assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][136] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.i>= -(1000))&&(now.global_state.controlsystem.i<=1000)), "((global_state.controlsystem.i>= -(1000))&&(global_state.controlsystem.i<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 47: // STATE 139 - dataview.pml:8496 - [global_state.controlsystem.d = (global_state.controlsystem.inputvalue-global_state.controlsystem.previous)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][139] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.d;
		now.global_state.controlsystem.d = (now.global_state.controlsystem.inputvalue-now.global_state.controlsystem.previous);
#ifdef VAR_RANGES
		logval("global_state.controlsystem.d", now.global_state.controlsystem.d);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 48: // STATE 140 - dataview.pml:8501 - [assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][140] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.d>= -(1000))&&(now.global_state.controlsystem.d<=1000)), "((global_state.controlsystem.d>= -(1000))&&(global_state.controlsystem.d<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 49: // STATE 143 - dataview.pml:8496 - [global_state.controlsystem.previous = global_state.controlsystem.inputvalue] (0:0:1 - 1)
		IfNotBlocked
		reached[7][143] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.previous;
		now.global_state.controlsystem.previous = now.global_state.controlsystem.inputvalue;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.previous", now.global_state.controlsystem.previous);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 50: // STATE 144 - dataview.pml:8501 - [assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][144] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.previous>= -(1000))&&(now.global_state.controlsystem.previous<=1000)), "((global_state.controlsystem.previous>= -(1000))&&(global_state.controlsystem.previous<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 51: // STATE 148 - dataview.pml:8496 - [global_state.controlsystem.outputvalue = (((global_state.controlsystem.cfg.kp*global_state.controlsystem.inputvalue)+(global_state.controlsystem.cfg.ki*global_state.controlsystem.i))+(global_state.controlsystem.cfg.kd*global_state.controlsystem.d))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.outputvalue;
		now.global_state.controlsystem.outputvalue = (((now.global_state.controlsystem.cfg.kp*now.global_state.controlsystem.inputvalue)+(now.global_state.controlsystem.cfg.ki*now.global_state.controlsystem.i))+(now.global_state.controlsystem.cfg.kd*now.global_state.controlsystem.d));
#ifdef VAR_RANGES
		logval("global_state.controlsystem.outputvalue", now.global_state.controlsystem.outputvalue);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 52: // STATE 149 - dataview.pml:8501 - [assert(((global_state.controlsystem.outputvalue>=-(1000))&&(global_state.controlsystem.outputvalue<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][149] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.outputvalue>= -(1000))&&(now.global_state.controlsystem.outputvalue<=1000)), "((global_state.controlsystem.outputvalue>= -(1000))&&(global_state.controlsystem.outputvalue<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 53: // STATE 152 - system.pml:46 - [Actuator_controlSignal_channel!global_state.controlsystem.outputvalue] (158:0:1 - 1)
		IfNotBlocked
		reached[7][152] = 1;
		if (q_full(now.Actuator_controlSignal_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_controlSignal_channel);
		sprintf(simtmp, "%f,", now.global_state.controlsystem.outputvalue); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_controlSignal_channel, 0, now.global_state.controlsystem.outputvalue, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 154, 158) */
		reached[7][154] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_10_20_transition_id;
		((P7 *)_this)->_1211_8_10_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_10_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 155, 158) */
		reached[7][155] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 54: // STATE 164 - system.pml:254 - [Controlsystem_lock!1] (0:0:0 - 3)
		IfNotBlocked
		reached[7][164] = 1;
		if (q_full(now.Controlsystem_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Controlsystem_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Controlsystem_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 55: // STATE 165 - dataview.pml:8596 - [global_state.host.sender = 4] (0:0:1 - 1)
		IfNotBlocked
		reached[7][165] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.host.sender;
		now.global_state.host.sender = 4;
#ifdef VAR_RANGES
		logval("global_state.host.sender", now.global_state.host.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 56: // STATE 166 - dataview.pml:8601 - [assert((((((global_state.host.sender==0)||(global_state.host.sender==1))||(global_state.host.sender==2))||(global_state.host.sender==3))||(global_state.host.sender==4)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][166] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((now.global_state.host.sender==0)||(now.global_state.host.sender==1))||(now.global_state.host.sender==2))||(now.global_state.host.sender==3))||(now.global_state.host.sender==4)), "(((((global_state.host.sender==0)||(global_state.host.sender==1))||(global_state.host.sender==2))||(global_state.host.sender==3))||(global_state.host.sender==4))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 57: // STATE 169 - host.pml:4 - [transition_id = 0] (0:195:2 - 1)
		IfNotBlocked
		reached[7][169] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_11_22_transition_id;
		((P7 *)_this)->_1211_8_11_22_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_11_22_transition_id);
#endif
		;
		/* merge: transition_id = 0(195, 170, 195) */
		reached[7][170] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1211_8_11_22_transition_id;
		((P7 *)_this)->_1211_8_11_22_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_11_22_transition_id);
#endif
		;
		/* merge: .(goto)(0, 196, 195) */
		reached[7][196] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 58: // STATE 171 - host.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][171] = 1;
		if (!((((P7 *)_this)->_1211_8_11_22_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_11_22_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_11_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_11_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 59: // STATE 173 - host.pml:9 - [((transition_id==0))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][173] = 1;
		if (!((((P7 *)_this)->_1211_8_11_22_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1211_8_11_22_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1211_8_11_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1211_8_11_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 60: // STATE 174 - dataview.pml:8496 - [global_state.host.cfg.kp = 1.300000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][174] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.host.cfg.kp;
		now.global_state.host.cfg.kp = 1.300000;
#ifdef VAR_RANGES
		logval("global_state.host.cfg.kp", now.global_state.host.cfg.kp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 61: // STATE 175 - dataview.pml:8501 - [assert(((global_state.host.cfg.kp>=-(1000))&&(global_state.host.cfg.kp<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][175] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.host.cfg.kp>= -(1000))&&(now.global_state.host.cfg.kp<=1000)), "((global_state.host.cfg.kp>= -(1000))&&(global_state.host.cfg.kp<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 62: // STATE 178 - dataview.pml:8496 - [global_state.host.cfg.kd = 2.100000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][178] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.host.cfg.kd;
		now.global_state.host.cfg.kd = 2.100000;
#ifdef VAR_RANGES
		logval("global_state.host.cfg.kd", now.global_state.host.cfg.kd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 63: // STATE 179 - dataview.pml:8501 - [assert(((global_state.host.cfg.kd>=-(1000))&&(global_state.host.cfg.kd<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][179] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.host.cfg.kd>= -(1000))&&(now.global_state.host.cfg.kd<=1000)), "((global_state.host.cfg.kd>= -(1000))&&(global_state.host.cfg.kd<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 64: // STATE 182 - dataview.pml:8496 - [global_state.host.cfg.ki = 3.200000] (0:0:1 - 1)
		IfNotBlocked
		reached[7][182] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.host.cfg.ki;
		now.global_state.host.cfg.ki = 3.200000;
#ifdef VAR_RANGES
		logval("global_state.host.cfg.ki", now.global_state.host.cfg.ki);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 65: // STATE 183 - dataview.pml:8501 - [assert(((global_state.host.cfg.ki>=-(1000))&&(global_state.host.cfg.ki<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][183] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.host.cfg.ki>= -(1000))&&(now.global_state.host.cfg.ki<=1000)), "((global_state.host.cfg.ki>= -(1000))&&(global_state.host.cfg.ki<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 66: // STATE 187 - system.pml:60 - [Controlsystem_configuration_channel!global_state.host.cfg.kp,global_state.host.cfg.ki,global_state.host.cfg.kd] (194:0:2 - 1)
		IfNotBlocked
		reached[7][187] = 1;
		if (q_full(now.Controlsystem_configuration_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Controlsystem_configuration_channel);
		sprintf(simtmp, "%f,", now.global_state.host.cfg.kp); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%f,", now.global_state.host.cfg.ki); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%f,", now.global_state.host.cfg.kd); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Controlsystem_configuration_channel, 0, now.global_state.host.cfg.kp, now.global_state.host.cfg.ki, now.global_state.host.cfg.kd, 3);
		/* merge: transition_id = -(1)(194, 189, 194) */
		reached[7][189] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1211_8_11_22_transition_id;
		((P7 *)_this)->_1211_8_11_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_1211_8_11_22_transition_id);
#endif
		;
		/* merge: global_state.host.state = 0(194, 190, 194) */
		reached[7][190] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.host.state;
		now.global_state.host.state = 0;
#ifdef VAR_RANGES
		logval("global_state.host.state", now.global_state.host.state);
#endif
		;
		/* merge: goto continuous_signals(0, 191, 194) */
		reached[7][191] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 67: // STATE 200 - system.pml:256 - [Host_lock!1] (203:0:1 - 3)
		IfNotBlocked
		reached[7][200] = 1;
		if (q_full(now.Host_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Host_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Host_lock, 0, 1, 0, 0, 1);
		/* merge: inited = 1(0, 201, 203) */
		reached[7][201] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.inited;
		now.inited = 1;
#ifdef VAR_RANGES
		logval("inited", now.inited);
#endif
		;
		_m = 2; goto P999; /* 1 */
	case 68: // STATE 203 - system.pml:259 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[7][203] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Sensor_trigger */
	case 69: // STATE 1 - system.pml:227 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[6][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 70: // STATE 2 - system.pml:230 - [(nempty(Sensor_trigger_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][2] = 1;
		if (!((q_len(now.Sensor_trigger_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 71: // STATE 3 - system.pml:231 - [Sensor_lock?_] (0:0:1 - 1)
		reached[6][3] = 1;
		if (q_len(now.Sensor_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Sensor_lock, XX-1, 0, 0);
		;
		qrecv(now.Sensor_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Sensor_lock);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 72: // STATE 4 - system.pml:234 - [(nempty(Sensor_trigger_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][4] = 1;
		if (!((q_len(now.Sensor_trigger_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 73: // STATE 5 - system.pml:235 - [Sensor_trigger_channel?_] (0:0:1 - 1)
		reached[6][5] = 1;
		if (q_len(now.Sensor_trigger_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Sensor_trigger_channel, XX-1, 0, 0);
		;
		qrecv(now.Sensor_trigger_channel, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Sensor_trigger_channel);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 74: // STATE 6 - sensor.pml:42 - [((global_state.sensor.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][6] = 1;
		if (!((now.global_state.sensor.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 75: // STATE 7 - sensor.pml:4 - [transition_id = 0] (0:42:2 - 1)
		IfNotBlocked
		reached[6][7] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1210_7_6_9_transition_id;
		((P6 *)_this)->_1210_7_6_9_transition_id = 0;
#ifdef VAR_RANGES
		logval("Sensor_trigger:transition_id", ((P6 *)_this)->_1210_7_6_9_transition_id);
#endif
		;
		/* merge: transition_id = 1(42, 8, 42) */
		reached[6][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P6 *)_this)->_1210_7_6_9_transition_id;
		((P6 *)_this)->_1210_7_6_9_transition_id = 1;
#ifdef VAR_RANGES
		logval("Sensor_trigger:transition_id", ((P6 *)_this)->_1210_7_6_9_transition_id);
#endif
		;
		/* merge: .(goto)(0, 43, 42) */
		reached[6][43] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 76: // STATE 9 - sensor.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][9] = 1;
		if (!((((P6 *)_this)->_1210_7_6_9_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1210_7_6_9_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1210_7_6_9_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1210_7_6_9_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 77: // STATE 11 - sensor.pml:9 - [((transition_id==0))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][11] = 1;
		if (!((((P6 *)_this)->_1210_7_6_9_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1210_7_6_9_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1210_7_6_9_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1210_7_6_9_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 78: // STATE 12 - dataview.pml:8496 - [global_state.sensor.x = 1.000000] (0:0:1 - 1)
		IfNotBlocked
		reached[6][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.sensor.x;
		now.global_state.sensor.x = 1.000000;
#ifdef VAR_RANGES
		logval("global_state.sensor.x", now.global_state.sensor.x);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 79: // STATE 13 - dataview.pml:8501 - [assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][13] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.sensor.x>= -(1000))&&(now.global_state.sensor.x<=1000)), "((global_state.sensor.x>= -(1000))&&(global_state.sensor.x<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 80: // STATE 16 - system.pml:35 - [global_state.timers.sensor.trigger.interval = 1000] (0:41:4 - 1)
		IfNotBlocked
		reached[6][16] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(4);
		(trpt+1)->bup.ovals[0] = now.global_state.timers.sensor.trigger.interval;
		now.global_state.timers.sensor.trigger.interval = 1000;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.interval", now.global_state.timers.sensor.trigger.interval);
#endif
		;
		/* merge: global_state.timers.sensor.trigger.timer_enabled = 1(41, 17, 41) */
		reached[6][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.timers.sensor.trigger.timer_enabled;
		now.global_state.timers.sensor.trigger.timer_enabled = 1;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.timer_enabled", ((int)now.global_state.timers.sensor.trigger.timer_enabled));
#endif
		;
		/* merge: printf('set_timer Sensor trigger %d\\n',1000)(41, 18, 41) */
		reached[6][18] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("set_timer Sensor trigger %d\n", 1000);
		/* merge: transition_id = -(1)(41, 20, 41) */
		reached[6][20] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((P6 *)_this)->_1210_7_6_9_transition_id;
		((P6 *)_this)->_1210_7_6_9_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Sensor_trigger:transition_id", ((P6 *)_this)->_1210_7_6_9_transition_id);
#endif
		;
		/* merge: global_state.sensor.state = 0(41, 21, 41) */
		reached[6][21] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = now.global_state.sensor.state;
		now.global_state.sensor.state = 0;
#ifdef VAR_RANGES
		logval("global_state.sensor.state", now.global_state.sensor.state);
#endif
		;
		/* merge: goto continuous_signals(0, 22, 41) */
		reached[6][22] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 81: // STATE 23 - sensor.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][23] = 1;
		if (!((((P6 *)_this)->_1210_7_6_9_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1210_7_6_9_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1210_7_6_9_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1210_7_6_9_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 82: // STATE 26 - dataview.pml:8496 - [global_state.sensor.x = (global_state.sensor.x*2.000000)] (0:0:1 - 1)
		IfNotBlocked
		reached[6][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.sensor.x;
		now.global_state.sensor.x = (now.global_state.sensor.x*2.000000);
#ifdef VAR_RANGES
		logval("global_state.sensor.x", now.global_state.sensor.x);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 83: // STATE 27 - dataview.pml:8501 - [assert(((global_state.sensor.x>=-(1000))&&(global_state.sensor.x<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][27] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.sensor.x>= -(1000))&&(now.global_state.sensor.x<=1000)), "((global_state.sensor.x>= -(1000))&&(global_state.sensor.x<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 84: // STATE 30 - system.pml:64 - [Controlsystem_reading_channel!global_state.sensor.x] (0:0:0 - 1)
		IfNotBlocked
		reached[6][30] = 1;
		if (q_full(now.Controlsystem_reading_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Controlsystem_reading_channel);
		sprintf(simtmp, "%f,", now.global_state.sensor.x); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Controlsystem_reading_channel, 0, now.global_state.sensor.x, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 85: // STATE 32 - system.pml:35 - [global_state.timers.sensor.trigger.interval = 1000] (0:41:4 - 1)
		IfNotBlocked
		reached[6][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(4);
		(trpt+1)->bup.ovals[0] = now.global_state.timers.sensor.trigger.interval;
		now.global_state.timers.sensor.trigger.interval = 1000;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.interval", now.global_state.timers.sensor.trigger.interval);
#endif
		;
		/* merge: global_state.timers.sensor.trigger.timer_enabled = 1(41, 33, 41) */
		reached[6][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.timers.sensor.trigger.timer_enabled;
		now.global_state.timers.sensor.trigger.timer_enabled = 1;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.timer_enabled", ((int)now.global_state.timers.sensor.trigger.timer_enabled));
#endif
		;
		/* merge: printf('set_timer Sensor trigger %d\\n',1000)(41, 34, 41) */
		reached[6][34] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("set_timer Sensor trigger %d\n", 1000);
		/* merge: transition_id = -(1)(41, 36, 41) */
		reached[6][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((P6 *)_this)->_1210_7_6_9_transition_id;
		((P6 *)_this)->_1210_7_6_9_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Sensor_trigger:transition_id", ((P6 *)_this)->_1210_7_6_9_transition_id);
#endif
		;
		/* merge: global_state.sensor.state = 0(41, 37, 41) */
		reached[6][37] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = now.global_state.sensor.state;
		now.global_state.sensor.state = 0;
#ifdef VAR_RANGES
		logval("global_state.sensor.state", now.global_state.sensor.state);
#endif
		;
		/* merge: goto continuous_signals(0, 38, 41) */
		reached[6][38] = 1;
		;
		_m = 3; goto P999; /* 5 */
	case 86: // STATE 52 - system.pml:238 - [(empty(Sensor_trigger_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][52] = 1;
		if (!((q_len(now.Sensor_trigger_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 87: // STATE 53 - system.pml:239 - [(1)] (56:0:0 - 1)
		IfNotBlocked
		reached[6][53] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 55, 56) */
		reached[6][55] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 88: // STATE 56 - system.pml:241 - [Sensor_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[6][56] = 1;
		if (q_full(now.Sensor_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Sensor_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Sensor_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 89: // STATE 61 - system.pml:244 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[6][61] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Sensor_tc */
	case 90: // STATE 1 - system.pml:207 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[5][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 91: // STATE 2 - system.pml:210 - [(nempty(Sensor_tc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][2] = 1;
		if (!((q_len(now.Sensor_tc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 92: // STATE 3 - system.pml:211 - [Sensor_lock?_] (0:0:1 - 1)
		reached[5][3] = 1;
		if (q_len(now.Sensor_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Sensor_lock, XX-1, 0, 0);
		;
		qrecv(now.Sensor_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Sensor_lock);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 93: // STATE 4 - system.pml:214 - [(nempty(Sensor_tc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][4] = 1;
		if (!((q_len(now.Sensor_tc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 94: // STATE 5 - system.pml:215 - [Sensor_tc_channel?_] (0:0:1 - 1)
		reached[5][5] = 1;
		if (q_len(now.Sensor_tc_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Sensor_tc_channel, XX-1, 0, 0);
		;
		qrecv(now.Sensor_tc_channel, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Sensor_tc_channel);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 95: // STATE 12 - system.pml:218 - [(empty(Sensor_tc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][12] = 1;
		if (!((q_len(now.Sensor_tc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 96: // STATE 13 - system.pml:219 - [(1)] (16:0:0 - 1)
		IfNotBlocked
		reached[5][13] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 15, 16) */
		reached[5][15] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 97: // STATE 16 - system.pml:221 - [Sensor_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[5][16] = 1;
		if (q_full(now.Sensor_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Sensor_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Sensor_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 98: // STATE 21 - system.pml:224 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[5][21] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Host_tm */
	case 99: // STATE 1 - system.pml:187 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[4][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 100: // STATE 2 - system.pml:190 - [(nempty(Host_tm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][2] = 1;
		if (!((q_len(now.Host_tm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 101: // STATE 3 - system.pml:191 - [Host_lock?_] (0:0:1 - 1)
		reached[4][3] = 1;
		if (q_len(now.Host_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Host_lock, XX-1, 0, 0);
		;
		qrecv(now.Host_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Host_lock);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 102: // STATE 4 - system.pml:194 - [(nempty(Host_tm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][4] = 1;
		if (!((q_len(now.Host_tm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 103: // STATE 5 - system.pml:195 - [Host_tm_channel?_] (0:0:1 - 1)
		reached[4][5] = 1;
		if (q_len(now.Host_tm_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Host_tm_channel, XX-1, 0, 0);
		;
		qrecv(now.Host_tm_channel, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Host_tm_channel);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 104: // STATE 12 - system.pml:198 - [(empty(Host_tm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][12] = 1;
		if (!((q_len(now.Host_tm_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 105: // STATE 13 - system.pml:199 - [(1)] (16:0:0 - 1)
		IfNotBlocked
		reached[4][13] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 15, 16) */
		reached[4][15] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 106: // STATE 16 - system.pml:201 - [Host_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[4][16] = 1;
		if (q_full(now.Host_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Host_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Host_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 107: // STATE 21 - system.pml:204 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[4][21] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Controlsystem_reading */
	case 108: // STATE 1 - system.pml:166 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[3][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 109: // STATE 2 - system.pml:169 - [(nempty(Controlsystem_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][2] = 1;
		if (!((q_len(now.Controlsystem_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 110: // STATE 3 - system.pml:170 - [Controlsystem_lock?_] (0:0:1 - 1)
		reached[3][3] = 1;
		if (q_len(now.Controlsystem_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Controlsystem_lock, XX-1, 0, 0);
		;
		qrecv(now.Controlsystem_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Controlsystem_lock);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 111: // STATE 4 - system.pml:173 - [(nempty(Controlsystem_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][4] = 1;
		if (!((q_len(now.Controlsystem_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 112: // STATE 5 - system.pml:174 - [Controlsystem_reading_channel?Controlsystem_reading_signal_parameter] (61:0:2 - 1)
		reached[3][5] = 1;
		if (q_len(now.Controlsystem_reading_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = now.Controlsystem_reading_signal_parameter;
		;
		now.Controlsystem_reading_signal_parameter = qrecv(now.Controlsystem_reading_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Controlsystem_reading_signal_parameter", now.Controlsystem_reading_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Controlsystem_reading_channel);
			sprintf(simtmp, "%f,", now.Controlsystem_reading_signal_parameter); strcat(simvals, simtmp);
		}
#endif
		;
		/* merge: Controlsystem_reading_channel_used = 1(0, 6, 61) */
		reached[3][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = Controlsystem_reading_channel_used;
		Controlsystem_reading_channel_used = 1;
#ifdef VAR_RANGES
		logval("Controlsystem_reading_channel_used", ((int)Controlsystem_reading_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 113: // STATE 7 - controlsystem.pml:54 - [((global_state.controlsystem.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][7] = 1;
		if (!((now.global_state.controlsystem.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 114: // STATE 8 - dataview.pml:8496 - [global_state.controlsystem.inputvalue = Controlsystem_reading_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[3][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.inputvalue;
		now.global_state.controlsystem.inputvalue = now.Controlsystem_reading_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.inputvalue", now.global_state.controlsystem.inputvalue);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 115: // STATE 9 - dataview.pml:8501 - [assert(((global_state.controlsystem.inputvalue>=-(1000))&&(global_state.controlsystem.inputvalue<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.inputvalue>= -(1000))&&(now.global_state.controlsystem.inputvalue<=1000)), "((global_state.controlsystem.inputvalue>= -(1000))&&(global_state.controlsystem.inputvalue<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 116: // STATE 12 - controlsystem.pml:4 - [transition_id = 0] (0:53:2 - 1)
		IfNotBlocked
		reached[3][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->_1207_4_3_8_transition_id;
		((P3 *)_this)->_1207_4_3_8_transition_id = 0;
#ifdef VAR_RANGES
		logval("Controlsystem_reading:transition_id", ((P3 *)_this)->_1207_4_3_8_transition_id);
#endif
		;
		/* merge: transition_id = 2(53, 13, 53) */
		reached[3][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->_1207_4_3_8_transition_id;
		((P3 *)_this)->_1207_4_3_8_transition_id = 2;
#ifdef VAR_RANGES
		logval("Controlsystem_reading:transition_id", ((P3 *)_this)->_1207_4_3_8_transition_id);
#endif
		;
		/* merge: .(goto)(0, 54, 53) */
		reached[3][54] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 117: // STATE 14 - controlsystem.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][14] = 1;
		if (!((((P3 *)_this)->_1207_4_3_8_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1207_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_1207_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_1207_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 118: // STATE 16 - controlsystem.pml:9 - [((transition_id==0))] (52:0:3 - 1)
		IfNotBlocked
		reached[3][16] = 1;
		if (!((((P3 *)_this)->_1207_4_3_8_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1207_4_3_8_transition_id */  (trpt+1)->bup.ovals = grab_values(3);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->_1207_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_1207_4_3_8_transition_id = 0;
		/* merge: transition_id = -(1)(52, 17, 52) */
		reached[3][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->_1207_4_3_8_transition_id;
		((P3 *)_this)->_1207_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Controlsystem_reading:transition_id", ((P3 *)_this)->_1207_4_3_8_transition_id);
#endif
		;
		/* merge: global_state.controlsystem.state = 1(52, 18, 52) */
		reached[3][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.controlsystem.state;
		now.global_state.controlsystem.state = 1;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.state", now.global_state.controlsystem.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 52) */
		reached[3][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 119: // STATE 20 - controlsystem.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][20] = 1;
		if (!((((P3 *)_this)->_1207_4_3_8_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1207_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_1207_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_1207_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 120: // STATE 21 - system.pml:74 - [(1)] (52:0:2 - 1)
		IfNotBlocked
		reached[3][21] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(52, 23, 52) */
		reached[3][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->_1207_4_3_8_transition_id;
		((P3 *)_this)->_1207_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Controlsystem_reading:transition_id", ((P3 *)_this)->_1207_4_3_8_transition_id);
#endif
		;
		/* merge: global_state.controlsystem.state = 0(52, 24, 52) */
		reached[3][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.controlsystem.state;
		now.global_state.controlsystem.state = 0;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.state", now.global_state.controlsystem.state);
#endif
		;
		/* merge: goto continuous_signals(0, 25, 52) */
		reached[3][25] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 121: // STATE 26 - controlsystem.pml:18 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][26] = 1;
		if (!((((P3 *)_this)->_1207_4_3_8_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1207_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_1207_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_1207_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 122: // STATE 29 - dataview.pml:8496 - [global_state.controlsystem.i = (global_state.controlsystem.i+global_state.controlsystem.inputvalue)] (0:0:1 - 1)
		IfNotBlocked
		reached[3][29] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.i;
		now.global_state.controlsystem.i = (now.global_state.controlsystem.i+now.global_state.controlsystem.inputvalue);
#ifdef VAR_RANGES
		logval("global_state.controlsystem.i", now.global_state.controlsystem.i);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 123: // STATE 30 - dataview.pml:8501 - [assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][30] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.i>= -(1000))&&(now.global_state.controlsystem.i<=1000)), "((global_state.controlsystem.i>= -(1000))&&(global_state.controlsystem.i<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 124: // STATE 33 - dataview.pml:8496 - [global_state.controlsystem.d = (global_state.controlsystem.inputvalue-global_state.controlsystem.previous)] (0:0:1 - 1)
		IfNotBlocked
		reached[3][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.d;
		now.global_state.controlsystem.d = (now.global_state.controlsystem.inputvalue-now.global_state.controlsystem.previous);
#ifdef VAR_RANGES
		logval("global_state.controlsystem.d", now.global_state.controlsystem.d);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 125: // STATE 34 - dataview.pml:8501 - [assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][34] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.d>= -(1000))&&(now.global_state.controlsystem.d<=1000)), "((global_state.controlsystem.d>= -(1000))&&(global_state.controlsystem.d<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 126: // STATE 37 - dataview.pml:8496 - [global_state.controlsystem.previous = global_state.controlsystem.inputvalue] (0:0:1 - 1)
		IfNotBlocked
		reached[3][37] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.previous;
		now.global_state.controlsystem.previous = now.global_state.controlsystem.inputvalue;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.previous", now.global_state.controlsystem.previous);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 127: // STATE 38 - dataview.pml:8501 - [assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][38] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.previous>= -(1000))&&(now.global_state.controlsystem.previous<=1000)), "((global_state.controlsystem.previous>= -(1000))&&(global_state.controlsystem.previous<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 128: // STATE 42 - dataview.pml:8496 - [global_state.controlsystem.outputvalue = (((global_state.controlsystem.cfg.kp*global_state.controlsystem.inputvalue)+(global_state.controlsystem.cfg.ki*global_state.controlsystem.i))+(global_state.controlsystem.cfg.kd*global_state.controlsystem.d))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][42] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.outputvalue;
		now.global_state.controlsystem.outputvalue = (((now.global_state.controlsystem.cfg.kp*now.global_state.controlsystem.inputvalue)+(now.global_state.controlsystem.cfg.ki*now.global_state.controlsystem.i))+(now.global_state.controlsystem.cfg.kd*now.global_state.controlsystem.d));
#ifdef VAR_RANGES
		logval("global_state.controlsystem.outputvalue", now.global_state.controlsystem.outputvalue);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 129: // STATE 43 - dataview.pml:8501 - [assert(((global_state.controlsystem.outputvalue>=-(1000))&&(global_state.controlsystem.outputvalue<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][43] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.outputvalue>= -(1000))&&(now.global_state.controlsystem.outputvalue<=1000)), "((global_state.controlsystem.outputvalue>= -(1000))&&(global_state.controlsystem.outputvalue<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 130: // STATE 46 - system.pml:46 - [Actuator_controlSignal_channel!global_state.controlsystem.outputvalue] (52:0:1 - 1)
		IfNotBlocked
		reached[3][46] = 1;
		if (q_full(now.Actuator_controlSignal_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_controlSignal_channel);
		sprintf(simtmp, "%f,", now.global_state.controlsystem.outputvalue); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_controlSignal_channel, 0, now.global_state.controlsystem.outputvalue, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 48, 52) */
		reached[3][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1207_4_3_8_transition_id;
		((P3 *)_this)->_1207_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Controlsystem_reading:transition_id", ((P3 *)_this)->_1207_4_3_8_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 49, 52) */
		reached[3][49] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 131: // STATE 63 - system.pml:178 - [(empty(Controlsystem_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][63] = 1;
		if (!((q_len(now.Controlsystem_reading_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 132: // STATE 64 - system.pml:179 - [(1)] (67:0:0 - 1)
		IfNotBlocked
		reached[3][64] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 66, 67) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 133: // STATE 67 - system.pml:181 - [Controlsystem_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[3][67] = 1;
		if (q_full(now.Controlsystem_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Controlsystem_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Controlsystem_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 134: // STATE 72 - system.pml:184 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[3][72] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Controlsystem_configuration */
	case 135: // STATE 1 - system.pml:145 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[2][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 136: // STATE 2 - system.pml:148 - [(nempty(Controlsystem_configuration_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][2] = 1;
		if (!((q_len(now.Controlsystem_configuration_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 137: // STATE 3 - system.pml:149 - [Controlsystem_lock?_] (0:0:1 - 1)
		reached[2][3] = 1;
		if (q_len(now.Controlsystem_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Controlsystem_lock, XX-1, 0, 0);
		;
		qrecv(now.Controlsystem_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Controlsystem_lock);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 138: // STATE 4 - system.pml:152 - [(nempty(Controlsystem_configuration_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][4] = 1;
		if (!((q_len(now.Controlsystem_configuration_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 139: // STATE 5 - system.pml:153 - [Controlsystem_configuration_channel?Controlsystem_configuration_signal_parameter.kp,Controlsystem_configuration_signal_parameter.ki,Controlsystem_configuration_signal_parameter.kd] (70:0:4 - 1)
		reached[2][5] = 1;
		if (q_len(now.Controlsystem_configuration_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_values(4);
		(trpt+1)->bup.ovals[0] = now.Controlsystem_configuration_signal_parameter.kp;
		(trpt+1)->bup.ovals[1] = now.Controlsystem_configuration_signal_parameter.ki;
		(trpt+1)->bup.ovals[2] = now.Controlsystem_configuration_signal_parameter.kd;
		;
		now.Controlsystem_configuration_signal_parameter.kp = qrecv(now.Controlsystem_configuration_channel, XX-1, 0, 0);
#ifdef VAR_RANGES
		logval("Controlsystem_configuration_signal_parameter.kp", now.Controlsystem_configuration_signal_parameter.kp);
#endif
		;
		now.Controlsystem_configuration_signal_parameter.ki = qrecv(now.Controlsystem_configuration_channel, XX-1, 1, 0);
#ifdef VAR_RANGES
		logval("Controlsystem_configuration_signal_parameter.ki", now.Controlsystem_configuration_signal_parameter.ki);
#endif
		;
		now.Controlsystem_configuration_signal_parameter.kd = qrecv(now.Controlsystem_configuration_channel, XX-1, 2, 1);
#ifdef VAR_RANGES
		logval("Controlsystem_configuration_signal_parameter.kd", now.Controlsystem_configuration_signal_parameter.kd);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Controlsystem_configuration_channel);
			sprintf(simtmp, "%f,", now.Controlsystem_configuration_signal_parameter.kp); strcat(simvals, simtmp);
			strcat(simvals, ",");
			sprintf(simtmp, "%f,", now.Controlsystem_configuration_signal_parameter.ki); strcat(simvals, simtmp);
			strcat(simvals, ",");
			sprintf(simtmp, "%f,", now.Controlsystem_configuration_signal_parameter.kd); strcat(simvals, simtmp);
		}
#endif
		;
		/* merge: Controlsystem_configuration_channel_used = 1(0, 6, 70) */
		reached[2][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = Controlsystem_configuration_channel_used;
		Controlsystem_configuration_channel_used = 1;
#ifdef VAR_RANGES
		logval("Controlsystem_configuration_channel_used", ((int)Controlsystem_configuration_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 140: // STATE 7 - controlsystem.pml:44 - [((global_state.controlsystem.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][7] = 1;
		if (!((now.global_state.controlsystem.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 141: // STATE 8 - dataview.pml:8505 - [global_state.controlsystem.cfg.kp = Controlsystem_configuration_signal_parameter.kp] (0:0:1 - 1)
		IfNotBlocked
		reached[2][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.cfg.kp;
		now.global_state.controlsystem.cfg.kp = now.Controlsystem_configuration_signal_parameter.kp;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.cfg.kp", now.global_state.controlsystem.cfg.kp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 142: // STATE 9 - dataview.pml:8510 - [assert(((global_state.controlsystem.cfg.kp>=-(1000))&&(global_state.controlsystem.cfg.kp<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.cfg.kp>= -(1000))&&(now.global_state.controlsystem.cfg.kp<=1000)), "((global_state.controlsystem.cfg.kp>= -(1000))&&(global_state.controlsystem.cfg.kp<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 143: // STATE 12 - dataview.pml:8514 - [global_state.controlsystem.cfg.ki = Controlsystem_configuration_signal_parameter.ki] (0:0:1 - 1)
		IfNotBlocked
		reached[2][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.cfg.ki;
		now.global_state.controlsystem.cfg.ki = now.Controlsystem_configuration_signal_parameter.ki;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.cfg.ki", now.global_state.controlsystem.cfg.ki);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 144: // STATE 13 - dataview.pml:8519 - [assert(((global_state.controlsystem.cfg.ki>=-(1000))&&(global_state.controlsystem.cfg.ki<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][13] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.cfg.ki>= -(1000))&&(now.global_state.controlsystem.cfg.ki<=1000)), "((global_state.controlsystem.cfg.ki>= -(1000))&&(global_state.controlsystem.cfg.ki<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 145: // STATE 16 - dataview.pml:8523 - [global_state.controlsystem.cfg.kd = Controlsystem_configuration_signal_parameter.kd] (0:0:1 - 1)
		IfNotBlocked
		reached[2][16] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.cfg.kd;
		now.global_state.controlsystem.cfg.kd = now.Controlsystem_configuration_signal_parameter.kd;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.cfg.kd", now.global_state.controlsystem.cfg.kd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 146: // STATE 17 - dataview.pml:8528 - [assert(((global_state.controlsystem.cfg.kd>=-(1000))&&(global_state.controlsystem.cfg.kd<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][17] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.cfg.kd>= -(1000))&&(now.global_state.controlsystem.cfg.kd<=1000)), "((global_state.controlsystem.cfg.kd>= -(1000))&&(global_state.controlsystem.cfg.kd<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 147: // STATE 21 - controlsystem.pml:4 - [transition_id = 0] (0:62:2 - 1)
		IfNotBlocked
		reached[2][21] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1206_3_2_6_transition_id;
		((P2 *)_this)->_1206_3_2_6_transition_id = 0;
#ifdef VAR_RANGES
		logval("Controlsystem_configuration:transition_id", ((P2 *)_this)->_1206_3_2_6_transition_id);
#endif
		;
		/* merge: transition_id = 1(62, 22, 62) */
		reached[2][22] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1206_3_2_6_transition_id;
		((P2 *)_this)->_1206_3_2_6_transition_id = 1;
#ifdef VAR_RANGES
		logval("Controlsystem_configuration:transition_id", ((P2 *)_this)->_1206_3_2_6_transition_id);
#endif
		;
		/* merge: .(goto)(0, 63, 62) */
		reached[2][63] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 148: // STATE 23 - controlsystem.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][23] = 1;
		if (!((((P2 *)_this)->_1206_3_2_6_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1206_3_2_6_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1206_3_2_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1206_3_2_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 149: // STATE 25 - controlsystem.pml:9 - [((transition_id==0))] (61:0:3 - 1)
		IfNotBlocked
		reached[2][25] = 1;
		if (!((((P2 *)_this)->_1206_3_2_6_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1206_3_2_6_transition_id */  (trpt+1)->bup.ovals = grab_values(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1206_3_2_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1206_3_2_6_transition_id = 0;
		/* merge: transition_id = -(1)(61, 26, 61) */
		reached[2][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1206_3_2_6_transition_id;
		((P2 *)_this)->_1206_3_2_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Controlsystem_configuration:transition_id", ((P2 *)_this)->_1206_3_2_6_transition_id);
#endif
		;
		/* merge: global_state.controlsystem.state = 1(61, 27, 61) */
		reached[2][27] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.controlsystem.state;
		now.global_state.controlsystem.state = 1;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.state", now.global_state.controlsystem.state);
#endif
		;
		/* merge: goto continuous_signals(0, 28, 61) */
		reached[2][28] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 150: // STATE 29 - controlsystem.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][29] = 1;
		if (!((((P2 *)_this)->_1206_3_2_6_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1206_3_2_6_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1206_3_2_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1206_3_2_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 151: // STATE 30 - system.pml:74 - [(1)] (61:0:2 - 1)
		IfNotBlocked
		reached[2][30] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(61, 32, 61) */
		reached[2][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1206_3_2_6_transition_id;
		((P2 *)_this)->_1206_3_2_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Controlsystem_configuration:transition_id", ((P2 *)_this)->_1206_3_2_6_transition_id);
#endif
		;
		/* merge: global_state.controlsystem.state = 0(61, 33, 61) */
		reached[2][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.controlsystem.state;
		now.global_state.controlsystem.state = 0;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.state", now.global_state.controlsystem.state);
#endif
		;
		/* merge: goto continuous_signals(0, 34, 61) */
		reached[2][34] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 152: // STATE 35 - controlsystem.pml:18 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][35] = 1;
		if (!((((P2 *)_this)->_1206_3_2_6_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1206_3_2_6_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1206_3_2_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1206_3_2_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 153: // STATE 38 - dataview.pml:8496 - [global_state.controlsystem.i = (global_state.controlsystem.i+global_state.controlsystem.inputvalue)] (0:0:1 - 1)
		IfNotBlocked
		reached[2][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.i;
		now.global_state.controlsystem.i = (now.global_state.controlsystem.i+now.global_state.controlsystem.inputvalue);
#ifdef VAR_RANGES
		logval("global_state.controlsystem.i", now.global_state.controlsystem.i);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 154: // STATE 39 - dataview.pml:8501 - [assert(((global_state.controlsystem.i>=-(1000))&&(global_state.controlsystem.i<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][39] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.i>= -(1000))&&(now.global_state.controlsystem.i<=1000)), "((global_state.controlsystem.i>= -(1000))&&(global_state.controlsystem.i<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 155: // STATE 42 - dataview.pml:8496 - [global_state.controlsystem.d = (global_state.controlsystem.inputvalue-global_state.controlsystem.previous)] (0:0:1 - 1)
		IfNotBlocked
		reached[2][42] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.d;
		now.global_state.controlsystem.d = (now.global_state.controlsystem.inputvalue-now.global_state.controlsystem.previous);
#ifdef VAR_RANGES
		logval("global_state.controlsystem.d", now.global_state.controlsystem.d);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 156: // STATE 43 - dataview.pml:8501 - [assert(((global_state.controlsystem.d>=-(1000))&&(global_state.controlsystem.d<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][43] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.d>= -(1000))&&(now.global_state.controlsystem.d<=1000)), "((global_state.controlsystem.d>= -(1000))&&(global_state.controlsystem.d<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 157: // STATE 46 - dataview.pml:8496 - [global_state.controlsystem.previous = global_state.controlsystem.inputvalue] (0:0:1 - 1)
		IfNotBlocked
		reached[2][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.previous;
		now.global_state.controlsystem.previous = now.global_state.controlsystem.inputvalue;
#ifdef VAR_RANGES
		logval("global_state.controlsystem.previous", now.global_state.controlsystem.previous);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 158: // STATE 47 - dataview.pml:8501 - [assert(((global_state.controlsystem.previous>=-(1000))&&(global_state.controlsystem.previous<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][47] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.previous>= -(1000))&&(now.global_state.controlsystem.previous<=1000)), "((global_state.controlsystem.previous>= -(1000))&&(global_state.controlsystem.previous<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 159: // STATE 51 - dataview.pml:8496 - [global_state.controlsystem.outputvalue = (((global_state.controlsystem.cfg.kp*global_state.controlsystem.inputvalue)+(global_state.controlsystem.cfg.ki*global_state.controlsystem.i))+(global_state.controlsystem.cfg.kd*global_state.controlsystem.d))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][51] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.controlsystem.outputvalue;
		now.global_state.controlsystem.outputvalue = (((now.global_state.controlsystem.cfg.kp*now.global_state.controlsystem.inputvalue)+(now.global_state.controlsystem.cfg.ki*now.global_state.controlsystem.i))+(now.global_state.controlsystem.cfg.kd*now.global_state.controlsystem.d));
#ifdef VAR_RANGES
		logval("global_state.controlsystem.outputvalue", now.global_state.controlsystem.outputvalue);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 160: // STATE 52 - dataview.pml:8501 - [assert(((global_state.controlsystem.outputvalue>=-(1000))&&(global_state.controlsystem.outputvalue<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][52] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.controlsystem.outputvalue>= -(1000))&&(now.global_state.controlsystem.outputvalue<=1000)), "((global_state.controlsystem.outputvalue>= -(1000))&&(global_state.controlsystem.outputvalue<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 161: // STATE 55 - system.pml:46 - [Actuator_controlSignal_channel!global_state.controlsystem.outputvalue] (61:0:1 - 1)
		IfNotBlocked
		reached[2][55] = 1;
		if (q_full(now.Actuator_controlSignal_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_controlSignal_channel);
		sprintf(simtmp, "%f,", now.global_state.controlsystem.outputvalue); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_controlSignal_channel, 0, now.global_state.controlsystem.outputvalue, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 57, 61) */
		reached[2][57] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_1206_3_2_6_transition_id;
		((P2 *)_this)->_1206_3_2_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Controlsystem_configuration:transition_id", ((P2 *)_this)->_1206_3_2_6_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 58, 61) */
		reached[2][58] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 162: // STATE 72 - system.pml:157 - [(empty(Controlsystem_configuration_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][72] = 1;
		if (!((q_len(now.Controlsystem_configuration_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 163: // STATE 73 - system.pml:158 - [(1)] (76:0:0 - 1)
		IfNotBlocked
		reached[2][73] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 75, 76) */
		reached[2][75] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 164: // STATE 76 - system.pml:160 - [Controlsystem_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[2][76] = 1;
		if (q_full(now.Controlsystem_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Controlsystem_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Controlsystem_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 165: // STATE 81 - system.pml:163 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[2][81] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Actuator_controlSignal */
	case 166: // STATE 1 - system.pml:124 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[1][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 167: // STATE 2 - system.pml:127 - [(nempty(Actuator_controlSignal_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][2] = 1;
		if (!((q_len(now.Actuator_controlSignal_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 168: // STATE 3 - system.pml:128 - [Actuator_lock?_] (0:0:1 - 1)
		reached[1][3] = 1;
		if (q_len(now.Actuator_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Actuator_lock, XX-1, 0, 0);
		;
		qrecv(now.Actuator_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator_lock);
			sprintf(simtmp, "%d,", ((int)_)); strcat(simvals, simtmp);
		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 169: // STATE 4 - system.pml:131 - [(nempty(Actuator_controlSignal_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][4] = 1;
		if (!((q_len(now.Actuator_controlSignal_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 170: // STATE 5 - system.pml:132 - [Actuator_controlSignal_channel?Actuator_controlsignal_signal_parameter] (85:0:2 - 1)
		reached[1][5] = 1;
		if (q_len(now.Actuator_controlSignal_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = now.Actuator_controlsignal_signal_parameter;
		;
		now.Actuator_controlsignal_signal_parameter = qrecv(now.Actuator_controlSignal_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Actuator_controlsignal_signal_parameter", now.Actuator_controlsignal_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator_controlSignal_channel);
			sprintf(simtmp, "%f,", now.Actuator_controlsignal_signal_parameter); strcat(simvals, simtmp);
		}
#endif
		;
		/* merge: Actuator_controlsignal_channel_used = 1(0, 6, 85) */
		reached[1][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = Actuator_controlsignal_channel_used;
		Actuator_controlsignal_channel_used = 1;
#ifdef VAR_RANGES
		logval("Actuator_controlsignal_channel_used", ((int)Actuator_controlsignal_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 171: // STATE 7 - actuator.pml:39 - [((global_state.actuator.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][7] = 1;
		if (!((now.global_state.actuator.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 172: // STATE 8 - dataview.pml:8496 - [global_state.actuator.inputcontrol = Actuator_controlsignal_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[1][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.inputcontrol;
		now.global_state.actuator.inputcontrol = now.Actuator_controlsignal_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator.inputcontrol", now.global_state.actuator.inputcontrol);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 173: // STATE 9 - dataview.pml:8501 - [assert(((global_state.actuator.inputcontrol>=-(1000))&&(global_state.actuator.inputcontrol<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.inputcontrol>= -(1000))&&(now.global_state.actuator.inputcontrol<=1000)), "((global_state.actuator.inputcontrol>= -(1000))&&(global_state.actuator.inputcontrol<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 174: // STATE 12 - actuator.pml:4 - [transition_id = 0] (0:40:2 - 1)
		IfNotBlocked
		reached[1][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_2_transition_id;
		((P1 *)_this)->_1205_2_1_2_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_2_transition_id);
#endif
		;
		/* merge: transition_id = 1(40, 13, 40) */
		reached[1][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1205_2_1_2_transition_id;
		((P1 *)_this)->_1205_2_1_2_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_2_transition_id);
#endif
		;
		/* merge: .(goto)(0, 41, 40) */
		reached[1][41] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 175: // STATE 14 - actuator.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][14] = 1;
		if (!((((P1 *)_this)->_1205_2_1_2_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1205_2_1_2_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1205_2_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1205_2_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 176: // STATE 16 - actuator.pml:9 - [((transition_id==0))] (39:0:3 - 1)
		IfNotBlocked
		reached[1][16] = 1;
		if (!((((P1 *)_this)->_1205_2_1_2_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1205_2_1_2_transition_id */  (trpt+1)->bup.ovals = grab_values(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1205_2_1_2_transition_id = 0;
		/* merge: transition_id = -(1)(39, 17, 39) */
		reached[1][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1205_2_1_2_transition_id;
		((P1 *)_this)->_1205_2_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(39, 18, 39) */
		reached[1][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 39) */
		reached[1][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 177: // STATE 20 - actuator.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][20] = 1;
		if (!((((P1 *)_this)->_1205_2_1_2_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1205_2_1_2_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1205_2_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1205_2_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 178: // STATE 23 - dataview.pml:8496 - [global_state.actuator.setting = (1.500000*global_state.actuator.inputcontrol)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.setting;
		now.global_state.actuator.setting = (1.500000*now.global_state.actuator.inputcontrol);
#ifdef VAR_RANGES
		logval("global_state.actuator.setting", now.global_state.actuator.setting);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 179: // STATE 24 - dataview.pml:8501 - [assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][24] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.setting>= -(1000))&&(now.global_state.actuator.setting<=1000)), "((global_state.actuator.setting>= -(1000))&&(global_state.actuator.setting<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 180: // STATE 27 - actuator.pml:17 - [(((global_state.actuator.setting>10.000000)==1))] (39:0:2 - 1)
		IfNotBlocked
		reached[1][27] = 1;
		if (!(((now.global_state.actuator.setting>10.000000)==1)))
			continue;
		/* merge: transition_id = -(1)(39, 28, 39) */
		reached[1][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_2_transition_id;
		((P1 *)_this)->_1205_2_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 1(39, 29, 39) */
		reached[1][29] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 30, 39) */
		reached[1][30] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 181: // STATE 31 - actuator.pml:21 - [(((global_state.actuator.setting>10.000000)==0))] (39:0:2 - 1)
		IfNotBlocked
		reached[1][31] = 1;
		if (!(((now.global_state.actuator.setting>10.000000)==0)))
			continue;
		/* merge: transition_id = -(1)(39, 32, 39) */
		reached[1][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_2_transition_id;
		((P1 *)_this)->_1205_2_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(39, 33, 39) */
		reached[1][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 34, 39) */
		reached[1][34] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 182: // STATE 44 - actuator.pml:42 - [((global_state.actuator.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][44] = 1;
		if (!((now.global_state.actuator.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 183: // STATE 45 - dataview.pml:8496 - [global_state.actuator.inputcontrol = Actuator_controlsignal_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[1][45] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.inputcontrol;
		now.global_state.actuator.inputcontrol = now.Actuator_controlsignal_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator.inputcontrol", now.global_state.actuator.inputcontrol);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 184: // STATE 46 - dataview.pml:8501 - [assert(((global_state.actuator.inputcontrol>=-(1000))&&(global_state.actuator.inputcontrol<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][46] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.inputcontrol>= -(1000))&&(now.global_state.actuator.inputcontrol<=1000)), "((global_state.actuator.inputcontrol>= -(1000))&&(global_state.actuator.inputcontrol<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 185: // STATE 49 - actuator.pml:4 - [transition_id = 0] (0:77:2 - 1)
		IfNotBlocked
		reached[1][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_4_transition_id;
		((P1 *)_this)->_1205_2_1_4_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_4_transition_id);
#endif
		;
		/* merge: transition_id = 1(77, 50, 77) */
		reached[1][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1205_2_1_4_transition_id;
		((P1 *)_this)->_1205_2_1_4_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_4_transition_id);
#endif
		;
		/* merge: .(goto)(0, 78, 77) */
		reached[1][78] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 186: // STATE 51 - actuator.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][51] = 1;
		if (!((((P1 *)_this)->_1205_2_1_4_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1205_2_1_4_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1205_2_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1205_2_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 187: // STATE 53 - actuator.pml:9 - [((transition_id==0))] (76:0:3 - 1)
		IfNotBlocked
		reached[1][53] = 1;
		if (!((((P1 *)_this)->_1205_2_1_4_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1205_2_1_4_transition_id */  (trpt+1)->bup.ovals = grab_values(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1205_2_1_4_transition_id = 0;
		/* merge: transition_id = -(1)(76, 54, 76) */
		reached[1][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1205_2_1_4_transition_id;
		((P1 *)_this)->_1205_2_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(76, 55, 76) */
		reached[1][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 76) */
		reached[1][56] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 188: // STATE 57 - actuator.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][57] = 1;
		if (!((((P1 *)_this)->_1205_2_1_4_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1205_2_1_4_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1205_2_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1205_2_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 189: // STATE 60 - dataview.pml:8496 - [global_state.actuator.setting = (1.500000*global_state.actuator.inputcontrol)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][60] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.setting;
		now.global_state.actuator.setting = (1.500000*now.global_state.actuator.inputcontrol);
#ifdef VAR_RANGES
		logval("global_state.actuator.setting", now.global_state.actuator.setting);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 190: // STATE 61 - dataview.pml:8501 - [assert(((global_state.actuator.setting>=-(1000))&&(global_state.actuator.setting<=1000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][61] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.setting>= -(1000))&&(now.global_state.actuator.setting<=1000)), "((global_state.actuator.setting>= -(1000))&&(global_state.actuator.setting<=1000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 191: // STATE 64 - actuator.pml:17 - [(((global_state.actuator.setting>10.000000)==1))] (76:0:2 - 1)
		IfNotBlocked
		reached[1][64] = 1;
		if (!(((now.global_state.actuator.setting>10.000000)==1)))
			continue;
		/* merge: transition_id = -(1)(76, 65, 76) */
		reached[1][65] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_4_transition_id;
		((P1 *)_this)->_1205_2_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 1(76, 66, 76) */
		reached[1][66] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 67, 76) */
		reached[1][67] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 192: // STATE 68 - actuator.pml:21 - [(((global_state.actuator.setting>10.000000)==0))] (76:0:2 - 1)
		IfNotBlocked
		reached[1][68] = 1;
		if (!(((now.global_state.actuator.setting>10.000000)==0)))
			continue;
		/* merge: transition_id = -(1)(76, 69, 76) */
		reached[1][69] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_values(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1205_2_1_4_transition_id;
		((P1 *)_this)->_1205_2_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_controlSignal:transition_id", ((P1 *)_this)->_1205_2_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(76, 70, 76) */
		reached[1][70] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 71, 76) */
		reached[1][71] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 193: // STATE 87 - system.pml:136 - [(empty(Actuator_controlSignal_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][87] = 1;
		if (!((q_len(now.Actuator_controlSignal_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 194: // STATE 88 - system.pml:137 - [(1)] (91:0:0 - 1)
		IfNotBlocked
		reached[1][88] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 90, 91) */
		reached[1][90] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 195: // STATE 91 - system.pml:139 - [Actuator_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[1][91] = 1;
		if (q_full(now.Actuator_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_lock);
		sprintf(simtmp, "%d,", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 196: // STATE 96 - system.pml:142 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[1][96] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC timer_manager_proc */
	case 197: // STATE 1 - system.pml:108 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[0][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 198: // STATE 3 - system.pml:113 - [(global_state.timers.sensor.trigger.timer_enabled)] (0:0:0 - 1)
		IfNotBlocked
		reached[0][3] = 1;
		if (!(((int)now.global_state.timers.sensor.trigger.timer_enabled)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 199: // STATE 4 - system.pml:114 - [Sensor_trigger_channel!0] (11:0:1 - 1)
		IfNotBlocked
		reached[0][4] = 1;
		if (q_full(now.Sensor_trigger_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Sensor_trigger_channel);
		sprintf(simtmp, "%d,", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Sensor_trigger_channel, 0, 0, 0, 0, 1);
		/* merge: global_state.timers.sensor.trigger.timer_enabled = 0(11, 5, 11) */
		reached[0][5] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.timers.sensor.trigger.timer_enabled;
		now.global_state.timers.sensor.trigger.timer_enabled = 0;
#ifdef VAR_RANGES
		logval("global_state.timers.sensor.trigger.timer_enabled", ((int)now.global_state.timers.sensor.trigger.timer_enabled));
#endif
		;
		/* merge: .(goto)(11, 9, 11) */
		reached[0][9] = 1;
		;
		/* merge: .(goto)(0, 12, 11) */
		reached[0][12] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 200: // STATE 9 - system.pml:119 - [.(goto)] (0:11:0 - 2)
		IfNotBlocked
		reached[0][9] = 1;
		;
		/* merge: .(goto)(0, 12, 11) */
		reached[0][12] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 201: // STATE 7 - system.pml:117 - [(1)] (11:0:0 - 1)
		IfNotBlocked
		reached[0][7] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(11, 9, 11) */
		reached[0][9] = 1;
		;
		/* merge: .(goto)(0, 12, 11) */
		reached[0][12] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 202: // STATE 14 - system.pml:121 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[0][14] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */
	case  _T5:	/* np_ */
		if (!((!(trpt->o_pm&4) && !(trpt->tau&128))))
			continue;
		/* else fall through */
	case  _T2:	/* true */
		_m = 3; goto P999;
#undef rand
	}

