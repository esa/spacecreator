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
	case 3: // STATE 4 - dataview.pml:20161 - [D_STEP20161]
		IfNotBlocked

		reached[9][4] = 1;
		reached[9][t->st] = 1;
		reached[9][tt] = 1;

		if (TstOnly) return 1;


		reached[9][4] = 1;
		reached[9][t->st] = 1;
		reached[9][tt] = 1;

		sv_save();
		S_1749_0: /* 2 */
S_1748_0: /* 2 */
S_1747_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.nothing.selection = 1;
#ifdef VAR_RANGES
		logval("nothing.selection", now.nothing.selection);
#endif
		;
		goto S_1819_0;
S_1819_0: /* 1 */

#if defined(C_States) && (HAS_TRACK==1)
		c_update((uchar *) &(now.c_state[0]));
#endif
		_m = 3; goto P999;

	case 4: // STATE 6 - dataview.pml:8783 - [global_state.actuator1.sender = 6] (0:0:1 - 1)
		IfNotBlocked
		reached[9][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator1.sender;
		now.global_state.actuator1.sender = 6;
#ifdef VAR_RANGES
		logval("global_state.actuator1.sender", now.global_state.actuator1.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 5: // STATE 7 - dataview.pml:8788 - [assert((((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][7] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.actuator1.sender==0)||(now.global_state.actuator1.sender==1))||(now.global_state.actuator1.sender==2))||(now.global_state.actuator1.sender==3))||(now.global_state.actuator1.sender==4))||(now.global_state.actuator1.sender==5))||(now.global_state.actuator1.sender==6)), "(((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 6: // STATE 10 - actuator1.pml:4 - [transition_id = 0] (0:69:2 - 1)
		IfNotBlocked
		reached[9][10] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: transition_id = 0(69, 11, 69) */
		reached[9][11] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: .(goto)(0, 70, 69) */
		reached[9][70] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 7: // STATE 12 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][12] = 1;
		if (!((((P9 *)_this)->_1061_10_10_33_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_10_33_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_10_33_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 8: // STATE 14 - actuator1.pml:9 - [((transition_id==0))] (68:0:3 - 1)
		IfNotBlocked
		reached[9][14] = 1;
		if (!((((P9 *)_this)->_1061_10_10_33_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_10_33_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_10_33_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_10_33_transition_id = 0;
		/* merge: transition_id = -(1)(68, 15, 68) */
		reached[9][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(68, 16, 68) */
		reached[9][16] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 17, 68) */
		reached[9][17] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 9: // STATE 18 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][18] = 1;
		if (!((((P9 *)_this)->_1061_10_10_33_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_10_33_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_10_33_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 10: // STATE 21 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][21] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 11: // STATE 22 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (68:0:2 - 1)
		IfNotBlocked
		reached[9][22] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(68, 24, 68) */
		reached[9][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(68, 25, 68) */
		reached[9][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 26, 68) */
		reached[9][26] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 12: // STATE 27 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][27] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 13: // STATE 28 - system.pml:73 - [Modemanager_actuatorTm1_channel!1] (68:0:2 - 1)
		IfNotBlocked
		reached[9][28] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(68, 30, 68) */
		reached[9][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 1(68, 31, 68) */
		reached[9][31] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 32, 68) */
		reached[9][32] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 14: // STATE 34 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][34] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 15: // STATE 38 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][38] = 1;
		if (!((((P9 *)_this)->_1061_10_10_33_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_10_33_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_10_33_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 16: // STATE 41 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][41] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 17: // STATE 42 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (68:0:2 - 1)
		IfNotBlocked
		reached[9][42] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(68, 44, 68) */
		reached[9][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(68, 45, 68) */
		reached[9][45] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 46, 68) */
		reached[9][46] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 18: // STATE 48 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 19: // STATE 52 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][52] = 1;
		if (!((((P9 *)_this)->_1061_10_10_33_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_10_33_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_10_33_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 20: // STATE 55 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][55] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 21: // STATE 56 - system.pml:73 - [Modemanager_actuatorTm1_channel!0] (68:0:2 - 1)
		IfNotBlocked
		reached[9][56] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(68, 58, 68) */
		reached[9][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 0(68, 59, 68) */
		reached[9][59] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 60, 68) */
		reached[9][60] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 22: // STATE 62 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_10_33_transition_id;
		((P9 *)_this)->_1061_10_10_33_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_10_33_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 23: // STATE 74 - system.pml:272 - [Actuator1_lock!1] (0:0:0 - 3)
		IfNotBlocked
		reached[9][74] = 1;
		if (q_full(now.Actuator1_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 24: // STATE 75 - dataview.pml:8783 - [global_state.actuator2.sender = 6] (0:0:1 - 1)
		IfNotBlocked
		reached[9][75] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator2.sender;
		now.global_state.actuator2.sender = 6;
#ifdef VAR_RANGES
		logval("global_state.actuator2.sender", now.global_state.actuator2.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 25: // STATE 76 - dataview.pml:8788 - [assert((((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][76] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.actuator2.sender==0)||(now.global_state.actuator2.sender==1))||(now.global_state.actuator2.sender==2))||(now.global_state.actuator2.sender==3))||(now.global_state.actuator2.sender==4))||(now.global_state.actuator2.sender==5))||(now.global_state.actuator2.sender==6)), "(((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 26: // STATE 79 - actuator2.pml:4 - [transition_id = 0] (0:138:2 - 1)
		IfNotBlocked
		reached[9][79] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: transition_id = 0(138, 80, 138) */
		reached[9][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: .(goto)(0, 139, 138) */
		reached[9][139] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 27: // STATE 81 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][81] = 1;
		if (!((((P9 *)_this)->_1061_10_11_35_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_11_35_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_11_35_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 28: // STATE 83 - actuator2.pml:9 - [((transition_id==0))] (137:0:3 - 1)
		IfNotBlocked
		reached[9][83] = 1;
		if (!((((P9 *)_this)->_1061_10_11_35_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_11_35_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_11_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_11_35_transition_id = 0;
		/* merge: transition_id = -(1)(137, 84, 137) */
		reached[9][84] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(137, 85, 137) */
		reached[9][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 86, 137) */
		reached[9][86] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 29: // STATE 87 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][87] = 1;
		if (!((((P9 *)_this)->_1061_10_11_35_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_11_35_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_11_35_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 30: // STATE 90 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][90] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 31: // STATE 91 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (137:0:2 - 1)
		IfNotBlocked
		reached[9][91] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(137, 93, 137) */
		reached[9][93] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(137, 94, 137) */
		reached[9][94] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 95, 137) */
		reached[9][95] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 32: // STATE 96 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][96] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 33: // STATE 97 - system.pml:77 - [Modemanager_actuatorTm2_channel!1] (137:0:2 - 1)
		IfNotBlocked
		reached[9][97] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(137, 99, 137) */
		reached[9][99] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 1(137, 100, 137) */
		reached[9][100] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 101, 137) */
		reached[9][101] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 34: // STATE 103 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][103] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 35: // STATE 107 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][107] = 1;
		if (!((((P9 *)_this)->_1061_10_11_35_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_11_35_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_11_35_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 36: // STATE 110 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][110] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 37: // STATE 111 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (137:0:2 - 1)
		IfNotBlocked
		reached[9][111] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(137, 113, 137) */
		reached[9][113] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(137, 114, 137) */
		reached[9][114] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 115, 137) */
		reached[9][115] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 38: // STATE 117 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][117] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 39: // STATE 121 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][121] = 1;
		if (!((((P9 *)_this)->_1061_10_11_35_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_11_35_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_11_35_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 40: // STATE 124 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][124] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 41: // STATE 125 - system.pml:77 - [Modemanager_actuatorTm2_channel!0] (137:0:2 - 1)
		IfNotBlocked
		reached[9][125] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(137, 127, 137) */
		reached[9][127] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 0(137, 128, 137) */
		reached[9][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 129, 137) */
		reached[9][129] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 42: // STATE 131 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][131] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_11_35_transition_id;
		((P9 *)_this)->_1061_10_11_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_11_35_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 43: // STATE 143 - system.pml:274 - [Actuator2_lock!1] (0:0:0 - 3)
		IfNotBlocked
		reached[9][143] = 1;
		if (q_full(now.Actuator2_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 44: // STATE 144 - dataview.pml:8783 - [global_state.modemanager.sender = 6] (0:0:1 - 1)
		IfNotBlocked
		reached[9][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.sender;
		now.global_state.modemanager.sender = 6;
#ifdef VAR_RANGES
		logval("global_state.modemanager.sender", now.global_state.modemanager.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 45: // STATE 145 - dataview.pml:8788 - [assert((((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][145] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.modemanager.sender==0)||(now.global_state.modemanager.sender==1))||(now.global_state.modemanager.sender==2))||(now.global_state.modemanager.sender==3))||(now.global_state.modemanager.sender==4))||(now.global_state.modemanager.sender==5))||(now.global_state.modemanager.sender==6)), "(((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 46: // STATE 148 - modemanager.pml:4 - [transition_id = 0] (0:288:2 - 1)
		IfNotBlocked
		reached[9][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: transition_id = 0(288, 149, 288) */
		reached[9][149] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: .(goto)(0, 289, 288) */
		reached[9][289] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 47: // STATE 150 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][150] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 48: // STATE 152 - modemanager.pml:9 - [((transition_id==0))] (287:0:3 - 1)
		IfNotBlocked
		reached[9][152] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		/* merge: transition_id = -(1)(287, 153, 287) */
		reached[9][153] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(287, 154, 287) */
		reached[9][154] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 155, 287) */
		reached[9][155] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 49: // STATE 156 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][156] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 50: // STATE 159 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][159] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 51: // STATE 160 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[9][160] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 52: // STATE 161 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][161] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 53: // STATE 164 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[9][164] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 54: // STATE 165 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][165] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 55: // STATE 168 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[9][168] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 56: // STATE 170 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (287:0:2 - 1)
		IfNotBlocked
		reached[9][170] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(287, 172, 287) */
		reached[9][172] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(287, 173, 287) */
		reached[9][173] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 174, 287) */
		reached[9][174] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 57: // STATE 175 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][175] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 58: // STATE 176 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[9][176] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 59: // STATE 177 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][177] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 60: // STATE 180 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[9][180] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 61: // STATE 181 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][181] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 62: // STATE 184 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[9][184] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 63: // STATE 185 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][185] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 64: // STATE 189 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (287:0:1 - 1)
		IfNotBlocked
		reached[9][189] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 191, 287) */
		reached[9][191] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 192, 287) */
		reached[9][192] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 65: // STATE 194 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][194] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 66: // STATE 198 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][198] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 67: // STATE 201 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][201] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 68: // STATE 202 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[9][202] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 69: // STATE 204 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (287:0:2 - 1)
		IfNotBlocked
		reached[9][204] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(287, 206, 287) */
		reached[9][206] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(287, 207, 287) */
		reached[9][207] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 208, 287) */
		reached[9][208] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 70: // STATE 209 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][209] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 71: // STATE 210 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[9][210] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 72: // STATE 211 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][211] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 73: // STATE 214 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[9][214] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 74: // STATE 215 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][215] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 75: // STATE 218 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[9][218] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 76: // STATE 219 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][219] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 77: // STATE 223 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (287:0:1 - 1)
		IfNotBlocked
		reached[9][223] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 225, 287) */
		reached[9][225] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 226, 287) */
		reached[9][226] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 78: // STATE 228 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][228] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 79: // STATE 232 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][232] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 80: // STATE 235 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][235] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 81: // STATE 236 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[9][236] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 82: // STATE 239 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[9][239] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 83: // STATE 243 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][243] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 84: // STATE 244 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (287:0:1 - 1)
		IfNotBlocked
		reached[9][244] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(287, 250, 287) */
		reached[9][250] = 1;
		;
		/* merge: transition_id = -(1)(287, 251, 287) */
		reached[9][251] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 252, 287) */
		reached[9][252] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 85: // STATE 247 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (287:0:1 - 1)
		IfNotBlocked
		reached[9][247] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(287, 250, 287) */
		reached[9][250] = 1;
		;
		/* merge: transition_id = -(1)(287, 251, 287) */
		reached[9][251] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 252, 287) */
		reached[9][252] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 86: // STATE 251 - modemanager.pml:73 - [transition_id = -(1)] (0:287:1 - 3)
		IfNotBlocked
		reached[9][251] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 252, 287) */
		reached[9][252] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 87: // STATE 253 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][253] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 88: // STATE 257 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][257] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 89: // STATE 260 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (287:0:1 - 1)
		IfNotBlocked
		reached[9][260] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 261, 287) */
		reached[9][261] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 262, 287) */
		reached[9][262] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 90: // STATE 263 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (287:0:2 - 1)
		IfNotBlocked
		reached[9][263] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(287, 264, 287) */
		reached[9][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(287, 265, 287) */
		reached[9][265] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 266, 287) */
		reached[9][266] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 91: // STATE 269 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][269] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 92: // STATE 273 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][273] = 1;
		if (!((((P9 *)_this)->_1061_10_12_37_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1061_10_12_37_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1061_10_12_37_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 93: // STATE 276 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (287:0:1 - 1)
		IfNotBlocked
		reached[9][276] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 277, 287) */
		reached[9][277] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 278, 287) */
		reached[9][278] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 94: // STATE 279 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (287:0:2 - 1)
		IfNotBlocked
		reached[9][279] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(287, 280, 287) */
		reached[9][280] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1061_10_12_37_transition_id;
		((P9 *)_this)->_1061_10_12_37_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1061_10_12_37_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(287, 281, 287) */
		reached[9][281] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 282, 287) */
		reached[9][282] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 95: // STATE 293 - system.pml:276 - [Modemanager_lock!1] (296:0:1 - 3)
		IfNotBlocked
		reached[9][293] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 0, 0, 1);
		/* merge: inited = 1(0, 294, 296) */
		reached[9][294] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.inited;
		now.inited = 1;
#ifdef VAR_RANGES
		logval("inited", now.inited);
#endif
		;
		_m = 2; goto P999; /* 1 */
	case 96: // STATE 296 - system.pml:279 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[9][296] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Sensor_reading */
	case 97: // STATE 1 - system.pml:256 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[8][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 98: // STATE 2 - system.pml:258 - [value = 0] (0:25:3 - 1)
		IfNotBlocked
		reached[8][2] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P8 *)_this)->value;
		((P8 *)_this)->value = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		/* merge: inputVectorCounter = 0(25, 3, 25) */
		reached[8][3] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P8 *)_this)->inputVectorCounter;
		((P8 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:inputVectorCounter", ((P8 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: inputVectorCounter = 0(25, 4, 25) */
		reached[8][4] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((P8 *)_this)->inputVectorCounter;
		((P8 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:inputVectorCounter", ((P8 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 26, 25) */
		reached[8][26] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 99: // STATE 5 - system.pml:259 - [((inputVectorCounter<=3))] (0:0:0 - 1)
		IfNotBlocked
		reached[8][5] = 1;
		if (!((((P8 *)_this)->inputVectorCounter<=3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 100: // STATE 6 - env_inlines.pml:7 - [DataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[8][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1060_9_8_29_DataItem_tmp;
		((P8 *)_this)->_1060_9_8_29_DataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:DataItem_tmp", ((P8 *)_this)->_1060_9_8_29_DataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 101: // STATE 7 - env_inlines.pml:9 - [DataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[8][7] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1060_9_8_29_DataItem_tmp;
		((P8 *)_this)->_1060_9_8_29_DataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:DataItem_tmp", ((P8 *)_this)->_1060_9_8_29_DataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 102: // STATE 10 - env_inlines.pml:9 - [goto :b26] (0:20:1 - 1)
		IfNotBlocked
		reached[8][10] = 1;
		;
		/* merge: value = DataItem_tmp(20, 17, 20) */
		reached[8][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = ((P8 *)_this)->_1060_9_8_29_DataItem_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 103: // STATE 8 - env_inlines.pml:9 - [((DataItem_tmp<255))] (11:0:1 - 1)
		IfNotBlocked
		reached[8][8] = 1;
		if (!((((P8 *)_this)->_1060_9_8_29_DataItem_tmp<255)))
			continue;
		/* merge: DataItem_tmp = (DataItem_tmp+1)(0, 9, 11) */
		reached[8][9] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1060_9_8_29_DataItem_tmp;
		((P8 *)_this)->_1060_9_8_29_DataItem_tmp = (((P8 *)_this)->_1060_9_8_29_DataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:DataItem_tmp", ((P8 *)_this)->_1060_9_8_29_DataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 12, 11) */
		reached[8][12] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 104: // STATE 17 - env_inlines.pml:12 - [value = DataItem_tmp] (0:20:1 - 3)
		IfNotBlocked
		reached[8][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = ((P8 *)_this)->_1060_9_8_29_DataItem_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 105: // STATE 19 - system.pml:81 - [Modemanager_reading_channel!value] (25:0:1 - 1)
		IfNotBlocked
		reached[8][19] = 1;
		if (q_full(now.Modemanager_reading_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_reading_channel);
		sprintf(simtmp, "%d", ((P8 *)_this)->value); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_reading_channel, 0, ((P8 *)_this)->value, 0, 0, 0, 0, 1);
		/* merge: inputVectorCounter = (inputVectorCounter+1)(0, 22, 25) */
		reached[8][22] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->inputVectorCounter;
		((P8 *)_this)->inputVectorCounter = (((P8 *)_this)->inputVectorCounter+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:inputVectorCounter", ((P8 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 26, 25) */
		reached[8][26] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 106: // STATE 28 - system.pml:266 - [-end-] (0:0:0 - 3)
		IfNotBlocked
		reached[8][28] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_systemTc */
	case 107: // STATE 1 - system.pml:235 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[7][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 108: // STATE 2 - system.pml:238 - [(nempty(Modemanager_systemTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][2] = 1;
		if (!((q_len(now.Modemanager_systemTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 109: // STATE 3 - system.pml:239 - [Modemanager_lock?_] (0:0:1 - 1)
		reached[7][3] = 1;
		if (q_len(now.Modemanager_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Modemanager_lock, XX-1, 0, 0);
		;
		qrecv(now.Modemanager_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 110: // STATE 4 - system.pml:242 - [(nempty(Modemanager_systemTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][4] = 1;
		if (!((q_len(now.Modemanager_systemTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 111: // STATE 5 - system.pml:243 - [Modemanager_systemTc_channel?Modemanager_systemtc_signal_parameter.data.goToIdle,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2,Modemanager_systemtc_signal_parameter.data.doReport,Modemanager_systemtc_signal_parameter.selection] (353:0:6 - 1)
		reached[7][5] = 1;
		if (q_len(now.Modemanager_systemTc_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(6);
		(trpt+1)->bup.ovals[0] = now.Modemanager_systemtc_signal_parameter.data.goToIdle;
		(trpt+1)->bup.ovals[1] = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1;
		(trpt+1)->bup.ovals[2] = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2;
		(trpt+1)->bup.ovals[3] = now.Modemanager_systemtc_signal_parameter.data.doReport;
		(trpt+1)->bup.ovals[4] = now.Modemanager_systemtc_signal_parameter.selection;
		;
		now.Modemanager_systemtc_signal_parameter.data.goToIdle = qrecv(now.Modemanager_systemTc_channel, XX-1, 0, 0);
#ifdef VAR_RANGES
		logval("Modemanager_systemtc_signal_parameter.data.goToIdle", now.Modemanager_systemtc_signal_parameter.data.goToIdle);
#endif
		;
		now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1 = qrecv(now.Modemanager_systemTc_channel, XX-1, 1, 0);
#ifdef VAR_RANGES
		logval("Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1", now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1);
#endif
		;
		now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2 = qrecv(now.Modemanager_systemTc_channel, XX-1, 2, 0);
#ifdef VAR_RANGES
		logval("Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2", now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2);
#endif
		;
		now.Modemanager_systemtc_signal_parameter.data.doReport = qrecv(now.Modemanager_systemTc_channel, XX-1, 3, 0);
#ifdef VAR_RANGES
		logval("Modemanager_systemtc_signal_parameter.data.doReport", now.Modemanager_systemtc_signal_parameter.data.doReport);
#endif
		;
		now.Modemanager_systemtc_signal_parameter.selection = qrecv(now.Modemanager_systemTc_channel, XX-1, 4, 1);
#ifdef VAR_RANGES
		logval("Modemanager_systemtc_signal_parameter.selection", now.Modemanager_systemtc_signal_parameter.selection);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_systemTc_channel);
		sprintf(simtmp, "%d", now.Modemanager_systemtc_signal_parameter.data.goToIdle); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Modemanager_systemtc_signal_parameter.data.doReport); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Modemanager_systemtc_signal_parameter.selection); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Modemanager_systemtc_channel_used = 1(0, 6, 353) */
		reached[7][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[5] = ((int)Modemanager_systemtc_channel_used);
		Modemanager_systemtc_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_systemtc_channel_used", ((int)Modemanager_systemtc_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 112: // STATE 7 - modemanager.pml:153 - [((global_state.modemanager.state==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][7] = 1;
		if (!((now.global_state.modemanager.state==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 113: // STATE 8 - dataview.pml:8674 - [global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection] (0:0:1 - 1)
		IfNotBlocked
		reached[7][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.selection;
		now.global_state.modemanager.stc.selection = now.Modemanager_systemtc_signal_parameter.selection;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.selection", now.global_state.modemanager.stc.selection);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 114: // STATE 9 - dataview.pml:8676 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][9] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 115: // STATE 10 - dataview.pml:8633 - [global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle] (0:0:1 - 1)
		IfNotBlocked
		reached[7][10] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.goToIdle;
		now.global_state.modemanager.stc.data.goToIdle = now.Modemanager_systemtc_signal_parameter.data.goToIdle;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.goToIdle", now.global_state.modemanager.stc.data.goToIdle);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 116: // STATE 11 - dataview.pml:8638 - [assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][11] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToIdle>=0)&&(now.global_state.modemanager.stc.data.goToIdle<=0)), "((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 117: // STATE 14 - dataview.pml:8678 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][14] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 118: // STATE 15 - dataview.pml:8642 - [global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.goToOperational.threshold1;
		now.global_state.modemanager.stc.data.goToOperational.threshold1 = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.goToOperational.threshold1", now.global_state.modemanager.stc.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 119: // STATE 16 - dataview.pml:8647 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][16] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold1<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 120: // STATE 19 - dataview.pml:8651 - [global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][19] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.goToOperational.threshold2;
		now.global_state.modemanager.stc.data.goToOperational.threshold2 = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.goToOperational.threshold2", now.global_state.modemanager.stc.data.goToOperational.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 121: // STATE 20 - dataview.pml:8656 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][20] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold2<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 122: // STATE 24 - dataview.pml:8680 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][24] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 123: // STATE 25 - dataview.pml:8665 - [global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport] (0:0:1 - 1)
		IfNotBlocked
		reached[7][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.doReport;
		now.global_state.modemanager.stc.data.doReport = now.Modemanager_systemtc_signal_parameter.data.doReport;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.doReport", now.global_state.modemanager.stc.data.doReport);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 124: // STATE 26 - dataview.pml:8670 - [assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][26] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.doReport>=0)&&(now.global_state.modemanager.stc.data.doReport<=0)), "((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 125: // STATE 34 - modemanager.pml:4 - [transition_id = 0] (0:174:2 - 1)
		IfNotBlocked
		reached[7][34] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: transition_id = 1(174, 35, 174) */
		reached[7][35] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id = 1;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: .(goto)(0, 175, 174) */
		reached[7][175] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 126: // STATE 36 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][36] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 127: // STATE 38 - modemanager.pml:9 - [((transition_id==0))] (173:0:3 - 1)
		IfNotBlocked
		reached[7][38] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		/* merge: transition_id = -(1)(173, 39, 173) */
		reached[7][39] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(173, 40, 173) */
		reached[7][40] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 41, 173) */
		reached[7][41] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 128: // STATE 42 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][42] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 129: // STATE 45 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][45] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 130: // STATE 46 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 131: // STATE 47 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][47] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 132: // STATE 50 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 133: // STATE 51 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][51] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 134: // STATE 54 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[7][54] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 135: // STATE 56 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (173:0:2 - 1)
		IfNotBlocked
		reached[7][56] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(173, 58, 173) */
		reached[7][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(173, 59, 173) */
		reached[7][59] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 60, 173) */
		reached[7][60] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 136: // STATE 61 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][61] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 137: // STATE 62 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 138: // STATE 63 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][63] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 139: // STATE 66 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][66] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 140: // STATE 67 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][67] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 141: // STATE 70 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[7][70] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 142: // STATE 71 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][71] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 143: // STATE 75 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (173:0:1 - 1)
		IfNotBlocked
		reached[7][75] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 77, 173) */
		reached[7][77] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 78, 173) */
		reached[7][78] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 144: // STATE 80 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 145: // STATE 84 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][84] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 146: // STATE 87 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][87] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 147: // STATE 88 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[7][88] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 148: // STATE 90 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (173:0:2 - 1)
		IfNotBlocked
		reached[7][90] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(173, 92, 173) */
		reached[7][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(173, 93, 173) */
		reached[7][93] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 94, 173) */
		reached[7][94] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 149: // STATE 95 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][95] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 150: // STATE 96 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][96] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 151: // STATE 97 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][97] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 152: // STATE 100 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][100] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 153: // STATE 101 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][101] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 154: // STATE 104 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[7][104] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 155: // STATE 105 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][105] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 156: // STATE 109 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (173:0:1 - 1)
		IfNotBlocked
		reached[7][109] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 111, 173) */
		reached[7][111] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 112, 173) */
		reached[7][112] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 157: // STATE 114 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][114] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 158: // STATE 118 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][118] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 159: // STATE 121 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][121] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 160: // STATE 122 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[7][122] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 161: // STATE 125 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[7][125] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 162: // STATE 129 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][129] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 163: // STATE 130 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (173:0:1 - 1)
		IfNotBlocked
		reached[7][130] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(173, 136, 173) */
		reached[7][136] = 1;
		;
		/* merge: transition_id = -(1)(173, 137, 173) */
		reached[7][137] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 138, 173) */
		reached[7][138] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 164: // STATE 133 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (173:0:1 - 1)
		IfNotBlocked
		reached[7][133] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(173, 136, 173) */
		reached[7][136] = 1;
		;
		/* merge: transition_id = -(1)(173, 137, 173) */
		reached[7][137] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 138, 173) */
		reached[7][138] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 165: // STATE 137 - modemanager.pml:73 - [transition_id = -(1)] (0:173:1 - 3)
		IfNotBlocked
		reached[7][137] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 138, 173) */
		reached[7][138] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 166: // STATE 139 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][139] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 167: // STATE 143 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][143] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 168: // STATE 146 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (173:0:1 - 1)
		IfNotBlocked
		reached[7][146] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 147, 173) */
		reached[7][147] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 148, 173) */
		reached[7][148] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 169: // STATE 149 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (173:0:2 - 1)
		IfNotBlocked
		reached[7][149] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(173, 150, 173) */
		reached[7][150] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(173, 151, 173) */
		reached[7][151] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 152, 173) */
		reached[7][152] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 170: // STATE 155 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][155] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 171: // STATE 159 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][159] = 1;
		if (!((((P7 *)_this)->_1059_8_7_26_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_26_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 172: // STATE 162 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (173:0:1 - 1)
		IfNotBlocked
		reached[7][162] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 163, 173) */
		reached[7][163] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 164, 173) */
		reached[7][164] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 173: // STATE 165 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (173:0:2 - 1)
		IfNotBlocked
		reached[7][165] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(173, 166, 173) */
		reached[7][166] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_26_transition_id;
		((P7 *)_this)->_1059_8_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_26_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(173, 167, 173) */
		reached[7][167] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 168, 173) */
		reached[7][168] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 174: // STATE 178 - modemanager.pml:156 - [((global_state.modemanager.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][178] = 1;
		if (!((now.global_state.modemanager.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 175: // STATE 179 - dataview.pml:8674 - [global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection] (0:0:1 - 1)
		IfNotBlocked
		reached[7][179] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.selection;
		now.global_state.modemanager.stc.selection = now.Modemanager_systemtc_signal_parameter.selection;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.selection", now.global_state.modemanager.stc.selection);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 176: // STATE 180 - dataview.pml:8676 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][180] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 177: // STATE 181 - dataview.pml:8633 - [global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle] (0:0:1 - 1)
		IfNotBlocked
		reached[7][181] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.goToIdle;
		now.global_state.modemanager.stc.data.goToIdle = now.Modemanager_systemtc_signal_parameter.data.goToIdle;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.goToIdle", now.global_state.modemanager.stc.data.goToIdle);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 178: // STATE 182 - dataview.pml:8638 - [assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][182] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToIdle>=0)&&(now.global_state.modemanager.stc.data.goToIdle<=0)), "((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 179: // STATE 185 - dataview.pml:8678 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][185] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 180: // STATE 186 - dataview.pml:8642 - [global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.goToOperational.threshold1;
		now.global_state.modemanager.stc.data.goToOperational.threshold1 = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.goToOperational.threshold1", now.global_state.modemanager.stc.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 181: // STATE 187 - dataview.pml:8647 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][187] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold1<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 182: // STATE 190 - dataview.pml:8651 - [global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][190] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.goToOperational.threshold2;
		now.global_state.modemanager.stc.data.goToOperational.threshold2 = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.goToOperational.threshold2", now.global_state.modemanager.stc.data.goToOperational.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 183: // STATE 191 - dataview.pml:8656 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][191] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold2<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 184: // STATE 195 - dataview.pml:8680 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][195] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 185: // STATE 196 - dataview.pml:8665 - [global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport] (0:0:1 - 1)
		IfNotBlocked
		reached[7][196] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stc.data.doReport;
		now.global_state.modemanager.stc.data.doReport = now.Modemanager_systemtc_signal_parameter.data.doReport;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stc.data.doReport", now.global_state.modemanager.stc.data.doReport);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 186: // STATE 197 - dataview.pml:8670 - [assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][197] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.doReport>=0)&&(now.global_state.modemanager.stc.data.doReport<=0)), "((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 187: // STATE 205 - modemanager.pml:4 - [transition_id = 0] (0:345:2 - 1)
		IfNotBlocked
		reached[7][205] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: transition_id = 2(345, 206, 345) */
		reached[7][206] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id = 2;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: .(goto)(0, 346, 345) */
		reached[7][346] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 188: // STATE 207 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][207] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 189: // STATE 209 - modemanager.pml:9 - [((transition_id==0))] (344:0:3 - 1)
		IfNotBlocked
		reached[7][209] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		/* merge: transition_id = -(1)(344, 210, 344) */
		reached[7][210] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(344, 211, 344) */
		reached[7][211] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 212, 344) */
		reached[7][212] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 190: // STATE 213 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][213] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 191: // STATE 216 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][216] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 192: // STATE 217 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][217] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 193: // STATE 218 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][218] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 194: // STATE 221 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][221] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 195: // STATE 222 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][222] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 196: // STATE 225 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[7][225] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 197: // STATE 227 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (344:0:2 - 1)
		IfNotBlocked
		reached[7][227] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(344, 229, 344) */
		reached[7][229] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(344, 230, 344) */
		reached[7][230] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 231, 344) */
		reached[7][231] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 198: // STATE 232 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][232] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 199: // STATE 233 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][233] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 200: // STATE 234 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][234] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 201: // STATE 237 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][237] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 202: // STATE 238 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][238] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 203: // STATE 241 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[7][241] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 204: // STATE 242 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][242] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 205: // STATE 246 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (344:0:1 - 1)
		IfNotBlocked
		reached[7][246] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 248, 344) */
		reached[7][248] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 249, 344) */
		reached[7][249] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 206: // STATE 251 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][251] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 207: // STATE 255 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][255] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 208: // STATE 258 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][258] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 209: // STATE 259 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[7][259] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 210: // STATE 261 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (344:0:2 - 1)
		IfNotBlocked
		reached[7][261] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(344, 263, 344) */
		reached[7][263] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(344, 264, 344) */
		reached[7][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 344) */
		reached[7][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 211: // STATE 266 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][266] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 212: // STATE 267 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][267] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 213: // STATE 268 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][268] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 214: // STATE 271 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][271] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 215: // STATE 272 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][272] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 216: // STATE 275 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[7][275] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 217: // STATE 276 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][276] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 218: // STATE 280 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (344:0:1 - 1)
		IfNotBlocked
		reached[7][280] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 282, 344) */
		reached[7][282] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 283, 344) */
		reached[7][283] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 219: // STATE 285 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][285] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 220: // STATE 289 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][289] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 221: // STATE 292 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][292] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 222: // STATE 293 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[7][293] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 223: // STATE 296 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[7][296] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 224: // STATE 300 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][300] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 225: // STATE 301 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (344:0:1 - 1)
		IfNotBlocked
		reached[7][301] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(344, 307, 344) */
		reached[7][307] = 1;
		;
		/* merge: transition_id = -(1)(344, 308, 344) */
		reached[7][308] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 309, 344) */
		reached[7][309] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 226: // STATE 304 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (344:0:1 - 1)
		IfNotBlocked
		reached[7][304] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(344, 307, 344) */
		reached[7][307] = 1;
		;
		/* merge: transition_id = -(1)(344, 308, 344) */
		reached[7][308] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 309, 344) */
		reached[7][309] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 227: // STATE 308 - modemanager.pml:73 - [transition_id = -(1)] (0:344:1 - 3)
		IfNotBlocked
		reached[7][308] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 309, 344) */
		reached[7][309] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 228: // STATE 310 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][310] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 229: // STATE 314 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][314] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 230: // STATE 317 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (344:0:1 - 1)
		IfNotBlocked
		reached[7][317] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 318, 344) */
		reached[7][318] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 319, 344) */
		reached[7][319] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 231: // STATE 320 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (344:0:2 - 1)
		IfNotBlocked
		reached[7][320] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(344, 321, 344) */
		reached[7][321] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(344, 322, 344) */
		reached[7][322] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 323, 344) */
		reached[7][323] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 232: // STATE 326 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][326] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 233: // STATE 330 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][330] = 1;
		if (!((((P7 *)_this)->_1059_8_7_28_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_8_7_28_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1059_8_7_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 234: // STATE 333 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (344:0:1 - 1)
		IfNotBlocked
		reached[7][333] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 334, 344) */
		reached[7][334] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 335, 344) */
		reached[7][335] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 235: // STATE 336 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (344:0:2 - 1)
		IfNotBlocked
		reached[7][336] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(344, 337, 344) */
		reached[7][337] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1059_8_7_28_transition_id;
		((P7 *)_this)->_1059_8_7_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1059_8_7_28_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(344, 338, 344) */
		reached[7][338] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 339, 344) */
		reached[7][339] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 236: // STATE 355 - system.pml:247 - [(empty(Modemanager_systemTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][355] = 1;
		if (!((q_len(now.Modemanager_systemTc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 237: // STATE 356 - system.pml:248 - [(1)] (359:0:0 - 1)
		IfNotBlocked
		reached[7][356] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 358, 359) */
		reached[7][358] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 238: // STATE 359 - system.pml:250 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[7][359] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 239: // STATE 364 - system.pml:253 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[7][364] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_reading */
	case 240: // STATE 1 - system.pml:214 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[6][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 241: // STATE 2 - system.pml:217 - [(nempty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][2] = 1;
		if (!((q_len(now.Modemanager_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 242: // STATE 3 - system.pml:218 - [Modemanager_lock?_] (0:0:1 - 1)
		reached[6][3] = 1;
		if (q_len(now.Modemanager_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Modemanager_lock, XX-1, 0, 0);
		;
		qrecv(now.Modemanager_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 243: // STATE 4 - system.pml:221 - [(nempty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][4] = 1;
		if (!((q_len(now.Modemanager_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 244: // STATE 5 - system.pml:222 - [Modemanager_reading_channel?Modemanager_reading_signal_parameter] (160:0:2 - 1)
		reached[6][5] = 1;
		if (q_len(now.Modemanager_reading_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Modemanager_reading_signal_parameter;
		;
		now.Modemanager_reading_signal_parameter = qrecv(now.Modemanager_reading_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Modemanager_reading_signal_parameter", now.Modemanager_reading_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_reading_channel);
		sprintf(simtmp, "%d", now.Modemanager_reading_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Modemanager_reading_channel_used = 1(0, 6, 160) */
		reached[6][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Modemanager_reading_channel_used);
		Modemanager_reading_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_reading_channel_used", ((int)Modemanager_reading_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 245: // STATE 7 - modemanager.pml:143 - [((global_state.modemanager.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][7] = 1;
		if (!((now.global_state.modemanager.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 246: // STATE 8 - dataview.pml:8574 - [global_state.modemanager.r = Modemanager_reading_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[6][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.r;
		now.global_state.modemanager.r = now.Modemanager_reading_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.r", now.global_state.modemanager.r);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 247: // STATE 9 - dataview.pml:8579 - [assert(((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.r>=0)&&(now.global_state.modemanager.r<=255)), "((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 248: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:152:2 - 1)
		IfNotBlocked
		reached[6][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: transition_id = 3(152, 13, 152) */
		reached[6][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id = 3;
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: .(goto)(0, 153, 152) */
		reached[6][153] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 249: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][14] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 250: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (151:0:3 - 1)
		IfNotBlocked
		reached[6][16] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		/* merge: transition_id = -(1)(151, 17, 151) */
		reached[6][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(151, 18, 151) */
		reached[6][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 151) */
		reached[6][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 251: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][20] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 252: // STATE 23 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][23] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 253: // STATE 24 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[6][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 254: // STATE 25 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 255: // STATE 28 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[6][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 256: // STATE 29 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 257: // STATE 32 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[6][32] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 258: // STATE 34 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (151:0:2 - 1)
		IfNotBlocked
		reached[6][34] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(151, 36, 151) */
		reached[6][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(151, 37, 151) */
		reached[6][37] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 38, 151) */
		reached[6][38] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 259: // STATE 39 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][39] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 260: // STATE 40 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[6][40] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 261: // STATE 41 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][41] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 262: // STATE 44 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[6][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 263: // STATE 45 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 264: // STATE 48 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[6][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 265: // STATE 49 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 266: // STATE 53 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
		IfNotBlocked
		reached[6][53] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 55, 151) */
		reached[6][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 151) */
		reached[6][56] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 267: // STATE 58 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[6][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 268: // STATE 62 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][62] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 269: // STATE 65 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][65] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 270: // STATE 66 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[6][66] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 271: // STATE 68 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (151:0:2 - 1)
		IfNotBlocked
		reached[6][68] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(151, 70, 151) */
		reached[6][70] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(151, 71, 151) */
		reached[6][71] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 72, 151) */
		reached[6][72] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 272: // STATE 73 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][73] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 273: // STATE 74 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[6][74] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 274: // STATE 75 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][75] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 275: // STATE 78 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[6][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 276: // STATE 79 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][79] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 277: // STATE 82 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[6][82] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 278: // STATE 83 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][83] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 279: // STATE 87 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
		IfNotBlocked
		reached[6][87] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 89, 151) */
		reached[6][89] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 151) */
		reached[6][90] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 280: // STATE 92 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[6][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 281: // STATE 96 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][96] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 282: // STATE 99 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][99] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 283: // STATE 100 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[6][100] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 284: // STATE 103 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[6][103] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 285: // STATE 107 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][107] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 286: // STATE 108 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (151:0:1 - 1)
		IfNotBlocked
		reached[6][108] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(151, 114, 151) */
		reached[6][114] = 1;
		;
		/* merge: transition_id = -(1)(151, 115, 151) */
		reached[6][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[6][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 287: // STATE 111 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (151:0:1 - 1)
		IfNotBlocked
		reached[6][111] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(151, 114, 151) */
		reached[6][114] = 1;
		;
		/* merge: transition_id = -(1)(151, 115, 151) */
		reached[6][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[6][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 288: // STATE 115 - modemanager.pml:73 - [transition_id = -(1)] (0:151:1 - 3)
		IfNotBlocked
		reached[6][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[6][116] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 289: // STATE 117 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][117] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 290: // STATE 121 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][121] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 291: // STATE 124 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[6][124] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 125, 151) */
		reached[6][125] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 126, 151) */
		reached[6][126] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 292: // STATE 127 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[6][127] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 128, 151) */
		reached[6][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(151, 129, 151) */
		reached[6][129] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 130, 151) */
		reached[6][130] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 293: // STATE 133 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][133] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 294: // STATE 137 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][137] = 1;
		if (!((((P6 *)_this)->_1058_7_6_24_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_7_6_24_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1058_7_6_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 295: // STATE 140 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[6][140] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 141, 151) */
		reached[6][141] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 142, 151) */
		reached[6][142] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 296: // STATE 143 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[6][143] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 144, 151) */
		reached[6][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1058_7_6_24_transition_id;
		((P6 *)_this)->_1058_7_6_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1058_7_6_24_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(151, 145, 151) */
		reached[6][145] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 146, 151) */
		reached[6][146] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 297: // STATE 162 - system.pml:226 - [(empty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][162] = 1;
		if (!((q_len(now.Modemanager_reading_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 298: // STATE 163 - system.pml:227 - [(1)] (166:0:0 - 1)
		IfNotBlocked
		reached[6][163] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 165, 166) */
		reached[6][165] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 299: // STATE 166 - system.pml:229 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[6][166] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 300: // STATE 171 - system.pml:232 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[6][171] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_actuatorTm2 */
	case 301: // STATE 1 - system.pml:193 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[5][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 302: // STATE 2 - system.pml:196 - [(nempty(Modemanager_actuatorTm2_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][2] = 1;
		if (!((q_len(now.Modemanager_actuatorTm2_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 303: // STATE 3 - system.pml:197 - [Modemanager_lock?_] (0:0:1 - 1)
		reached[5][3] = 1;
		if (q_len(now.Modemanager_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Modemanager_lock, XX-1, 0, 0);
		;
		qrecv(now.Modemanager_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 304: // STATE 4 - system.pml:200 - [(nempty(Modemanager_actuatorTm2_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][4] = 1;
		if (!((q_len(now.Modemanager_actuatorTm2_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 305: // STATE 5 - system.pml:201 - [Modemanager_actuatorTm2_channel?Modemanager_actuatortm2_signal_parameter] (309:0:2 - 1)
		reached[5][5] = 1;
		if (q_len(now.Modemanager_actuatorTm2_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Modemanager_actuatortm2_signal_parameter;
		;
		now.Modemanager_actuatortm2_signal_parameter = qrecv(now.Modemanager_actuatorTm2_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatortm2_signal_parameter", now.Modemanager_actuatortm2_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", now.Modemanager_actuatortm2_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Modemanager_actuatortm2_channel_used = 1(0, 6, 309) */
		reached[5][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Modemanager_actuatortm2_channel_used);
		Modemanager_actuatortm2_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_actuatortm2_channel_used", ((int)Modemanager_actuatortm2_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 306: // STATE 7 - modemanager.pml:130 - [((global_state.modemanager.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][7] = 1;
		if (!((now.global_state.modemanager.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 307: // STATE 8 - dataview.pml:8601 - [global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[5][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.status2;
		now.global_state.modemanager.status2 = now.Modemanager_actuatortm2_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.status2", now.global_state.modemanager.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 308: // STATE 9 - dataview.pml:8606 - [assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status2==0)||(now.global_state.modemanager.status2==1))||(now.global_state.modemanager.status2==2)), "(((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 309: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:152:2 - 1)
		IfNotBlocked
		reached[5][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: transition_id = 4(152, 13, 152) */
		reached[5][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id = 4;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: .(goto)(0, 153, 152) */
		reached[5][153] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 310: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][14] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 311: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (151:0:3 - 1)
		IfNotBlocked
		reached[5][16] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		/* merge: transition_id = -(1)(151, 17, 151) */
		reached[5][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(151, 18, 151) */
		reached[5][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 151) */
		reached[5][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 312: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][20] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 313: // STATE 23 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][23] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 314: // STATE 24 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[5][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 315: // STATE 25 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 316: // STATE 28 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[5][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 317: // STATE 29 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 318: // STATE 32 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[5][32] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 319: // STATE 34 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (151:0:2 - 1)
		IfNotBlocked
		reached[5][34] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(151, 36, 151) */
		reached[5][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(151, 37, 151) */
		reached[5][37] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 38, 151) */
		reached[5][38] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 320: // STATE 39 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][39] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 321: // STATE 40 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[5][40] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 322: // STATE 41 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][41] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 323: // STATE 44 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[5][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 324: // STATE 45 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 325: // STATE 48 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[5][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 326: // STATE 49 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 327: // STATE 53 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
		IfNotBlocked
		reached[5][53] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 55, 151) */
		reached[5][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 151) */
		reached[5][56] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 328: // STATE 58 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 329: // STATE 62 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][62] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 330: // STATE 65 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][65] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 331: // STATE 66 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[5][66] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 332: // STATE 68 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (151:0:2 - 1)
		IfNotBlocked
		reached[5][68] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(151, 70, 151) */
		reached[5][70] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(151, 71, 151) */
		reached[5][71] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 72, 151) */
		reached[5][72] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 333: // STATE 73 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][73] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 334: // STATE 74 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[5][74] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 335: // STATE 75 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][75] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 336: // STATE 78 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[5][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 337: // STATE 79 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][79] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 338: // STATE 82 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[5][82] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 339: // STATE 83 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][83] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 340: // STATE 87 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
		IfNotBlocked
		reached[5][87] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 89, 151) */
		reached[5][89] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 151) */
		reached[5][90] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 341: // STATE 92 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 342: // STATE 96 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][96] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 343: // STATE 99 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][99] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 344: // STATE 100 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[5][100] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 345: // STATE 103 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[5][103] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 346: // STATE 107 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][107] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 347: // STATE 108 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (151:0:1 - 1)
		IfNotBlocked
		reached[5][108] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(151, 114, 151) */
		reached[5][114] = 1;
		;
		/* merge: transition_id = -(1)(151, 115, 151) */
		reached[5][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[5][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 348: // STATE 111 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (151:0:1 - 1)
		IfNotBlocked
		reached[5][111] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(151, 114, 151) */
		reached[5][114] = 1;
		;
		/* merge: transition_id = -(1)(151, 115, 151) */
		reached[5][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[5][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 349: // STATE 115 - modemanager.pml:73 - [transition_id = -(1)] (0:151:1 - 3)
		IfNotBlocked
		reached[5][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[5][116] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 350: // STATE 117 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][117] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 351: // STATE 121 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][121] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 352: // STATE 124 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[5][124] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 125, 151) */
		reached[5][125] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 126, 151) */
		reached[5][126] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 353: // STATE 127 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[5][127] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 128, 151) */
		reached[5][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(151, 129, 151) */
		reached[5][129] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 130, 151) */
		reached[5][130] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 354: // STATE 133 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][133] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 355: // STATE 137 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][137] = 1;
		if (!((((P5 *)_this)->_1057_6_5_20_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_20_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 356: // STATE 140 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[5][140] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 141, 151) */
		reached[5][141] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 142, 151) */
		reached[5][142] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 357: // STATE 143 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[5][143] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 144, 151) */
		reached[5][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_20_transition_id;
		((P5 *)_this)->_1057_6_5_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_20_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(151, 145, 151) */
		reached[5][145] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 146, 151) */
		reached[5][146] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 358: // STATE 156 - modemanager.pml:133 - [((global_state.modemanager.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][156] = 1;
		if (!((now.global_state.modemanager.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 359: // STATE 157 - dataview.pml:8601 - [global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[5][157] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.status2;
		now.global_state.modemanager.status2 = now.Modemanager_actuatortm2_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.status2", now.global_state.modemanager.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 360: // STATE 158 - dataview.pml:8606 - [assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][158] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status2==0)||(now.global_state.modemanager.status2==1))||(now.global_state.modemanager.status2==2)), "(((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 361: // STATE 161 - modemanager.pml:4 - [transition_id = 0] (0:301:2 - 1)
		IfNotBlocked
		reached[5][161] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: transition_id = 6(301, 162, 301) */
		reached[5][162] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id = 6;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: .(goto)(0, 302, 301) */
		reached[5][302] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 362: // STATE 163 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][163] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 363: // STATE 165 - modemanager.pml:9 - [((transition_id==0))] (300:0:3 - 1)
		IfNotBlocked
		reached[5][165] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		/* merge: transition_id = -(1)(300, 166, 300) */
		reached[5][166] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(300, 167, 300) */
		reached[5][167] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 168, 300) */
		reached[5][168] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 364: // STATE 169 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][169] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 365: // STATE 172 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][172] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 366: // STATE 173 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[5][173] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 367: // STATE 174 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][174] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 368: // STATE 177 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[5][177] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 369: // STATE 178 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][178] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 370: // STATE 181 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[5][181] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 371: // STATE 183 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (300:0:2 - 1)
		IfNotBlocked
		reached[5][183] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(300, 185, 300) */
		reached[5][185] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(300, 186, 300) */
		reached[5][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 187, 300) */
		reached[5][187] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 372: // STATE 188 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][188] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 373: // STATE 189 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[5][189] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 374: // STATE 190 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][190] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 375: // STATE 193 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[5][193] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 376: // STATE 194 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][194] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 377: // STATE 197 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[5][197] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 378: // STATE 198 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][198] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 379: // STATE 202 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
		IfNotBlocked
		reached[5][202] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 204, 300) */
		reached[5][204] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 205, 300) */
		reached[5][205] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 380: // STATE 207 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][207] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 381: // STATE 211 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][211] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 382: // STATE 214 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][214] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 383: // STATE 215 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[5][215] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 384: // STATE 217 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (300:0:2 - 1)
		IfNotBlocked
		reached[5][217] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(300, 219, 300) */
		reached[5][219] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(300, 220, 300) */
		reached[5][220] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 221, 300) */
		reached[5][221] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 385: // STATE 222 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][222] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 386: // STATE 223 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[5][223] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 387: // STATE 224 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][224] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 388: // STATE 227 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[5][227] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 389: // STATE 228 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][228] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 390: // STATE 231 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[5][231] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 391: // STATE 232 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][232] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 392: // STATE 236 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
		IfNotBlocked
		reached[5][236] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 238, 300) */
		reached[5][238] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 239, 300) */
		reached[5][239] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 393: // STATE 241 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][241] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 394: // STATE 245 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][245] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 395: // STATE 248 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][248] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 396: // STATE 249 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[5][249] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 397: // STATE 252 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[5][252] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 398: // STATE 256 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][256] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 399: // STATE 257 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (300:0:1 - 1)
		IfNotBlocked
		reached[5][257] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(300, 263, 300) */
		reached[5][263] = 1;
		;
		/* merge: transition_id = -(1)(300, 264, 300) */
		reached[5][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[5][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 400: // STATE 260 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (300:0:1 - 1)
		IfNotBlocked
		reached[5][260] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(300, 263, 300) */
		reached[5][263] = 1;
		;
		/* merge: transition_id = -(1)(300, 264, 300) */
		reached[5][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[5][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 401: // STATE 264 - modemanager.pml:73 - [transition_id = -(1)] (0:300:1 - 3)
		IfNotBlocked
		reached[5][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[5][265] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 402: // STATE 266 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][266] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 403: // STATE 270 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][270] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 404: // STATE 273 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[5][273] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 274, 300) */
		reached[5][274] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 275, 300) */
		reached[5][275] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 405: // STATE 276 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[5][276] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 277, 300) */
		reached[5][277] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(300, 278, 300) */
		reached[5][278] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 279, 300) */
		reached[5][279] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 406: // STATE 282 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][282] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 407: // STATE 286 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][286] = 1;
		if (!((((P5 *)_this)->_1057_6_5_22_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1057_6_5_22_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1057_6_5_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 408: // STATE 289 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[5][289] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 290, 300) */
		reached[5][290] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 291, 300) */
		reached[5][291] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 409: // STATE 292 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[5][292] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 293, 300) */
		reached[5][293] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1057_6_5_22_transition_id;
		((P5 *)_this)->_1057_6_5_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1057_6_5_22_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(300, 294, 300) */
		reached[5][294] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 295, 300) */
		reached[5][295] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 410: // STATE 311 - system.pml:205 - [(empty(Modemanager_actuatorTm2_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][311] = 1;
		if (!((q_len(now.Modemanager_actuatorTm2_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 411: // STATE 312 - system.pml:206 - [(1)] (315:0:0 - 1)
		IfNotBlocked
		reached[5][312] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 314, 315) */
		reached[5][314] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 412: // STATE 315 - system.pml:208 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[5][315] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 413: // STATE 320 - system.pml:211 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[5][320] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_actuatorTm1 */
	case 414: // STATE 1 - system.pml:172 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[4][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 415: // STATE 2 - system.pml:175 - [(nempty(Modemanager_actuatorTm1_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][2] = 1;
		if (!((q_len(now.Modemanager_actuatorTm1_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 416: // STATE 3 - system.pml:176 - [Modemanager_lock?_] (0:0:1 - 1)
		reached[4][3] = 1;
		if (q_len(now.Modemanager_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Modemanager_lock, XX-1, 0, 0);
		;
		qrecv(now.Modemanager_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 417: // STATE 4 - system.pml:179 - [(nempty(Modemanager_actuatorTm1_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][4] = 1;
		if (!((q_len(now.Modemanager_actuatorTm1_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 418: // STATE 5 - system.pml:180 - [Modemanager_actuatorTm1_channel?Modemanager_actuatortm1_signal_parameter] (309:0:2 - 1)
		reached[4][5] = 1;
		if (q_len(now.Modemanager_actuatorTm1_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Modemanager_actuatortm1_signal_parameter;
		;
		now.Modemanager_actuatortm1_signal_parameter = qrecv(now.Modemanager_actuatorTm1_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatortm1_signal_parameter", now.Modemanager_actuatortm1_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", now.Modemanager_actuatortm1_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Modemanager_actuatortm1_channel_used = 1(0, 6, 309) */
		reached[4][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Modemanager_actuatortm1_channel_used);
		Modemanager_actuatortm1_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_actuatortm1_channel_used", ((int)Modemanager_actuatortm1_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 419: // STATE 7 - modemanager.pml:117 - [((global_state.modemanager.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][7] = 1;
		if (!((now.global_state.modemanager.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 420: // STATE 8 - dataview.pml:8601 - [global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[4][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.status1;
		now.global_state.modemanager.status1 = now.Modemanager_actuatortm1_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.status1", now.global_state.modemanager.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 421: // STATE 9 - dataview.pml:8606 - [assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status1==0)||(now.global_state.modemanager.status1==1))||(now.global_state.modemanager.status1==2)), "(((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 422: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:152:2 - 1)
		IfNotBlocked
		reached[4][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: transition_id = 5(152, 13, 152) */
		reached[4][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id = 5;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: .(goto)(0, 153, 152) */
		reached[4][153] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 423: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][14] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 424: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (151:0:3 - 1)
		IfNotBlocked
		reached[4][16] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		/* merge: transition_id = -(1)(151, 17, 151) */
		reached[4][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(151, 18, 151) */
		reached[4][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 151) */
		reached[4][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 425: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][20] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 426: // STATE 23 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][23] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 427: // STATE 24 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[4][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 428: // STATE 25 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 429: // STATE 28 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[4][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 430: // STATE 29 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 431: // STATE 32 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[4][32] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 432: // STATE 34 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (151:0:2 - 1)
		IfNotBlocked
		reached[4][34] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(151, 36, 151) */
		reached[4][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(151, 37, 151) */
		reached[4][37] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 38, 151) */
		reached[4][38] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 433: // STATE 39 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][39] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 434: // STATE 40 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[4][40] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 435: // STATE 41 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][41] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 436: // STATE 44 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[4][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 437: // STATE 45 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 438: // STATE 48 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[4][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 439: // STATE 49 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 440: // STATE 53 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
		IfNotBlocked
		reached[4][53] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 55, 151) */
		reached[4][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 151) */
		reached[4][56] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 441: // STATE 58 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 442: // STATE 62 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][62] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 443: // STATE 65 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][65] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 444: // STATE 66 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[4][66] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 445: // STATE 68 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (151:0:2 - 1)
		IfNotBlocked
		reached[4][68] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(151, 70, 151) */
		reached[4][70] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(151, 71, 151) */
		reached[4][71] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 72, 151) */
		reached[4][72] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 446: // STATE 73 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][73] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 447: // STATE 74 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[4][74] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 448: // STATE 75 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][75] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 449: // STATE 78 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[4][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 450: // STATE 79 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][79] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 451: // STATE 82 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[4][82] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 452: // STATE 83 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][83] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 453: // STATE 87 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
		IfNotBlocked
		reached[4][87] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 89, 151) */
		reached[4][89] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 151) */
		reached[4][90] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 454: // STATE 92 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 455: // STATE 96 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][96] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 456: // STATE 99 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][99] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 457: // STATE 100 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[4][100] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 458: // STATE 103 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[4][103] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 459: // STATE 107 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][107] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 460: // STATE 108 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (151:0:1 - 1)
		IfNotBlocked
		reached[4][108] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(151, 114, 151) */
		reached[4][114] = 1;
		;
		/* merge: transition_id = -(1)(151, 115, 151) */
		reached[4][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[4][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 461: // STATE 111 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (151:0:1 - 1)
		IfNotBlocked
		reached[4][111] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(151, 114, 151) */
		reached[4][114] = 1;
		;
		/* merge: transition_id = -(1)(151, 115, 151) */
		reached[4][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[4][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 462: // STATE 115 - modemanager.pml:73 - [transition_id = -(1)] (0:151:1 - 3)
		IfNotBlocked
		reached[4][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[4][116] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 463: // STATE 117 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][117] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 464: // STATE 121 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][121] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 465: // STATE 124 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[4][124] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 125, 151) */
		reached[4][125] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 126, 151) */
		reached[4][126] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 466: // STATE 127 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[4][127] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 128, 151) */
		reached[4][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(151, 129, 151) */
		reached[4][129] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 130, 151) */
		reached[4][130] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 467: // STATE 133 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][133] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 468: // STATE 137 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][137] = 1;
		if (!((((P4 *)_this)->_1056_5_4_16_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_16_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_16_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 469: // STATE 140 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[4][140] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 141, 151) */
		reached[4][141] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 142, 151) */
		reached[4][142] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 470: // STATE 143 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[4][143] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 144, 151) */
		reached[4][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_16_transition_id;
		((P4 *)_this)->_1056_5_4_16_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_16_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(151, 145, 151) */
		reached[4][145] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 146, 151) */
		reached[4][146] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 471: // STATE 156 - modemanager.pml:120 - [((global_state.modemanager.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][156] = 1;
		if (!((now.global_state.modemanager.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 472: // STATE 157 - dataview.pml:8601 - [global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[4][157] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.status1;
		now.global_state.modemanager.status1 = now.Modemanager_actuatortm1_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.status1", now.global_state.modemanager.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 473: // STATE 158 - dataview.pml:8606 - [assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][158] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status1==0)||(now.global_state.modemanager.status1==1))||(now.global_state.modemanager.status1==2)), "(((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 474: // STATE 161 - modemanager.pml:4 - [transition_id = 0] (0:301:2 - 1)
		IfNotBlocked
		reached[4][161] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: transition_id = 7(301, 162, 301) */
		reached[4][162] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id = 7;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: .(goto)(0, 302, 301) */
		reached[4][302] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 475: // STATE 163 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][163] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 476: // STATE 165 - modemanager.pml:9 - [((transition_id==0))] (300:0:3 - 1)
		IfNotBlocked
		reached[4][165] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		/* merge: transition_id = -(1)(300, 166, 300) */
		reached[4][166] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(300, 167, 300) */
		reached[4][167] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 168, 300) */
		reached[4][168] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 477: // STATE 169 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][169] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 478: // STATE 172 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][172] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 479: // STATE 173 - dataview.pml:8574 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[4][173] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold1;
		now.global_state.modemanager.threshold1 = now.global_state.modemanager.stc.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold1", now.global_state.modemanager.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 480: // STATE 174 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][174] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 481: // STATE 177 - dataview.pml:8574 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[4][177] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.threshold2;
		now.global_state.modemanager.threshold2 = now.global_state.modemanager.stc.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.threshold2", now.global_state.modemanager.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 482: // STATE 178 - dataview.pml:8579 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][178] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 483: // STATE 181 - system.pml:41 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
		IfNotBlocked
		reached[4][181] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 484: // STATE 183 - system.pml:55 - [Actuator2_actuatorTc_channel!0] (300:0:2 - 1)
		IfNotBlocked
		reached[4][183] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(300, 185, 300) */
		reached[4][185] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(300, 186, 300) */
		reached[4][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 187, 300) */
		reached[4][187] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 485: // STATE 188 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][188] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 486: // STATE 189 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[4][189] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 487: // STATE 190 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][190] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 488: // STATE 193 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[4][193] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 489: // STATE 194 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][194] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 490: // STATE 197 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[4][197] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 491: // STATE 198 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][198] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 492: // STATE 202 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
		IfNotBlocked
		reached[4][202] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 204, 300) */
		reached[4][204] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 205, 300) */
		reached[4][205] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 493: // STATE 207 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][207] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 494: // STATE 211 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][211] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 495: // STATE 214 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][214] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 496: // STATE 215 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[4][215] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 497: // STATE 217 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (300:0:2 - 1)
		IfNotBlocked
		reached[4][217] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(300, 219, 300) */
		reached[4][219] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 2(300, 220, 300) */
		reached[4][220] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 221, 300) */
		reached[4][221] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 498: // STATE 222 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][222] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 499: // STATE 223 - dataview.pml:8601 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[4][223] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status1;
		now.global_state.modemanager.stm.status1 = now.global_state.modemanager.status1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status1", now.global_state.modemanager.stm.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 500: // STATE 224 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][224] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 501: // STATE 227 - dataview.pml:8601 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[4][227] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.status2;
		now.global_state.modemanager.stm.status2 = now.global_state.modemanager.status2;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.status2", now.global_state.modemanager.stm.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 502: // STATE 228 - dataview.pml:8606 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][228] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 503: // STATE 231 - dataview.pml:8574 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
		IfNotBlocked
		reached[4][231] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.stm.reading;
		now.global_state.modemanager.stm.reading = now.global_state.modemanager.r;
#ifdef VAR_RANGES
		logval("global_state.modemanager.stm.reading", now.global_state.modemanager.stm.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 504: // STATE 232 - dataview.pml:8579 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][232] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 505: // STATE 236 - system.pml:69 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
		IfNotBlocked
		reached[4][236] = 1;
		if (q_full(now.Egse_systemTm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.stm.status2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_systemTm_channel, 0, now.global_state.modemanager.stm.reading, now.global_state.modemanager.stm.status1, now.global_state.modemanager.stm.status2, 0, 0, 3);
		/* merge: transition_id = -(1)(0, 238, 300) */
		reached[4][238] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 239, 300) */
		reached[4][239] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 506: // STATE 241 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][241] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 507: // STATE 245 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][245] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 508: // STATE 248 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][248] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 509: // STATE 249 - system.pml:41 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
		IfNotBlocked
		reached[4][249] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 510: // STATE 252 - system.pml:41 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
		IfNotBlocked
		reached[4][252] = 1;
		if (q_full(now.Actuator1_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 511: // STATE 256 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][256] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 512: // STATE 257 - system.pml:55 - [Actuator2_actuatorTc_channel!1] (300:0:1 - 1)
		IfNotBlocked
		reached[4][257] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: .(goto)(300, 263, 300) */
		reached[4][263] = 1;
		;
		/* merge: transition_id = -(1)(300, 264, 300) */
		reached[4][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[4][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 513: // STATE 260 - system.pml:55 - [Actuator2_actuatorTc_channel!2] (300:0:1 - 1)
		IfNotBlocked
		reached[4][260] = 1;
		if (q_full(now.Actuator2_actuatorTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_actuatorTc_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: .(goto)(300, 263, 300) */
		reached[4][263] = 1;
		;
		/* merge: transition_id = -(1)(300, 264, 300) */
		reached[4][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[4][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 514: // STATE 264 - modemanager.pml:73 - [transition_id = -(1)] (0:300:1 - 3)
		IfNotBlocked
		reached[4][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[4][265] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 515: // STATE 266 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][266] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 516: // STATE 270 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][270] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 517: // STATE 273 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[4][273] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 274, 300) */
		reached[4][274] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 275, 300) */
		reached[4][275] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 518: // STATE 276 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[4][276] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 277, 300) */
		reached[4][277] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(300, 278, 300) */
		reached[4][278] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 279, 300) */
		reached[4][279] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 519: // STATE 282 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][282] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 520: // STATE 286 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][286] = 1;
		if (!((((P4 *)_this)->_1056_5_4_18_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1056_5_4_18_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1056_5_4_18_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 521: // STATE 289 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[4][289] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 290, 300) */
		reached[4][290] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 291, 300) */
		reached[4][291] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 522: // STATE 292 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[4][292] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 293, 300) */
		reached[4][293] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1056_5_4_18_transition_id;
		((P4 *)_this)->_1056_5_4_18_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1056_5_4_18_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 3(300, 294, 300) */
		reached[4][294] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 3;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 295, 300) */
		reached[4][295] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 523: // STATE 311 - system.pml:184 - [(empty(Modemanager_actuatorTm1_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][311] = 1;
		if (!((q_len(now.Modemanager_actuatorTm1_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 524: // STATE 312 - system.pml:185 - [(1)] (315:0:0 - 1)
		IfNotBlocked
		reached[4][312] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 314, 315) */
		reached[4][314] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 525: // STATE 315 - system.pml:187 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[4][315] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 526: // STATE 320 - system.pml:190 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[4][320] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Egse_systemTc */
	case 527: // STATE 1 - system.pml:159 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[3][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 528: // STATE 2 - system.pml:161 - [] (0:74:2 - 1)
		IfNotBlocked
		reached[3][2] = 1;
		;
		/* merge: inputVectorCounter = 0(74, 3, 74) */
		reached[3][3] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->inputVectorCounter;
		((P3 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:inputVectorCounter", ((P3 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: inputVectorCounter = 0(74, 4, 74) */
		reached[3][4] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->inputVectorCounter;
		((P3 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:inputVectorCounter", ((P3 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 75, 74) */
		reached[3][75] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 529: // STATE 5 - system.pml:162 - [((inputVectorCounter<=3))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][5] = 1;
		if (!((((P3 *)_this)->inputVectorCounter<=3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 530: // STATE 7 - env_inlines.pml:17 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][7] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp;
		((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 531: // STATE 8 - env_inlines.pml:19 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp;
		((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 532: // STATE 11 - env_inlines.pml:19 - [goto :b10] (0:69:2 - 1)
		IfNotBlocked
		reached[3][11] = 1;
		;
		/* merge: value.data.goToIdle = Dummy_tmp(69, 18, 69) */
		reached[3][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToIdle;
		((P3 *)_this)->value.data.goToIdle = ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToIdle", ((P3 *)_this)->value.data.goToIdle);
#endif
		;
		/* merge: value.selection = 1(69, 20, 69) */
		reached[3][20] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 1;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 66, 69) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 533: // STATE 9 - env_inlines.pml:19 - [((Dummy_tmp<0))] (12:0:1 - 1)
		IfNotBlocked
		reached[3][9] = 1;
		if (!((((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp<0)))
			continue;
		/* merge: Dummy_tmp = (Dummy_tmp+1)(0, 10, 12) */
		reached[3][10] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp;
		((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp = (((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp);
#endif
		;
		/* merge: .(goto)(0, 13, 12) */
		reached[3][13] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 534: // STATE 18 - env_inlines.pml:22 - [value.data.goToIdle = Dummy_tmp] (0:69:2 - 3)
		IfNotBlocked
		reached[3][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToIdle;
		((P3 *)_this)->value.data.goToIdle = ((P3 *)_this)->_1055_4_3_13_49_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToIdle", ((P3 *)_this)->value.data.goToIdle);
#endif
		;
		/* merge: value.selection = 1(69, 20, 69) */
		reached[3][20] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 1;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 66, 69) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 535: // STATE 22 - env_inlines.pml:7 - [DataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][22] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp;
		((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:DataItem_tmp", ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 536: // STATE 23 - env_inlines.pml:9 - [DataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp;
		((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:DataItem_tmp", ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 537: // STATE 26 - env_inlines.pml:9 - [goto :b11] (0:47:1 - 1)
		IfNotBlocked
		reached[3][26] = 1;
		;
		/* merge: value.data.goToOperational.threshold1 = DataItem_tmp(47, 33, 47) */
		reached[3][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->value.data.goToOperational.threshold1;
		((P3 *)_this)->value.data.goToOperational.threshold1 = ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold1", ((P3 *)_this)->value.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 538: // STATE 24 - env_inlines.pml:9 - [((DataItem_tmp<255))] (27:0:1 - 1)
		IfNotBlocked
		reached[3][24] = 1;
		if (!((((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp<255)))
			continue;
		/* merge: DataItem_tmp = (DataItem_tmp+1)(0, 25, 27) */
		reached[3][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp;
		((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp = (((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:DataItem_tmp", ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 28, 27) */
		reached[3][28] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 539: // STATE 33 - env_inlines.pml:12 - [value.data.goToOperational.threshold1 = DataItem_tmp] (0:47:1 - 3)
		IfNotBlocked
		reached[3][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->value.data.goToOperational.threshold1;
		((P3 *)_this)->value.data.goToOperational.threshold1 = ((P3 *)_this)->_1055_4_3_13_50_2_DataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold1", ((P3 *)_this)->value.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 540: // STATE 35 - env_inlines.pml:7 - [DataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][35] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp;
		((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:DataItem_tmp", ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 541: // STATE 36 - env_inlines.pml:9 - [DataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp;
		((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:DataItem_tmp", ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 542: // STATE 39 - env_inlines.pml:9 - [goto :b12] (0:69:2 - 1)
		IfNotBlocked
		reached[3][39] = 1;
		;
		/* merge: value.data.goToOperational.threshold2 = DataItem_tmp(69, 46, 69) */
		reached[3][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToOperational.threshold2;
		((P3 *)_this)->value.data.goToOperational.threshold2 = ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold2", ((P3 *)_this)->value.data.goToOperational.threshold2);
#endif
		;
		/* merge: value.selection = 2(69, 49, 69) */
		reached[3][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 66, 69) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 543: // STATE 37 - env_inlines.pml:9 - [((DataItem_tmp<255))] (40:0:1 - 1)
		IfNotBlocked
		reached[3][37] = 1;
		if (!((((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp<255)))
			continue;
		/* merge: DataItem_tmp = (DataItem_tmp+1)(0, 38, 40) */
		reached[3][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp;
		((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp = (((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:DataItem_tmp", ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 41, 40) */
		reached[3][41] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 544: // STATE 46 - env_inlines.pml:12 - [value.data.goToOperational.threshold2 = DataItem_tmp] (0:69:2 - 3)
		IfNotBlocked
		reached[3][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToOperational.threshold2;
		((P3 *)_this)->value.data.goToOperational.threshold2 = ((P3 *)_this)->_1055_4_3_13_50_3_DataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold2", ((P3 *)_this)->value.data.goToOperational.threshold2);
#endif
		;
		/* merge: value.selection = 2(69, 49, 69) */
		reached[3][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 66, 69) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 545: // STATE 51 - env_inlines.pml:17 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][51] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp;
		((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 546: // STATE 52 - env_inlines.pml:19 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][52] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp;
		((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 547: // STATE 55 - env_inlines.pml:19 - [goto :b13] (0:69:2 - 1)
		IfNotBlocked
		reached[3][55] = 1;
		;
		/* merge: value.data.doReport = Dummy_tmp(69, 62, 69) */
		reached[3][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.doReport;
		((P3 *)_this)->value.data.doReport = ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.doReport", ((P3 *)_this)->value.data.doReport);
#endif
		;
		/* merge: value.selection = 3(69, 64, 69) */
		reached[3][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 3;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 66, 69) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 548: // STATE 53 - env_inlines.pml:19 - [((Dummy_tmp<0))] (56:0:1 - 1)
		IfNotBlocked
		reached[3][53] = 1;
		if (!((((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp<0)))
			continue;
		/* merge: Dummy_tmp = (Dummy_tmp+1)(0, 54, 56) */
		reached[3][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp;
		((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp = (((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp);
#endif
		;
		/* merge: .(goto)(0, 57, 56) */
		reached[3][57] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 549: // STATE 62 - env_inlines.pml:22 - [value.data.doReport = Dummy_tmp] (0:69:2 - 3)
		IfNotBlocked
		reached[3][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.doReport;
		((P3 *)_this)->value.data.doReport = ((P3 *)_this)->_1055_4_3_13_51_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.doReport", ((P3 *)_this)->value.data.doReport);
#endif
		;
		/* merge: value.selection = 3(69, 64, 69) */
		reached[3][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 3;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 66, 69) */
		reached[3][66] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 550: // STATE 68 - system.pml:85 - [Modemanager_systemTc_channel!value.data.goToIdle,value.data.goToOperational.threshold1,value.data.goToOperational.threshold2,value.data.doReport,value.selection] (74:0:1 - 1)
		IfNotBlocked
		reached[3][68] = 1;
		if (q_full(now.Modemanager_systemTc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_systemTc_channel);
		sprintf(simtmp, "%d", ((P3 *)_this)->value.data.goToIdle); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", ((P3 *)_this)->value.data.goToOperational.threshold1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", ((P3 *)_this)->value.data.goToOperational.threshold2); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", ((P3 *)_this)->value.data.doReport); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", ((P3 *)_this)->value.selection); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_systemTc_channel, 0, ((P3 *)_this)->value.data.goToIdle, ((P3 *)_this)->value.data.goToOperational.threshold1, ((P3 *)_this)->value.data.goToOperational.threshold2, ((P3 *)_this)->value.data.doReport, ((P3 *)_this)->value.selection, 5);
		/* merge: inputVectorCounter = (inputVectorCounter+1)(0, 71, 74) */
		reached[3][71] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->inputVectorCounter;
		((P3 *)_this)->inputVectorCounter = (((P3 *)_this)->inputVectorCounter+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:inputVectorCounter", ((P3 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 75, 74) */
		reached[3][75] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 551: // STATE 77 - system.pml:169 - [-end-] (0:0:0 - 3)
		IfNotBlocked
		reached[3][77] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Egse_systemTm */
	case 552: // STATE 1 - system.pml:141 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[2][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 553: // STATE 2 - system.pml:144 - [(nempty(Egse_systemTm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][2] = 1;
		if (!((q_len(now.Egse_systemTm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 554: // STATE 3 - system.pml:147 - [(nempty(Egse_systemTm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][3] = 1;
		if (!((q_len(now.Egse_systemTm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 555: // STATE 4 - system.pml:148 - [Egse_systemTm_channel?Egse_systemtm_signal_parameter.reading,Egse_systemtm_signal_parameter.status1,Egse_systemtm_signal_parameter.status2] (9:0:4 - 1)
		reached[2][4] = 1;
		if (q_len(now.Egse_systemTm_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(4);
		(trpt+1)->bup.ovals[0] = now.Egse_systemtm_signal_parameter.reading;
		(trpt+1)->bup.ovals[1] = now.Egse_systemtm_signal_parameter.status1;
		(trpt+1)->bup.ovals[2] = now.Egse_systemtm_signal_parameter.status2;
		;
		now.Egse_systemtm_signal_parameter.reading = qrecv(now.Egse_systemTm_channel, XX-1, 0, 0);
#ifdef VAR_RANGES
		logval("Egse_systemtm_signal_parameter.reading", now.Egse_systemtm_signal_parameter.reading);
#endif
		;
		now.Egse_systemtm_signal_parameter.status1 = qrecv(now.Egse_systemTm_channel, XX-1, 1, 0);
#ifdef VAR_RANGES
		logval("Egse_systemtm_signal_parameter.status1", now.Egse_systemtm_signal_parameter.status1);
#endif
		;
		now.Egse_systemtm_signal_parameter.status2 = qrecv(now.Egse_systemTm_channel, XX-1, 2, 1);
#ifdef VAR_RANGES
		logval("Egse_systemtm_signal_parameter.status2", now.Egse_systemtm_signal_parameter.status2);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Egse_systemTm_channel);
		sprintf(simtmp, "%d", now.Egse_systemtm_signal_parameter.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Egse_systemtm_signal_parameter.status1); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Egse_systemtm_signal_parameter.status2); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Egse_systemtm_channel_used = 1(0, 5, 9) */
		reached[2][5] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = ((int)Egse_systemtm_channel_used);
		Egse_systemtm_channel_used = 1;
#ifdef VAR_RANGES
		logval("Egse_systemtm_channel_used", ((int)Egse_systemtm_channel_used));
#endif
		;
		/* merge: goto Egse_systemtm_loop(0, 6, 9) */
		reached[2][6] = 1;
		;
		_m = 4; goto P999; /* 2 */
	case 556: // STATE 7 - system.pml:151 - [(empty(Egse_systemTm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][7] = 1;
		if (!((q_len(now.Egse_systemTm_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 557: // STATE 8 - system.pml:152 - [(1)] (12:0:0 - 1)
		IfNotBlocked
		reached[2][8] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 10, 12) */
		reached[2][10] = 1;
		;
		/* merge: .(goto)(0, 13, 12) */
		reached[2][13] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 558: // STATE 15 - system.pml:156 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[2][15] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Actuator2_actuatorTc */
	case 559: // STATE 1 - system.pml:120 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[1][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 560: // STATE 2 - system.pml:123 - [(nempty(Actuator2_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][2] = 1;
		if (!((q_len(now.Actuator2_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 561: // STATE 3 - system.pml:124 - [Actuator2_lock?_] (0:0:1 - 1)
		reached[1][3] = 1;
		if (q_len(now.Actuator2_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Actuator2_lock, XX-1, 0, 0);
		;
		qrecv(now.Actuator2_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator2_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 562: // STATE 4 - system.pml:127 - [(nempty(Actuator2_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][4] = 1;
		if (!((q_len(now.Actuator2_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 563: // STATE 5 - system.pml:128 - [Actuator2_actuatorTc_channel?Actuator2_actuatortc_signal_parameter] (215:0:2 - 1)
		reached[1][5] = 1;
		if (q_len(now.Actuator2_actuatorTc_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Actuator2_actuatortc_signal_parameter;
		;
		now.Actuator2_actuatortc_signal_parameter = qrecv(now.Actuator2_actuatorTc_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatortc_signal_parameter", now.Actuator2_actuatortc_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator2_actuatorTc_channel);
		sprintf(simtmp, "%d", now.Actuator2_actuatortc_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Actuator2_actuatortc_channel_used = 1(0, 6, 215) */
		reached[1][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Actuator2_actuatortc_channel_used);
		Actuator2_actuatortc_channel_used = 1;
#ifdef VAR_RANGES
		logval("Actuator2_actuatortc_channel_used", ((int)Actuator2_actuatortc_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 564: // STATE 7 - actuator2.pml:66 - [((global_state.actuator2.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][7] = 1;
		if (!((now.global_state.actuator2.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 565: // STATE 8 - dataview.pml:8592 - [global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[1][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator2.cmd;
		now.global_state.actuator2.cmd = now.Actuator2_actuatortc_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator2.cmd", now.global_state.actuator2.cmd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 566: // STATE 9 - dataview.pml:8597 - [assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator2.cmd==0)||(now.global_state.actuator2.cmd==1))||(now.global_state.actuator2.cmd==2)), "(((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 567: // STATE 12 - actuator2.pml:4 - [transition_id = 0] (0:71:2 - 1)
		IfNotBlocked
		reached[1][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: transition_id = 1(71, 13, 71) */
		reached[1][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: .(goto)(0, 72, 71) */
		reached[1][72] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 568: // STATE 14 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][14] = 1;
		if (!((((P1 *)_this)->_1053_2_2_8_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 569: // STATE 16 - actuator2.pml:9 - [((transition_id==0))] (70:0:3 - 1)
		IfNotBlocked
		reached[1][16] = 1;
		if (!((((P1 *)_this)->_1053_2_2_8_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_8_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_8_transition_id = 0;
		/* merge: transition_id = -(1)(70, 17, 70) */
		reached[1][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(70, 18, 70) */
		reached[1][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 70) */
		reached[1][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 570: // STATE 20 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][20] = 1;
		if (!((((P1 *)_this)->_1053_2_2_8_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 571: // STATE 23 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][23] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 572: // STATE 24 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (70:0:2 - 1)
		IfNotBlocked
		reached[1][24] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 26, 70) */
		reached[1][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(70, 27, 70) */
		reached[1][27] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 28, 70) */
		reached[1][28] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 573: // STATE 29 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][29] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 574: // STATE 30 - system.pml:77 - [Modemanager_actuatorTm2_channel!1] (70:0:2 - 1)
		IfNotBlocked
		reached[1][30] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 32, 70) */
		reached[1][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 1(70, 33, 70) */
		reached[1][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 34, 70) */
		reached[1][34] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 575: // STATE 36 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 576: // STATE 40 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][40] = 1;
		if (!((((P1 *)_this)->_1053_2_2_8_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 577: // STATE 43 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][43] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 578: // STATE 44 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (70:0:2 - 1)
		IfNotBlocked
		reached[1][44] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 46, 70) */
		reached[1][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(70, 47, 70) */
		reached[1][47] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 48, 70) */
		reached[1][48] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 579: // STATE 50 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 580: // STATE 54 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][54] = 1;
		if (!((((P1 *)_this)->_1053_2_2_8_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 581: // STATE 57 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][57] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 582: // STATE 58 - system.pml:77 - [Modemanager_actuatorTm2_channel!0] (70:0:2 - 1)
		IfNotBlocked
		reached[1][58] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 60, 70) */
		reached[1][60] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 0(70, 61, 70) */
		reached[1][61] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 62, 70) */
		reached[1][62] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 583: // STATE 64 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_8_transition_id;
		((P1 *)_this)->_1053_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_8_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 584: // STATE 75 - actuator2.pml:69 - [((global_state.actuator2.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][75] = 1;
		if (!((now.global_state.actuator2.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 585: // STATE 76 - dataview.pml:8592 - [global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[1][76] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator2.cmd;
		now.global_state.actuator2.cmd = now.Actuator2_actuatortc_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator2.cmd", now.global_state.actuator2.cmd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 586: // STATE 77 - dataview.pml:8597 - [assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][77] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator2.cmd==0)||(now.global_state.actuator2.cmd==1))||(now.global_state.actuator2.cmd==2)), "(((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 587: // STATE 80 - actuator2.pml:4 - [transition_id = 0] (0:139:2 - 1)
		IfNotBlocked
		reached[1][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: transition_id = 2(139, 81, 139) */
		reached[1][81] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id = 2;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: .(goto)(0, 140, 139) */
		reached[1][140] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 588: // STATE 82 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][82] = 1;
		if (!((((P1 *)_this)->_1053_2_2_10_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 589: // STATE 84 - actuator2.pml:9 - [((transition_id==0))] (138:0:3 - 1)
		IfNotBlocked
		reached[1][84] = 1;
		if (!((((P1 *)_this)->_1053_2_2_10_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_10_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_10_transition_id = 0;
		/* merge: transition_id = -(1)(138, 85, 138) */
		reached[1][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(138, 86, 138) */
		reached[1][86] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 87, 138) */
		reached[1][87] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 590: // STATE 88 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][88] = 1;
		if (!((((P1 *)_this)->_1053_2_2_10_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 591: // STATE 91 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][91] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 592: // STATE 92 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (138:0:2 - 1)
		IfNotBlocked
		reached[1][92] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 94, 138) */
		reached[1][94] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(138, 95, 138) */
		reached[1][95] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 96, 138) */
		reached[1][96] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 593: // STATE 97 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][97] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 594: // STATE 98 - system.pml:77 - [Modemanager_actuatorTm2_channel!1] (138:0:2 - 1)
		IfNotBlocked
		reached[1][98] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 100, 138) */
		reached[1][100] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 1(138, 101, 138) */
		reached[1][101] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 102, 138) */
		reached[1][102] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 595: // STATE 104 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][104] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 596: // STATE 108 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][108] = 1;
		if (!((((P1 *)_this)->_1053_2_2_10_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 597: // STATE 111 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][111] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 598: // STATE 112 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (138:0:2 - 1)
		IfNotBlocked
		reached[1][112] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 114, 138) */
		reached[1][114] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(138, 115, 138) */
		reached[1][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 138) */
		reached[1][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 599: // STATE 118 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][118] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 600: // STATE 122 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][122] = 1;
		if (!((((P1 *)_this)->_1053_2_2_10_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 601: // STATE 125 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][125] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 602: // STATE 126 - system.pml:77 - [Modemanager_actuatorTm2_channel!0] (138:0:2 - 1)
		IfNotBlocked
		reached[1][126] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 128, 138) */
		reached[1][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 0(138, 129, 138) */
		reached[1][129] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 130, 138) */
		reached[1][130] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 603: // STATE 132 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][132] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_10_transition_id;
		((P1 *)_this)->_1053_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_10_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 604: // STATE 143 - actuator2.pml:72 - [((global_state.actuator2.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][143] = 1;
		if (!((now.global_state.actuator2.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 605: // STATE 144 - dataview.pml:8592 - [global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[1][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator2.cmd;
		now.global_state.actuator2.cmd = now.Actuator2_actuatortc_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator2.cmd", now.global_state.actuator2.cmd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 606: // STATE 145 - dataview.pml:8597 - [assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][145] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator2.cmd==0)||(now.global_state.actuator2.cmd==1))||(now.global_state.actuator2.cmd==2)), "(((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 607: // STATE 148 - actuator2.pml:4 - [transition_id = 0] (0:207:2 - 1)
		IfNotBlocked
		reached[1][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: transition_id = 3(207, 149, 207) */
		reached[1][149] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id = 3;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: .(goto)(0, 208, 207) */
		reached[1][208] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 608: // STATE 150 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][150] = 1;
		if (!((((P1 *)_this)->_1053_2_2_12_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 609: // STATE 152 - actuator2.pml:9 - [((transition_id==0))] (206:0:3 - 1)
		IfNotBlocked
		reached[1][152] = 1;
		if (!((((P1 *)_this)->_1053_2_2_12_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_12_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_12_transition_id = 0;
		/* merge: transition_id = -(1)(206, 153, 206) */
		reached[1][153] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(206, 154, 206) */
		reached[1][154] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 155, 206) */
		reached[1][155] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 610: // STATE 156 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][156] = 1;
		if (!((((P1 *)_this)->_1053_2_2_12_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 611: // STATE 159 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][159] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 612: // STATE 160 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (206:0:2 - 1)
		IfNotBlocked
		reached[1][160] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 162, 206) */
		reached[1][162] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(206, 163, 206) */
		reached[1][163] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 164, 206) */
		reached[1][164] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 613: // STATE 165 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][165] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 614: // STATE 166 - system.pml:77 - [Modemanager_actuatorTm2_channel!1] (206:0:2 - 1)
		IfNotBlocked
		reached[1][166] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 168, 206) */
		reached[1][168] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 1(206, 169, 206) */
		reached[1][169] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 170, 206) */
		reached[1][170] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 615: // STATE 172 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][172] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 616: // STATE 176 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][176] = 1;
		if (!((((P1 *)_this)->_1053_2_2_12_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 617: // STATE 179 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][179] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 618: // STATE 180 - system.pml:77 - [Modemanager_actuatorTm2_channel!2] (206:0:2 - 1)
		IfNotBlocked
		reached[1][180] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 182, 206) */
		reached[1][182] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 2(206, 183, 206) */
		reached[1][183] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 184, 206) */
		reached[1][184] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 619: // STATE 186 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 620: // STATE 190 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][190] = 1;
		if (!((((P1 *)_this)->_1053_2_2_12_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1053_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1053_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 621: // STATE 193 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][193] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 622: // STATE 194 - system.pml:77 - [Modemanager_actuatorTm2_channel!0] (206:0:2 - 1)
		IfNotBlocked
		reached[1][194] = 1;
		if (q_full(now.Modemanager_actuatorTm2_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm2_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm2_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 196, 206) */
		reached[1][196] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		/* merge: global_state.actuator2.state = 0(206, 197, 206) */
		reached[1][197] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator2.state;
		now.global_state.actuator2.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator2.state", now.global_state.actuator2.state);
#endif
		;
		/* merge: goto continuous_signals(0, 198, 206) */
		reached[1][198] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 623: // STATE 200 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][200] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1053_2_2_12_transition_id;
		((P1 *)_this)->_1053_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1053_2_2_12_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 624: // STATE 217 - system.pml:132 - [(empty(Actuator2_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][217] = 1;
		if (!((q_len(now.Actuator2_actuatorTc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 625: // STATE 218 - system.pml:133 - [(1)] (221:0:0 - 1)
		IfNotBlocked
		reached[1][218] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 220, 221) */
		reached[1][220] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 626: // STATE 221 - system.pml:135 - [Actuator2_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[1][221] = 1;
		if (q_full(now.Actuator2_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator2_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator2_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 627: // STATE 226 - system.pml:138 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[1][226] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Actuator1_actuatorTc */
	case 628: // STATE 1 - system.pml:99 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[0][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 629: // STATE 2 - system.pml:102 - [(nempty(Actuator1_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][2] = 1;
		if (!((q_len(now.Actuator1_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 630: // STATE 3 - system.pml:103 - [Actuator1_lock?_] (0:0:1 - 1)
		reached[0][3] = 1;
		if (q_len(now.Actuator1_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Actuator1_lock, XX-1, 0, 0);
		;
		qrecv(now.Actuator1_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator1_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 631: // STATE 4 - system.pml:106 - [(nempty(Actuator1_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][4] = 1;
		if (!((q_len(now.Actuator1_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 632: // STATE 5 - system.pml:107 - [Actuator1_actuatorTc_channel?Actuator1_actuatortc_signal_parameter] (215:0:2 - 1)
		reached[0][5] = 1;
		if (q_len(now.Actuator1_actuatorTc_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Actuator1_actuatortc_signal_parameter;
		;
		now.Actuator1_actuatortc_signal_parameter = qrecv(now.Actuator1_actuatorTc_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatortc_signal_parameter", now.Actuator1_actuatortc_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator1_actuatorTc_channel);
		sprintf(simtmp, "%d", now.Actuator1_actuatortc_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Actuator1_actuatortc_channel_used = 1(0, 6, 215) */
		reached[0][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Actuator1_actuatortc_channel_used);
		Actuator1_actuatortc_channel_used = 1;
#ifdef VAR_RANGES
		logval("Actuator1_actuatortc_channel_used", ((int)Actuator1_actuatortc_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 633: // STATE 7 - actuator1.pml:66 - [((global_state.actuator1.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][7] = 1;
		if (!((now.global_state.actuator1.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 634: // STATE 8 - dataview.pml:8592 - [global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[0][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator1.cmd;
		now.global_state.actuator1.cmd = now.Actuator1_actuatortc_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator1.cmd", now.global_state.actuator1.cmd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 635: // STATE 9 - dataview.pml:8597 - [assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator1.cmd==0)||(now.global_state.actuator1.cmd==1))||(now.global_state.actuator1.cmd==2)), "(((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 636: // STATE 12 - actuator1.pml:4 - [transition_id = 0] (0:71:2 - 1)
		IfNotBlocked
		reached[0][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: transition_id = 1(71, 13, 71) */
		reached[0][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: .(goto)(0, 72, 71) */
		reached[0][72] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 637: // STATE 14 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][14] = 1;
		if (!((((P0 *)_this)->_1052_1_1_2_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 638: // STATE 16 - actuator1.pml:9 - [((transition_id==0))] (70:0:3 - 1)
		IfNotBlocked
		reached[0][16] = 1;
		if (!((((P0 *)_this)->_1052_1_1_2_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_2_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_2_transition_id = 0;
		/* merge: transition_id = -(1)(70, 17, 70) */
		reached[0][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(70, 18, 70) */
		reached[0][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 70) */
		reached[0][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 639: // STATE 20 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][20] = 1;
		if (!((((P0 *)_this)->_1052_1_1_2_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 640: // STATE 23 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][23] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 641: // STATE 24 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (70:0:2 - 1)
		IfNotBlocked
		reached[0][24] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 26, 70) */
		reached[0][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(70, 27, 70) */
		reached[0][27] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 28, 70) */
		reached[0][28] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 642: // STATE 29 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][29] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 643: // STATE 30 - system.pml:73 - [Modemanager_actuatorTm1_channel!1] (70:0:2 - 1)
		IfNotBlocked
		reached[0][30] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 32, 70) */
		reached[0][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 1(70, 33, 70) */
		reached[0][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 34, 70) */
		reached[0][34] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 644: // STATE 36 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 645: // STATE 40 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][40] = 1;
		if (!((((P0 *)_this)->_1052_1_1_2_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 646: // STATE 43 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][43] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 647: // STATE 44 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (70:0:2 - 1)
		IfNotBlocked
		reached[0][44] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 46, 70) */
		reached[0][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(70, 47, 70) */
		reached[0][47] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 48, 70) */
		reached[0][48] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 648: // STATE 50 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 649: // STATE 54 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][54] = 1;
		if (!((((P0 *)_this)->_1052_1_1_2_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 650: // STATE 57 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][57] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 651: // STATE 58 - system.pml:73 - [Modemanager_actuatorTm1_channel!0] (70:0:2 - 1)
		IfNotBlocked
		reached[0][58] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(70, 60, 70) */
		reached[0][60] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 0(70, 61, 70) */
		reached[0][61] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 62, 70) */
		reached[0][62] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 652: // STATE 64 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_2_transition_id;
		((P0 *)_this)->_1052_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_2_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 653: // STATE 75 - actuator1.pml:69 - [((global_state.actuator1.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][75] = 1;
		if (!((now.global_state.actuator1.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 654: // STATE 76 - dataview.pml:8592 - [global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[0][76] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator1.cmd;
		now.global_state.actuator1.cmd = now.Actuator1_actuatortc_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator1.cmd", now.global_state.actuator1.cmd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 655: // STATE 77 - dataview.pml:8597 - [assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][77] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator1.cmd==0)||(now.global_state.actuator1.cmd==1))||(now.global_state.actuator1.cmd==2)), "(((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 656: // STATE 80 - actuator1.pml:4 - [transition_id = 0] (0:139:2 - 1)
		IfNotBlocked
		reached[0][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: transition_id = 2(139, 81, 139) */
		reached[0][81] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id = 2;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: .(goto)(0, 140, 139) */
		reached[0][140] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 657: // STATE 82 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][82] = 1;
		if (!((((P0 *)_this)->_1052_1_1_4_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 658: // STATE 84 - actuator1.pml:9 - [((transition_id==0))] (138:0:3 - 1)
		IfNotBlocked
		reached[0][84] = 1;
		if (!((((P0 *)_this)->_1052_1_1_4_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_4_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_4_transition_id = 0;
		/* merge: transition_id = -(1)(138, 85, 138) */
		reached[0][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(138, 86, 138) */
		reached[0][86] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 87, 138) */
		reached[0][87] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 659: // STATE 88 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][88] = 1;
		if (!((((P0 *)_this)->_1052_1_1_4_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 660: // STATE 91 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][91] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 661: // STATE 92 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (138:0:2 - 1)
		IfNotBlocked
		reached[0][92] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 94, 138) */
		reached[0][94] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(138, 95, 138) */
		reached[0][95] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 96, 138) */
		reached[0][96] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 662: // STATE 97 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][97] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 663: // STATE 98 - system.pml:73 - [Modemanager_actuatorTm1_channel!1] (138:0:2 - 1)
		IfNotBlocked
		reached[0][98] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 100, 138) */
		reached[0][100] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 1(138, 101, 138) */
		reached[0][101] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 102, 138) */
		reached[0][102] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 664: // STATE 104 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][104] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 665: // STATE 108 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][108] = 1;
		if (!((((P0 *)_this)->_1052_1_1_4_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 666: // STATE 111 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][111] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 667: // STATE 112 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (138:0:2 - 1)
		IfNotBlocked
		reached[0][112] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 114, 138) */
		reached[0][114] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(138, 115, 138) */
		reached[0][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 138) */
		reached[0][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 668: // STATE 118 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][118] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 669: // STATE 122 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][122] = 1;
		if (!((((P0 *)_this)->_1052_1_1_4_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 670: // STATE 125 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][125] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 671: // STATE 126 - system.pml:73 - [Modemanager_actuatorTm1_channel!0] (138:0:2 - 1)
		IfNotBlocked
		reached[0][126] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(138, 128, 138) */
		reached[0][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 0(138, 129, 138) */
		reached[0][129] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 130, 138) */
		reached[0][130] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 672: // STATE 132 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][132] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_4_transition_id;
		((P0 *)_this)->_1052_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_4_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 673: // STATE 143 - actuator1.pml:72 - [((global_state.actuator1.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][143] = 1;
		if (!((now.global_state.actuator1.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 674: // STATE 144 - dataview.pml:8592 - [global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[0][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator1.cmd;
		now.global_state.actuator1.cmd = now.Actuator1_actuatortc_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator1.cmd", now.global_state.actuator1.cmd);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 675: // STATE 145 - dataview.pml:8597 - [assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][145] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator1.cmd==0)||(now.global_state.actuator1.cmd==1))||(now.global_state.actuator1.cmd==2)), "(((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 676: // STATE 148 - actuator1.pml:4 - [transition_id = 0] (0:207:2 - 1)
		IfNotBlocked
		reached[0][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: transition_id = 3(207, 149, 207) */
		reached[0][149] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id = 3;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: .(goto)(0, 208, 207) */
		reached[0][208] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 677: // STATE 150 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][150] = 1;
		if (!((((P0 *)_this)->_1052_1_1_6_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 678: // STATE 152 - actuator1.pml:9 - [((transition_id==0))] (206:0:3 - 1)
		IfNotBlocked
		reached[0][152] = 1;
		if (!((((P0 *)_this)->_1052_1_1_6_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_6_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_6_transition_id = 0;
		/* merge: transition_id = -(1)(206, 153, 206) */
		reached[0][153] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(206, 154, 206) */
		reached[0][154] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 155, 206) */
		reached[0][155] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 679: // STATE 156 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][156] = 1;
		if (!((((P0 *)_this)->_1052_1_1_6_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 680: // STATE 159 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][159] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 681: // STATE 160 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (206:0:2 - 1)
		IfNotBlocked
		reached[0][160] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 162, 206) */
		reached[0][162] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(206, 163, 206) */
		reached[0][163] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 164, 206) */
		reached[0][164] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 682: // STATE 165 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][165] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 683: // STATE 166 - system.pml:73 - [Modemanager_actuatorTm1_channel!1] (206:0:2 - 1)
		IfNotBlocked
		reached[0][166] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 1, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 168, 206) */
		reached[0][168] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 1(206, 169, 206) */
		reached[0][169] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 170, 206) */
		reached[0][170] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 684: // STATE 172 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][172] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 685: // STATE 176 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][176] = 1;
		if (!((((P0 *)_this)->_1052_1_1_6_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 686: // STATE 179 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][179] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 687: // STATE 180 - system.pml:73 - [Modemanager_actuatorTm1_channel!2] (206:0:2 - 1)
		IfNotBlocked
		reached[0][180] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 2); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 2, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 182, 206) */
		reached[0][182] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 2(206, 183, 206) */
		reached[0][183] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 2;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 184, 206) */
		reached[0][184] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 688: // STATE 186 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 689: // STATE 190 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][190] = 1;
		if (!((((P0 *)_this)->_1052_1_1_6_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1052_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1052_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 690: // STATE 193 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][193] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 691: // STATE 194 - system.pml:73 - [Modemanager_actuatorTm1_channel!0] (206:0:2 - 1)
		IfNotBlocked
		reached[0][194] = 1;
		if (q_full(now.Modemanager_actuatorTm1_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_actuatorTm1_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_actuatorTm1_channel, 0, 0, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(206, 196, 206) */
		reached[0][196] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		/* merge: global_state.actuator1.state = 0(206, 197, 206) */
		reached[0][197] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator1.state;
		now.global_state.actuator1.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator1.state", now.global_state.actuator1.state);
#endif
		;
		/* merge: goto continuous_signals(0, 198, 206) */
		reached[0][198] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 692: // STATE 200 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][200] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1052_1_1_6_transition_id;
		((P0 *)_this)->_1052_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1052_1_1_6_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 693: // STATE 217 - system.pml:111 - [(empty(Actuator1_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][217] = 1;
		if (!((q_len(now.Actuator1_actuatorTc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 694: // STATE 218 - system.pml:112 - [(1)] (221:0:0 - 1)
		IfNotBlocked
		reached[0][218] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 220, 221) */
		reached[0][220] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 695: // STATE 221 - system.pml:114 - [Actuator1_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[0][221] = 1;
		if (q_full(now.Actuator1_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator1_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator1_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 696: // STATE 226 - system.pml:117 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[0][226] = 1;
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

