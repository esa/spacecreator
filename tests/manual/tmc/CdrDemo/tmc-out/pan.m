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

		 /* CLAIM never_0 */
	case 3: // STATE 1 - scl.pml:4 - [(inited)] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported1 = 0;
			if (verbose && !reported1)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported1 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported1 = 0;
			if (verbose && !reported1)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported1 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[10][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 4: // STATE 8 - scl.pml:12 - [((global_state.untitled_msc.state==3))] (16:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported8 = 0;
			if (verbose && !reported8)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported8 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported8 = 0;
			if (verbose && !reported8)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported8 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[10][8] = 1;
		if (!((now.global_state.untitled_msc.state==3)))
			continue;
		/* merge: printf('Observer entered errorstate: untitled_msc\\n')(16, 9, 16) */
		reached[10][9] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("Observer entered errorstate: untitled_msc\n");
		/* merge: assert(!((global_state.untitled_msc.state==3)))(16, 10, 16) */
		reached[10][10] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert( !((now.global_state.untitled_msc.state==3)), " !((global_state.untitled_msc.state==3))", II, tt, t);
		/* merge: .(goto)(0, 17, 16) */
		reached[10][17] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 5: // STATE 12 - scl.pml:16 - [((global_state.untitled_msc.state==1))] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported12 = 0;
			if (verbose && !reported12)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported12 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported12 = 0;
			if (verbose && !reported12)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported12 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[10][12] = 1;
		if (!((now.global_state.untitled_msc.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 6: // STATE 19 - scl.pml:24 - [((global_state.untitled_msc.state==3))] (25:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported19 = 0;
			if (verbose && !reported19)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported19 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported19 = 0;
			if (verbose && !reported19)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported19 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[10][19] = 1;
		if (!((now.global_state.untitled_msc.state==3)))
			continue;
		/* merge: printf('Observer entered errorstate: untitled_msc\\n')(25, 20, 25) */
		reached[10][20] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("Observer entered errorstate: untitled_msc\n");
		/* merge: assert(!((global_state.untitled_msc.state==3)))(25, 21, 25) */
		reached[10][21] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert( !((now.global_state.untitled_msc.state==3)), " !((global_state.untitled_msc.state==3))", II, tt, t);
		/* merge: .(goto)(0, 26, 25) */
		reached[10][26] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 7: // STATE 28 - scl.pml:31 - [-end-] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported28 = 0;
			if (verbose && !reported28)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported28 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported28 = 0;
			if (verbose && !reported28)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported28 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[10][28] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC :init: */
	case 8: // STATE 4 - dataview.pml:20551 - [D_STEP20551]
		IfNotBlocked

		reached[9][4] = 1;
		reached[9][t->st] = 1;
		reached[9][tt] = 1;

		if (TstOnly) return 1;


		reached[9][4] = 1;
		reached[9][t->st] = 1;
		reached[9][tt] = 1;

		sv_save();
		S_1998_0: /* 2 */
S_1997_0: /* 2 */
S_1996_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.nothing.selection = 1;
#ifdef VAR_RANGES
		logval("nothing.selection", now.nothing.selection);
#endif
		;
		goto S_2068_0;
S_2068_0: /* 1 */

#if defined(C_States) && (HAS_TRACK==1)
		c_update((uchar *) &(now.c_state[0]));
#endif
		_m = 3; goto P999;

	case 9: // STATE 6 - dataview.pml:9173 - [global_state.actuator1.sender = 6] (0:0:1 - 1)
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
	case 10: // STATE 7 - dataview.pml:9178 - [assert((((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][7] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.actuator1.sender==0)||(now.global_state.actuator1.sender==1))||(now.global_state.actuator1.sender==2))||(now.global_state.actuator1.sender==3))||(now.global_state.actuator1.sender==4))||(now.global_state.actuator1.sender==5))||(now.global_state.actuator1.sender==6)), "(((((((global_state.actuator1.sender==0)||(global_state.actuator1.sender==1))||(global_state.actuator1.sender==2))||(global_state.actuator1.sender==3))||(global_state.actuator1.sender==4))||(global_state.actuator1.sender==5))||(global_state.actuator1.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 11: // STATE 10 - actuator1.pml:4 - [transition_id = 0] (0:69:2 - 1)
		IfNotBlocked
		reached[9][10] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
#endif
		;
		/* merge: transition_id = 0(69, 11, 69) */
		reached[9][11] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
#endif
		;
		/* merge: .(goto)(0, 70, 69) */
		reached[9][70] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 12: // STATE 12 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][12] = 1;
		if (!((((P9 *)_this)->_1067_10_14_41_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_14_41_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_14_41_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 13: // STATE 14 - actuator1.pml:9 - [((transition_id==0))] (68:0:3 - 1)
		IfNotBlocked
		reached[9][14] = 1;
		if (!((((P9 *)_this)->_1067_10_14_41_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_14_41_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_14_41_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_14_41_transition_id = 0;
		/* merge: transition_id = -(1)(68, 15, 68) */
		reached[9][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
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
	case 14: // STATE 18 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][18] = 1;
		if (!((((P9 *)_this)->_1067_10_14_41_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_14_41_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_14_41_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 15: // STATE 21 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][21] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 16: // STATE 22 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (68:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
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
	case 17: // STATE 27 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][27] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 18: // STATE 28 - system.pml:76 - [Modemanager_actuatorTm1_channel!1] (68:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
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
	case 19: // STATE 34 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][34] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 20: // STATE 38 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][38] = 1;
		if (!((((P9 *)_this)->_1067_10_14_41_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_14_41_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_14_41_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 21: // STATE 41 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][41] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 22: // STATE 42 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (68:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
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
	case 23: // STATE 48 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 24: // STATE 52 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][52] = 1;
		if (!((((P9 *)_this)->_1067_10_14_41_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_14_41_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_14_41_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 25: // STATE 55 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][55] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 26: // STATE 56 - system.pml:76 - [Modemanager_actuatorTm1_channel!0] (68:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
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
	case 27: // STATE 62 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_14_41_transition_id;
		((P9 *)_this)->_1067_10_14_41_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_14_41_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 28: // STATE 74 - system.pml:277 - [Actuator1_lock!1] (0:0:0 - 3)
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
	case 29: // STATE 75 - dataview.pml:9173 - [global_state.actuator2.sender = 6] (0:0:1 - 1)
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
	case 30: // STATE 76 - dataview.pml:9178 - [assert((((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][76] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.actuator2.sender==0)||(now.global_state.actuator2.sender==1))||(now.global_state.actuator2.sender==2))||(now.global_state.actuator2.sender==3))||(now.global_state.actuator2.sender==4))||(now.global_state.actuator2.sender==5))||(now.global_state.actuator2.sender==6)), "(((((((global_state.actuator2.sender==0)||(global_state.actuator2.sender==1))||(global_state.actuator2.sender==2))||(global_state.actuator2.sender==3))||(global_state.actuator2.sender==4))||(global_state.actuator2.sender==5))||(global_state.actuator2.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 31: // STATE 79 - actuator2.pml:4 - [transition_id = 0] (0:138:2 - 1)
		IfNotBlocked
		reached[9][79] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
#endif
		;
		/* merge: transition_id = 0(138, 80, 138) */
		reached[9][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
#endif
		;
		/* merge: .(goto)(0, 139, 138) */
		reached[9][139] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 32: // STATE 81 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][81] = 1;
		if (!((((P9 *)_this)->_1067_10_15_43_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_15_43_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_15_43_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 33: // STATE 83 - actuator2.pml:9 - [((transition_id==0))] (137:0:3 - 1)
		IfNotBlocked
		reached[9][83] = 1;
		if (!((((P9 *)_this)->_1067_10_15_43_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_15_43_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_15_43_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_15_43_transition_id = 0;
		/* merge: transition_id = -(1)(137, 84, 137) */
		reached[9][84] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
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
	case 34: // STATE 87 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][87] = 1;
		if (!((((P9 *)_this)->_1067_10_15_43_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_15_43_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_15_43_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 35: // STATE 90 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][90] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 36: // STATE 91 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (137:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
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
	case 37: // STATE 96 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][96] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 38: // STATE 97 - system.pml:80 - [Modemanager_actuatorTm2_channel!1] (137:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
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
	case 39: // STATE 103 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][103] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 40: // STATE 107 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][107] = 1;
		if (!((((P9 *)_this)->_1067_10_15_43_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_15_43_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_15_43_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 41: // STATE 110 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][110] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 42: // STATE 111 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (137:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
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
	case 43: // STATE 117 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][117] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 44: // STATE 121 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][121] = 1;
		if (!((((P9 *)_this)->_1067_10_15_43_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_15_43_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_15_43_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 45: // STATE 124 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][124] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 46: // STATE 125 - system.pml:80 - [Modemanager_actuatorTm2_channel!0] (137:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
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
	case 47: // STATE 131 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][131] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_15_43_transition_id;
		((P9 *)_this)->_1067_10_15_43_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_15_43_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 48: // STATE 143 - system.pml:279 - [Actuator2_lock!1] (0:0:0 - 3)
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
	case 49: // STATE 144 - dataview.pml:9173 - [global_state.modemanager.sender = 6] (0:0:1 - 1)
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
	case 50: // STATE 145 - dataview.pml:9178 - [assert((((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][145] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.modemanager.sender==0)||(now.global_state.modemanager.sender==1))||(now.global_state.modemanager.sender==2))||(now.global_state.modemanager.sender==3))||(now.global_state.modemanager.sender==4))||(now.global_state.modemanager.sender==5))||(now.global_state.modemanager.sender==6)), "(((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))||(global_state.modemanager.sender==5))||(global_state.modemanager.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 51: // STATE 148 - modemanager.pml:4 - [transition_id = 0] (0:288:2 - 1)
		IfNotBlocked
		reached[9][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: transition_id = 0(288, 149, 288) */
		reached[9][149] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: .(goto)(0, 289, 288) */
		reached[9][289] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 52: // STATE 150 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][150] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 53: // STATE 152 - modemanager.pml:9 - [((transition_id==0))] (287:0:3 - 1)
		IfNotBlocked
		reached[9][152] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		/* merge: transition_id = -(1)(287, 153, 287) */
		reached[9][153] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
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
	case 54: // STATE 156 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][156] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 55: // STATE 159 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][159] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 56: // STATE 160 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 57: // STATE 161 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][161] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 58: // STATE 164 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 59: // STATE 165 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][165] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 60: // STATE 168 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 61: // STATE 170 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (287:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
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
	case 62: // STATE 175 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][175] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 63: // STATE 176 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 64: // STATE 177 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][177] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 65: // STATE 180 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 66: // STATE 181 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][181] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 67: // STATE 184 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 68: // STATE 185 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][185] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 69: // STATE 189 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (287:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 192, 287) */
		reached[9][192] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 70: // STATE 194 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][194] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 71: // STATE 198 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][198] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 72: // STATE 201 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][201] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 73: // STATE 202 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 74: // STATE 204 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (287:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
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
	case 75: // STATE 209 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][209] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 76: // STATE 210 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 77: // STATE 211 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][211] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 78: // STATE 214 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 79: // STATE 215 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][215] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 80: // STATE 218 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 81: // STATE 219 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][219] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 82: // STATE 223 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (287:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 226, 287) */
		reached[9][226] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 83: // STATE 228 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[9][228] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 84: // STATE 232 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][232] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 85: // STATE 235 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][235] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 86: // STATE 236 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 87: // STATE 239 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 88: // STATE 243 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][243] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 89: // STATE 244 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (287:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 252, 287) */
		reached[9][252] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 90: // STATE 247 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (287:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 252, 287) */
		reached[9][252] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 91: // STATE 251 - modemanager.pml:73 - [transition_id = -(1)] (0:287:1 - 3)
		IfNotBlocked
		reached[9][251] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 252, 287) */
		reached[9][252] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 92: // STATE 253 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][253] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 93: // STATE 257 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][257] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 94: // STATE 260 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (287:0:1 - 1)
		IfNotBlocked
		reached[9][260] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 261, 287) */
		reached[9][261] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 262, 287) */
		reached[9][262] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 95: // STATE 263 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (287:0:2 - 1)
		IfNotBlocked
		reached[9][263] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(287, 264, 287) */
		reached[9][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
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
	case 96: // STATE 269 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][269] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 97: // STATE 273 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][273] = 1;
		if (!((((P9 *)_this)->_1067_10_16_45_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_16_45_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_16_45_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 98: // STATE 276 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (287:0:1 - 1)
		IfNotBlocked
		reached[9][276] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 277, 287) */
		reached[9][277] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 278, 287) */
		reached[9][278] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 99: // STATE 279 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (287:0:2 - 1)
		IfNotBlocked
		reached[9][279] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(287, 280, 287) */
		reached[9][280] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_16_45_transition_id;
		((P9 *)_this)->_1067_10_16_45_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_16_45_transition_id);
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
	case 100: // STATE 293 - system.pml:281 - [Modemanager_lock!1] (0:0:0 - 3)
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
		_m = 2; goto P999; /* 0 */
	case 101: // STATE 294 - dataview.pml:9173 - [global_state.untitled_msc.sender = 6] (0:0:1 - 1)
		IfNotBlocked
		reached[9][294] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.untitled_msc.sender;
		now.global_state.untitled_msc.sender = 6;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.sender", now.global_state.untitled_msc.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 102: // STATE 295 - dataview.pml:9178 - [assert((((((((global_state.untitled_msc.sender==0)||(global_state.untitled_msc.sender==1))||(global_state.untitled_msc.sender==2))||(global_state.untitled_msc.sender==3))||(global_state.untitled_msc.sender==4))||(global_state.untitled_msc.sender==5))||(global_state.untitled_msc.sender==6)))] (0:0:0 - 1)
		IfNotBlocked
		reached[9][295] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((((now.global_state.untitled_msc.sender==0)||(now.global_state.untitled_msc.sender==1))||(now.global_state.untitled_msc.sender==2))||(now.global_state.untitled_msc.sender==3))||(now.global_state.untitled_msc.sender==4))||(now.global_state.untitled_msc.sender==5))||(now.global_state.untitled_msc.sender==6)), "(((((((global_state.untitled_msc.sender==0)||(global_state.untitled_msc.sender==1))||(global_state.untitled_msc.sender==2))||(global_state.untitled_msc.sender==3))||(global_state.untitled_msc.sender==4))||(global_state.untitled_msc.sender==5))||(global_state.untitled_msc.sender==6))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 103: // STATE 298 - Untitled_msc.pml:6 - [transition_id = 0] (0:333:2 - 1)
		IfNotBlocked
		reached[9][298] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: transition_id = 0(333, 299, 333) */
		reached[9][299] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: .(goto)(0, 334, 333) */
		reached[9][334] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 104: // STATE 300 - Untitled_msc.pml:9 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][300] = 1;
		if (!((((P9 *)_this)->_1067_10_17_47_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_17_47_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_17_47_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_17_47_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 105: // STATE 302 - Untitled_msc.pml:11 - [((transition_id==0))] (332:0:3 - 1)
		IfNotBlocked
		reached[9][302] = 1;
		if (!((((P9 *)_this)->_1067_10_17_47_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_17_47_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_17_47_transition_id = 0;
		/* merge: transition_id = -(1)(332, 303, 332) */
		reached[9][303] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(332, 304, 332) */
		reached[9][304] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 305, 332) */
		reached[9][305] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 106: // STATE 306 - Untitled_msc.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[9][306] = 1;
		if (!((((P9 *)_this)->_1067_10_17_47_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_17_47_transition_id */  (trpt+1)->bup.oval = ((P9 *)_this)->_1067_10_17_47_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_17_47_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 107: // STATE 307 - Untitled_msc.pml:17 - [((Untitled_MSC_0_sig0_param0.selection==3))] (332:0:2 - 1)
		IfNotBlocked
		reached[9][307] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==3)))
			continue;
		/* merge: transition_id = -(1)(332, 308, 332) */
		reached[9][308] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 2(332, 309, 332) */
		reached[9][309] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 2;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 310, 332) */
		reached[9][310] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 108: // STATE 312 - Untitled_msc.pml:22 - [(1)] (332:0:2 - 1)
		IfNotBlocked
		reached[9][312] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(332, 314, 332) */
		reached[9][314] = 1;
		;
		/* merge: transition_id = -(1)(332, 315, 332) */
		reached[9][315] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(332, 316, 332) */
		reached[9][316] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 317, 332) */
		reached[9][317] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 109: // STATE 315 - Untitled_msc.pml:24 - [transition_id = -(1)] (0:332:2 - 2)
		IfNotBlocked
		reached[9][315] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(332, 316, 332) */
		reached[9][316] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 317, 332) */
		reached[9][317] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 110: // STATE 318 - Untitled_msc.pml:27 - [((transition_id==2))] (332:0:3 - 1)
		IfNotBlocked
		reached[9][318] = 1;
		if (!((((P9 *)_this)->_1067_10_17_47_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_17_47_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_17_47_transition_id = 0;
		/* merge: transition_id = -(1)(332, 319, 332) */
		reached[9][319] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(332, 320, 332) */
		reached[9][320] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 321, 332) */
		reached[9][321] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 111: // STATE 322 - Untitled_msc.pml:31 - [((transition_id==3))] (332:0:3 - 1)
		IfNotBlocked
		reached[9][322] = 1;
		if (!((((P9 *)_this)->_1067_10_17_47_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_17_47_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_17_47_transition_id = 0;
		/* merge: transition_id = -(1)(332, 323, 332) */
		reached[9][323] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 1(332, 324, 332) */
		reached[9][324] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 1;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 325, 332) */
		reached[9][325] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 112: // STATE 326 - Untitled_msc.pml:35 - [((transition_id==4))] (332:0:3 - 1)
		IfNotBlocked
		reached[9][326] = 1;
		if (!((((P9 *)_this)->_1067_10_17_47_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1067_10_17_47_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P9 *)_this)->_1067_10_17_47_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P9 *)_this)->_1067_10_17_47_transition_id = 0;
		/* merge: transition_id = -(1)(332, 327, 332) */
		reached[9][327] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P9 *)_this)->_1067_10_17_47_transition_id;
		((P9 *)_this)->_1067_10_17_47_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P9 *)_this)->_1067_10_17_47_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 3(332, 328, 332) */
		reached[9][328] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 3;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 329, 332) */
		reached[9][329] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 113: // STATE 338 - system.pml:283 - [Untitled_msc_lock!1] (341:0:1 - 3)
		IfNotBlocked
		reached[9][338] = 1;
		if (q_full(now.Untitled_msc_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Untitled_msc_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Untitled_msc_lock, 0, 1, 0, 0, 0, 0, 1);
		/* merge: inited = 1(0, 339, 341) */
		reached[9][339] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.inited;
		now.inited = 1;
#ifdef VAR_RANGES
		logval("inited", now.inited);
#endif
		;
		_m = 2; goto P999; /* 1 */
	case 114: // STATE 341 - system.pml:286 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[9][341] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Sensor_reading */
	case 115: // STATE 1 - system.pml:263 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[8][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 116: // STATE 2 - system.pml:265 - [value = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[8][2] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 117: // STATE 3 - env_inlines.pml:55 - [Sensor_reading_x_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[8][3] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 118: // STATE 4 - env_inlines.pml:57 - [Sensor_reading_x_tmp = 10] (0:0:1 - 1)
		IfNotBlocked
		reached[8][4] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = 10;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 119: // STATE 7 - env_inlines.pml:57 - [goto :b34] (0:33:1 - 1)
		IfNotBlocked
		reached[8][7] = 1;
		;
		/* merge: value = Sensor_reading_x_tmp(33, 30, 33) */
		reached[8][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 120: // STATE 5 - env_inlines.pml:57 - [((Sensor_reading_x_tmp<10))] (8:0:1 - 1)
		IfNotBlocked
		reached[8][5] = 1;
		if (!((((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp<10)))
			continue;
		/* merge: Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)(0, 6, 8) */
		reached[8][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = (((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		/* merge: .(goto)(0, 9, 8) */
		reached[8][9] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 121: // STATE 12 - env_inlines.pml:60 - [Sensor_reading_x_tmp = 50] (0:0:1 - 1)
		IfNotBlocked
		reached[8][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = 50;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 122: // STATE 15 - env_inlines.pml:60 - [goto :b35] (0:33:1 - 1)
		IfNotBlocked
		reached[8][15] = 1;
		;
		/* merge: value = Sensor_reading_x_tmp(33, 30, 33) */
		reached[8][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 123: // STATE 13 - env_inlines.pml:60 - [((Sensor_reading_x_tmp<50))] (16:0:1 - 1)
		IfNotBlocked
		reached[8][13] = 1;
		if (!((((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp<50)))
			continue;
		/* merge: Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)(0, 14, 16) */
		reached[8][14] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = (((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		/* merge: .(goto)(0, 17, 16) */
		reached[8][17] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 124: // STATE 20 - env_inlines.pml:63 - [Sensor_reading_x_tmp = 100] (0:0:1 - 1)
		IfNotBlocked
		reached[8][20] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = 100;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 125: // STATE 23 - env_inlines.pml:63 - [goto :b36] (0:33:1 - 1)
		IfNotBlocked
		reached[8][23] = 1;
		;
		/* merge: value = Sensor_reading_x_tmp(33, 30, 33) */
		reached[8][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 126: // STATE 21 - env_inlines.pml:63 - [((Sensor_reading_x_tmp<100))] (24:0:1 - 1)
		IfNotBlocked
		reached[8][21] = 1;
		if (!((((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp<100)))
			continue;
		/* merge: Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)(0, 22, 24) */
		reached[8][22] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
		((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp = (((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp);
#endif
		;
		/* merge: .(goto)(0, 25, 24) */
		reached[8][25] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 127: // STATE 30 - env_inlines.pml:66 - [value = Sensor_reading_x_tmp] (0:33:1 - 7)
		IfNotBlocked
		reached[8][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P8 *)_this)->value;
		((P8 *)_this)->value = ((P8 *)_this)->_1066_9_11_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P8 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 128: // STATE 32 - system.pml:84 - [Modemanager_reading_channel!value] (0:0:0 - 1)
		IfNotBlocked
		reached[8][32] = 1;
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
		_m = 2; goto P999; /* 0 */
	case 129: // STATE 38 - system.pml:271 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[8][38] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_systemTc */
	case 130: // STATE 1 - system.pml:239 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[7][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 131: // STATE 2 - system.pml:242 - [(nempty(Modemanager_systemTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][2] = 1;
		if (!((q_len(now.Modemanager_systemTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 132: // STATE 3 - system.pml:243 - [Modemanager_lock?_] (0:0:1 - 1)
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
	case 133: // STATE 4 - system.pml:246 - [(nempty(Modemanager_systemTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][4] = 1;
		if (!((q_len(now.Modemanager_systemTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 134: // STATE 5 - system.pml:247 - [Modemanager_systemTc_channel?Modemanager_systemtc_signal_parameter.data.goToIdle,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1,Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2,Modemanager_systemtc_signal_parameter.data.doReport,Modemanager_systemtc_signal_parameter.selection] (353:0:6 - 1)
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
	case 135: // STATE 7 - modemanager.pml:153 - [((global_state.modemanager.state==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][7] = 1;
		if (!((now.global_state.modemanager.state==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 136: // STATE 8 - dataview.pml:9064 - [global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection] (0:0:1 - 1)
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
	case 137: // STATE 9 - dataview.pml:9066 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][9] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 138: // STATE 10 - dataview.pml:9023 - [global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle] (0:0:1 - 1)
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
	case 139: // STATE 11 - dataview.pml:9028 - [assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][11] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToIdle>=0)&&(now.global_state.modemanager.stc.data.goToIdle<=0)), "((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 140: // STATE 14 - dataview.pml:9068 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][14] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 141: // STATE 15 - dataview.pml:9032 - [global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 142: // STATE 16 - dataview.pml:9037 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][16] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold1<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 143: // STATE 19 - dataview.pml:9041 - [global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 144: // STATE 20 - dataview.pml:9046 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][20] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold2<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 145: // STATE 24 - dataview.pml:9070 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][24] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 146: // STATE 25 - dataview.pml:9055 - [global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport] (0:0:1 - 1)
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
	case 147: // STATE 26 - dataview.pml:9060 - [assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][26] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.doReport>=0)&&(now.global_state.modemanager.stc.data.doReport<=0)), "((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 148: // STATE 34 - modemanager.pml:4 - [transition_id = 0] (0:174:2 - 1)
		IfNotBlocked
		reached[7][34] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: transition_id = 1(174, 35, 174) */
		reached[7][35] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id = 1;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: .(goto)(0, 175, 174) */
		reached[7][175] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 149: // STATE 36 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][36] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 150: // STATE 38 - modemanager.pml:9 - [((transition_id==0))] (173:0:3 - 1)
		IfNotBlocked
		reached[7][38] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		/* merge: transition_id = -(1)(173, 39, 173) */
		reached[7][39] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
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
	case 151: // STATE 42 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][42] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 152: // STATE 45 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][45] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 153: // STATE 46 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 154: // STATE 47 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][47] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 155: // STATE 50 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 156: // STATE 51 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][51] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 157: // STATE 54 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 158: // STATE 56 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (173:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
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
	case 159: // STATE 61 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][61] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 160: // STATE 62 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 161: // STATE 63 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][63] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 162: // STATE 66 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 163: // STATE 67 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][67] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 164: // STATE 70 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 165: // STATE 71 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][71] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 166: // STATE 75 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (173:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 78, 173) */
		reached[7][78] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 167: // STATE 80 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 168: // STATE 84 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][84] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 169: // STATE 87 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][87] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 170: // STATE 88 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 171: // STATE 90 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (173:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
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
	case 172: // STATE 95 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][95] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 173: // STATE 96 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 174: // STATE 97 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][97] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 175: // STATE 100 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 176: // STATE 101 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][101] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 177: // STATE 104 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 178: // STATE 105 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][105] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 179: // STATE 109 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (173:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 112, 173) */
		reached[7][112] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 180: // STATE 114 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][114] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 181: // STATE 118 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][118] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 182: // STATE 121 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][121] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 183: // STATE 122 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 184: // STATE 125 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 185: // STATE 129 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][129] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 186: // STATE 130 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (173:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 138, 173) */
		reached[7][138] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 187: // STATE 133 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (173:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 138, 173) */
		reached[7][138] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 188: // STATE 137 - modemanager.pml:73 - [transition_id = -(1)] (0:173:1 - 3)
		IfNotBlocked
		reached[7][137] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 138, 173) */
		reached[7][138] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 189: // STATE 139 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][139] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 190: // STATE 143 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][143] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 191: // STATE 146 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (173:0:1 - 1)
		IfNotBlocked
		reached[7][146] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 147, 173) */
		reached[7][147] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 148, 173) */
		reached[7][148] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 192: // STATE 149 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (173:0:2 - 1)
		IfNotBlocked
		reached[7][149] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(173, 150, 173) */
		reached[7][150] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
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
	case 193: // STATE 155 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][155] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 194: // STATE 159 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][159] = 1;
		if (!((((P7 *)_this)->_1065_8_9_30_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_30_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_30_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 195: // STATE 162 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (173:0:1 - 1)
		IfNotBlocked
		reached[7][162] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 163, 173) */
		reached[7][163] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 164, 173) */
		reached[7][164] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 196: // STATE 165 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (173:0:2 - 1)
		IfNotBlocked
		reached[7][165] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(173, 166, 173) */
		reached[7][166] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_30_transition_id;
		((P7 *)_this)->_1065_8_9_30_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_30_transition_id);
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
	case 197: // STATE 178 - modemanager.pml:156 - [((global_state.modemanager.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][178] = 1;
		if (!((now.global_state.modemanager.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 198: // STATE 179 - dataview.pml:9064 - [global_state.modemanager.stc.selection = Modemanager_systemtc_signal_parameter.selection] (0:0:1 - 1)
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
	case 199: // STATE 180 - dataview.pml:9066 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][180] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 200: // STATE 181 - dataview.pml:9023 - [global_state.modemanager.stc.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle] (0:0:1 - 1)
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
	case 201: // STATE 182 - dataview.pml:9028 - [assert(((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][182] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToIdle>=0)&&(now.global_state.modemanager.stc.data.goToIdle<=0)), "((global_state.modemanager.stc.data.goToIdle>=0)&&(global_state.modemanager.stc.data.goToIdle<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 202: // STATE 185 - dataview.pml:9068 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][185] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 203: // STATE 186 - dataview.pml:9032 - [global_state.modemanager.stc.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 204: // STATE 187 - dataview.pml:9037 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][187] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold1<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold1>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 205: // STATE 190 - dataview.pml:9041 - [global_state.modemanager.stc.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 206: // STATE 191 - dataview.pml:9046 - [assert(((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][191] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(now.global_state.modemanager.stc.data.goToOperational.threshold2<=255)), "((global_state.modemanager.stc.data.goToOperational.threshold2>=0)&&(global_state.modemanager.stc.data.goToOperational.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 207: // STATE 195 - dataview.pml:9070 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][195] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 208: // STATE 196 - dataview.pml:9055 - [global_state.modemanager.stc.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport] (0:0:1 - 1)
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
	case 209: // STATE 197 - dataview.pml:9060 - [assert(((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][197] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stc.data.doReport>=0)&&(now.global_state.modemanager.stc.data.doReport<=0)), "((global_state.modemanager.stc.data.doReport>=0)&&(global_state.modemanager.stc.data.doReport<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 210: // STATE 205 - modemanager.pml:4 - [transition_id = 0] (0:345:2 - 1)
		IfNotBlocked
		reached[7][205] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: transition_id = 2(345, 206, 345) */
		reached[7][206] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id = 2;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: .(goto)(0, 346, 345) */
		reached[7][346] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 211: // STATE 207 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][207] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 212: // STATE 209 - modemanager.pml:9 - [((transition_id==0))] (344:0:3 - 1)
		IfNotBlocked
		reached[7][209] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		/* merge: transition_id = -(1)(344, 210, 344) */
		reached[7][210] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
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
	case 213: // STATE 213 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][213] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 214: // STATE 216 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][216] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 215: // STATE 217 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 216: // STATE 218 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][218] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 217: // STATE 221 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 218: // STATE 222 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][222] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 219: // STATE 225 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 220: // STATE 227 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (344:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
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
	case 221: // STATE 232 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][232] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 222: // STATE 233 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 223: // STATE 234 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][234] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 224: // STATE 237 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 225: // STATE 238 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][238] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 226: // STATE 241 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 227: // STATE 242 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][242] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 228: // STATE 246 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (344:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 249, 344) */
		reached[7][249] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 229: // STATE 251 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][251] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 230: // STATE 255 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][255] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 231: // STATE 258 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][258] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 232: // STATE 259 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 233: // STATE 261 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (344:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
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
	case 234: // STATE 266 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][266] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 235: // STATE 267 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 236: // STATE 268 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][268] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 237: // STATE 271 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 238: // STATE 272 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][272] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 239: // STATE 275 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 240: // STATE 276 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][276] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 241: // STATE 280 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (344:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 283, 344) */
		reached[7][283] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 242: // STATE 285 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][285] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 243: // STATE 289 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][289] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 244: // STATE 292 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][292] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 245: // STATE 293 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 246: // STATE 296 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 247: // STATE 300 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][300] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 248: // STATE 301 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (344:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 309, 344) */
		reached[7][309] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 249: // STATE 304 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (344:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 309, 344) */
		reached[7][309] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 250: // STATE 308 - modemanager.pml:73 - [transition_id = -(1)] (0:344:1 - 3)
		IfNotBlocked
		reached[7][308] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 309, 344) */
		reached[7][309] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 251: // STATE 310 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][310] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 252: // STATE 314 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][314] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 253: // STATE 317 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (344:0:1 - 1)
		IfNotBlocked
		reached[7][317] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 318, 344) */
		reached[7][318] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 319, 344) */
		reached[7][319] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 254: // STATE 320 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (344:0:2 - 1)
		IfNotBlocked
		reached[7][320] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(344, 321, 344) */
		reached[7][321] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
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
	case 255: // STATE 326 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][326] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 256: // STATE 330 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][330] = 1;
		if (!((((P7 *)_this)->_1065_8_9_32_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_9_32_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_9_32_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 257: // STATE 333 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (344:0:1 - 1)
		IfNotBlocked
		reached[7][333] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 334, 344) */
		reached[7][334] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 335, 344) */
		reached[7][335] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 258: // STATE 336 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (344:0:2 - 1)
		IfNotBlocked
		reached[7][336] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(344, 337, 344) */
		reached[7][337] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_9_32_transition_id;
		((P7 *)_this)->_1065_8_9_32_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_9_32_transition_id);
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
	case 259: // STATE 354 - system.pml:250 - [Untitled_msc_lock?_] (0:0:1 - 8)
		reached[7][354] = 1;
		if (q_len(now.Untitled_msc_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Untitled_msc_lock, XX-1, 0, 0);
		;
		qrecv(now.Untitled_msc_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Untitled_msc_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 260: // STATE 355 - Untitled_msc.pml:55 - [((global_state.untitled_msc.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][355] = 1;
		if (!((now.global_state.untitled_msc.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 261: // STATE 356 - dataview.pml:9064 - [Untitled_MSC_0_sig0_param0.selection = Modemanager_systemtc_signal_parameter.selection] (0:0:1 - 1)
		IfNotBlocked
		reached[7][356] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig0_param0.selection;
		now.Untitled_MSC_0_sig0_param0.selection = now.Modemanager_systemtc_signal_parameter.selection;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig0_param0.selection", now.Untitled_MSC_0_sig0_param0.selection);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 262: // STATE 357 - dataview.pml:9066 - [((Untitled_MSC_0_sig0_param0.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][357] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 263: // STATE 358 - dataview.pml:9023 - [Untitled_MSC_0_sig0_param0.data.goToIdle = Modemanager_systemtc_signal_parameter.data.goToIdle] (0:0:1 - 1)
		IfNotBlocked
		reached[7][358] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig0_param0.data.goToIdle;
		now.Untitled_MSC_0_sig0_param0.data.goToIdle = now.Modemanager_systemtc_signal_parameter.data.goToIdle;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig0_param0.data.goToIdle", now.Untitled_MSC_0_sig0_param0.data.goToIdle);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 264: // STATE 359 - dataview.pml:9028 - [assert(((Untitled_MSC_0_sig0_param0.data.goToIdle>=0)&&(Untitled_MSC_0_sig0_param0.data.goToIdle<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][359] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.Untitled_MSC_0_sig0_param0.data.goToIdle>=0)&&(now.Untitled_MSC_0_sig0_param0.data.goToIdle<=0)), "((Untitled_MSC_0_sig0_param0.data.goToIdle>=0)&&(Untitled_MSC_0_sig0_param0.data.goToIdle<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 265: // STATE 362 - dataview.pml:9068 - [((Untitled_MSC_0_sig0_param0.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][362] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 266: // STATE 363 - dataview.pml:9032 - [Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1] (0:0:1 - 1)
		IfNotBlocked
		reached[7][363] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1;
		now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1 = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold1;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1", now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 267: // STATE 364 - dataview.pml:9037 - [assert(((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][364] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1>=0)&&(now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1<=255)), "((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 268: // STATE 367 - dataview.pml:9041 - [Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2 = Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2] (0:0:1 - 1)
		IfNotBlocked
		reached[7][367] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2;
		now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2 = now.Modemanager_systemtc_signal_parameter.data.goToOperational.threshold2;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2", now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 269: // STATE 368 - dataview.pml:9046 - [assert(((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][368] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2>=0)&&(now.Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2<=255)), "((Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2>=0)&&(Untitled_MSC_0_sig0_param0.data.goToOperational.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 270: // STATE 372 - dataview.pml:9070 - [((Untitled_MSC_0_sig0_param0.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][372] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 271: // STATE 373 - dataview.pml:9055 - [Untitled_MSC_0_sig0_param0.data.doReport = Modemanager_systemtc_signal_parameter.data.doReport] (0:0:1 - 1)
		IfNotBlocked
		reached[7][373] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig0_param0.data.doReport;
		now.Untitled_MSC_0_sig0_param0.data.doReport = now.Modemanager_systemtc_signal_parameter.data.doReport;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig0_param0.data.doReport", now.Untitled_MSC_0_sig0_param0.data.doReport);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 272: // STATE 374 - dataview.pml:9060 - [assert(((Untitled_MSC_0_sig0_param0.data.doReport>=0)&&(Untitled_MSC_0_sig0_param0.data.doReport<=0)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][374] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.Untitled_MSC_0_sig0_param0.data.doReport>=0)&&(now.Untitled_MSC_0_sig0_param0.data.doReport<=0)), "((Untitled_MSC_0_sig0_param0.data.doReport>=0)&&(Untitled_MSC_0_sig0_param0.data.doReport<=0))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 273: // STATE 382 - Untitled_msc.pml:6 - [transition_id = 0] (0:417:2 - 1)
		IfNotBlocked
		reached[7][382] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: transition_id = 1(417, 383, 417) */
		reached[7][383] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id = 1;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: .(goto)(0, 418, 417) */
		reached[7][418] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 274: // STATE 384 - Untitled_msc.pml:9 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][384] = 1;
		if (!((((P7 *)_this)->_1065_8_10_34_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_34_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_10_34_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_34_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 275: // STATE 386 - Untitled_msc.pml:11 - [((transition_id==0))] (416:0:3 - 1)
		IfNotBlocked
		reached[7][386] = 1;
		if (!((((P7 *)_this)->_1065_8_10_34_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_34_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_34_transition_id = 0;
		/* merge: transition_id = -(1)(416, 387, 416) */
		reached[7][387] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(416, 388, 416) */
		reached[7][388] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 389, 416) */
		reached[7][389] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 276: // STATE 390 - Untitled_msc.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][390] = 1;
		if (!((((P7 *)_this)->_1065_8_10_34_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_34_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_10_34_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_34_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 277: // STATE 391 - Untitled_msc.pml:17 - [((Untitled_MSC_0_sig0_param0.selection==3))] (416:0:2 - 1)
		IfNotBlocked
		reached[7][391] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==3)))
			continue;
		/* merge: transition_id = -(1)(416, 392, 416) */
		reached[7][392] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 2(416, 393, 416) */
		reached[7][393] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 2;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 394, 416) */
		reached[7][394] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 278: // STATE 396 - Untitled_msc.pml:22 - [(1)] (416:0:2 - 1)
		IfNotBlocked
		reached[7][396] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(416, 398, 416) */
		reached[7][398] = 1;
		;
		/* merge: transition_id = -(1)(416, 399, 416) */
		reached[7][399] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(416, 400, 416) */
		reached[7][400] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 401, 416) */
		reached[7][401] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 279: // STATE 399 - Untitled_msc.pml:24 - [transition_id = -(1)] (0:416:2 - 2)
		IfNotBlocked
		reached[7][399] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(416, 400, 416) */
		reached[7][400] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 401, 416) */
		reached[7][401] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 280: // STATE 402 - Untitled_msc.pml:27 - [((transition_id==2))] (416:0:3 - 1)
		IfNotBlocked
		reached[7][402] = 1;
		if (!((((P7 *)_this)->_1065_8_10_34_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_34_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_34_transition_id = 0;
		/* merge: transition_id = -(1)(416, 403, 416) */
		reached[7][403] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(416, 404, 416) */
		reached[7][404] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 405, 416) */
		reached[7][405] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 281: // STATE 406 - Untitled_msc.pml:31 - [((transition_id==3))] (416:0:3 - 1)
		IfNotBlocked
		reached[7][406] = 1;
		if (!((((P7 *)_this)->_1065_8_10_34_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_34_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_34_transition_id = 0;
		/* merge: transition_id = -(1)(416, 407, 416) */
		reached[7][407] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 1(416, 408, 416) */
		reached[7][408] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 1;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 409, 416) */
		reached[7][409] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 282: // STATE 410 - Untitled_msc.pml:35 - [((transition_id==4))] (416:0:3 - 1)
		IfNotBlocked
		reached[7][410] = 1;
		if (!((((P7 *)_this)->_1065_8_10_34_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_34_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_34_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_34_transition_id = 0;
		/* merge: transition_id = -(1)(416, 411, 416) */
		reached[7][411] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_34_transition_id;
		((P7 *)_this)->_1065_8_10_34_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_34_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 3(416, 412, 416) */
		reached[7][412] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 3;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 413, 416) */
		reached[7][413] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 283: // STATE 421 - Untitled_msc.pml:58 - [((global_state.untitled_msc.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][421] = 1;
		if (!((now.global_state.untitled_msc.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 284: // STATE 422 - Untitled_msc.pml:6 - [transition_id = 0] (0:457:2 - 1)
		IfNotBlocked
		reached[7][422] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: transition_id = 4(457, 423, 457) */
		reached[7][423] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id = 4;
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: .(goto)(0, 458, 457) */
		reached[7][458] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 285: // STATE 424 - Untitled_msc.pml:9 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][424] = 1;
		if (!((((P7 *)_this)->_1065_8_10_35_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_35_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_10_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_35_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 286: // STATE 426 - Untitled_msc.pml:11 - [((transition_id==0))] (456:0:3 - 1)
		IfNotBlocked
		reached[7][426] = 1;
		if (!((((P7 *)_this)->_1065_8_10_35_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_35_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_35_transition_id = 0;
		/* merge: transition_id = -(1)(456, 427, 456) */
		reached[7][427] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(456, 428, 456) */
		reached[7][428] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 429, 456) */
		reached[7][429] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 287: // STATE 430 - Untitled_msc.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][430] = 1;
		if (!((((P7 *)_this)->_1065_8_10_35_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_35_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_1065_8_10_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_35_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 288: // STATE 431 - Untitled_msc.pml:17 - [((Untitled_MSC_0_sig0_param0.selection==3))] (456:0:2 - 1)
		IfNotBlocked
		reached[7][431] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==3)))
			continue;
		/* merge: transition_id = -(1)(456, 432, 456) */
		reached[7][432] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 2(456, 433, 456) */
		reached[7][433] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 2;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 434, 456) */
		reached[7][434] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 289: // STATE 436 - Untitled_msc.pml:22 - [(1)] (456:0:2 - 1)
		IfNotBlocked
		reached[7][436] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(456, 438, 456) */
		reached[7][438] = 1;
		;
		/* merge: transition_id = -(1)(456, 439, 456) */
		reached[7][439] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(456, 440, 456) */
		reached[7][440] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 441, 456) */
		reached[7][441] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 290: // STATE 439 - Untitled_msc.pml:24 - [transition_id = -(1)] (0:456:2 - 2)
		IfNotBlocked
		reached[7][439] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(456, 440, 456) */
		reached[7][440] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 441, 456) */
		reached[7][441] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 291: // STATE 442 - Untitled_msc.pml:27 - [((transition_id==2))] (456:0:3 - 1)
		IfNotBlocked
		reached[7][442] = 1;
		if (!((((P7 *)_this)->_1065_8_10_35_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_35_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_35_transition_id = 0;
		/* merge: transition_id = -(1)(456, 443, 456) */
		reached[7][443] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(456, 444, 456) */
		reached[7][444] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 445, 456) */
		reached[7][445] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 292: // STATE 446 - Untitled_msc.pml:31 - [((transition_id==3))] (456:0:3 - 1)
		IfNotBlocked
		reached[7][446] = 1;
		if (!((((P7 *)_this)->_1065_8_10_35_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_35_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_35_transition_id = 0;
		/* merge: transition_id = -(1)(456, 447, 456) */
		reached[7][447] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 1(456, 448, 456) */
		reached[7][448] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 1;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 449, 456) */
		reached[7][449] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 293: // STATE 450 - Untitled_msc.pml:35 - [((transition_id==4))] (456:0:3 - 1)
		IfNotBlocked
		reached[7][450] = 1;
		if (!((((P7 *)_this)->_1065_8_10_35_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1065_8_10_35_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_1065_8_10_35_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_1065_8_10_35_transition_id = 0;
		/* merge: transition_id = -(1)(456, 451, 456) */
		reached[7][451] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_1065_8_10_35_transition_id;
		((P7 *)_this)->_1065_8_10_35_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_systemTc:transition_id", ((P7 *)_this)->_1065_8_10_35_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 3(456, 452, 456) */
		reached[7][452] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 3;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 453, 456) */
		reached[7][453] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 294: // STATE 466 - system.pml:252 - [Untitled_msc_lock!1] (0:0:0 - 8)
		IfNotBlocked
		reached[7][466] = 1;
		if (q_full(now.Untitled_msc_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Untitled_msc_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Untitled_msc_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 295: // STATE 468 - system.pml:254 - [(empty(Modemanager_systemTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][468] = 1;
		if (!((q_len(now.Modemanager_systemTc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 296: // STATE 469 - system.pml:255 - [(1)] (472:0:0 - 1)
		IfNotBlocked
		reached[7][469] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 471, 472) */
		reached[7][471] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 297: // STATE 472 - system.pml:257 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[7][472] = 1;
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
	case 298: // STATE 477 - system.pml:260 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[7][477] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_reading */
	case 299: // STATE 1 - system.pml:218 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[6][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 300: // STATE 2 - system.pml:221 - [(nempty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][2] = 1;
		if (!((q_len(now.Modemanager_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 301: // STATE 3 - system.pml:222 - [Modemanager_lock?_] (0:0:1 - 1)
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
	case 302: // STATE 4 - system.pml:225 - [(nempty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][4] = 1;
		if (!((q_len(now.Modemanager_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 303: // STATE 5 - system.pml:226 - [Modemanager_reading_channel?Modemanager_reading_signal_parameter] (160:0:2 - 1)
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
	case 304: // STATE 7 - modemanager.pml:143 - [((global_state.modemanager.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][7] = 1;
		if (!((now.global_state.modemanager.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 305: // STATE 8 - dataview.pml:8964 - [global_state.modemanager.r = Modemanager_reading_signal_parameter] (0:0:1 - 1)
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
	case 306: // STATE 9 - dataview.pml:8969 - [assert(((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.r>=0)&&(now.global_state.modemanager.r<=255)), "((global_state.modemanager.r>=0)&&(global_state.modemanager.r<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 307: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:152:2 - 1)
		IfNotBlocked
		reached[6][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: transition_id = 3(152, 13, 152) */
		reached[6][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id = 3;
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: .(goto)(0, 153, 152) */
		reached[6][153] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 308: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][14] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 309: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (151:0:3 - 1)
		IfNotBlocked
		reached[6][16] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		/* merge: transition_id = -(1)(151, 17, 151) */
		reached[6][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
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
	case 310: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][20] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 311: // STATE 23 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][23] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 312: // STATE 24 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 313: // STATE 25 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 314: // STATE 28 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 315: // STATE 29 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 316: // STATE 32 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 317: // STATE 34 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (151:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
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
	case 318: // STATE 39 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][39] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 319: // STATE 40 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 320: // STATE 41 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][41] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 321: // STATE 44 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 322: // STATE 45 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 323: // STATE 48 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 324: // STATE 49 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 325: // STATE 53 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 151) */
		reached[6][56] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 326: // STATE 58 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[6][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 327: // STATE 62 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][62] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 328: // STATE 65 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][65] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 329: // STATE 66 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 330: // STATE 68 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (151:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
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
	case 331: // STATE 73 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][73] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 332: // STATE 74 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 333: // STATE 75 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][75] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 334: // STATE 78 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 335: // STATE 79 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][79] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 336: // STATE 82 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 337: // STATE 83 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][83] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 338: // STATE 87 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 151) */
		reached[6][90] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 339: // STATE 92 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[6][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 340: // STATE 96 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][96] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 341: // STATE 99 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][99] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 342: // STATE 100 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 343: // STATE 103 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 344: // STATE 107 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][107] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 345: // STATE 108 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[6][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 346: // STATE 111 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[6][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 347: // STATE 115 - modemanager.pml:73 - [transition_id = -(1)] (0:151:1 - 3)
		IfNotBlocked
		reached[6][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[6][116] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 348: // STATE 117 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][117] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 349: // STATE 121 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][121] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 350: // STATE 124 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[6][124] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 125, 151) */
		reached[6][125] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 126, 151) */
		reached[6][126] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 351: // STATE 127 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[6][127] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 128, 151) */
		reached[6][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
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
	case 352: // STATE 133 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][133] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 353: // STATE 137 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[6][137] = 1;
		if (!((((P6 *)_this)->_1064_7_8_28_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1064_7_8_28_transition_id */  (trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P6 *)_this)->_1064_7_8_28_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 354: // STATE 140 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[6][140] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 141, 151) */
		reached[6][141] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 142, 151) */
		reached[6][142] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 355: // STATE 143 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[6][143] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 144, 151) */
		reached[6][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->_1064_7_8_28_transition_id;
		((P6 *)_this)->_1064_7_8_28_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P6 *)_this)->_1064_7_8_28_transition_id);
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
	case 356: // STATE 162 - system.pml:230 - [(empty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][162] = 1;
		if (!((q_len(now.Modemanager_reading_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 357: // STATE 163 - system.pml:231 - [(1)] (166:0:0 - 1)
		IfNotBlocked
		reached[6][163] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 165, 166) */
		reached[6][165] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 358: // STATE 166 - system.pml:233 - [Modemanager_lock!1] (0:0:0 - 1)
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
	case 359: // STATE 171 - system.pml:236 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[6][171] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_actuatorTm2 */
	case 360: // STATE 1 - system.pml:197 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[5][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 361: // STATE 2 - system.pml:200 - [(nempty(Modemanager_actuatorTm2_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][2] = 1;
		if (!((q_len(now.Modemanager_actuatorTm2_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 362: // STATE 3 - system.pml:201 - [Modemanager_lock?_] (0:0:1 - 1)
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
	case 363: // STATE 4 - system.pml:204 - [(nempty(Modemanager_actuatorTm2_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][4] = 1;
		if (!((q_len(now.Modemanager_actuatorTm2_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 364: // STATE 5 - system.pml:205 - [Modemanager_actuatorTm2_channel?Modemanager_actuatortm2_signal_parameter] (309:0:2 - 1)
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
	case 365: // STATE 7 - modemanager.pml:130 - [((global_state.modemanager.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][7] = 1;
		if (!((now.global_state.modemanager.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 366: // STATE 8 - dataview.pml:8991 - [global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter] (0:0:1 - 1)
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
	case 367: // STATE 9 - dataview.pml:8996 - [assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status2==0)||(now.global_state.modemanager.status2==1))||(now.global_state.modemanager.status2==2)), "(((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 368: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:152:2 - 1)
		IfNotBlocked
		reached[5][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: transition_id = 4(152, 13, 152) */
		reached[5][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id = 4;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: .(goto)(0, 153, 152) */
		reached[5][153] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 369: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][14] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 370: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (151:0:3 - 1)
		IfNotBlocked
		reached[5][16] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		/* merge: transition_id = -(1)(151, 17, 151) */
		reached[5][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
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
	case 371: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][20] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 372: // STATE 23 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][23] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 373: // STATE 24 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 374: // STATE 25 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 375: // STATE 28 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 376: // STATE 29 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 377: // STATE 32 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 378: // STATE 34 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (151:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
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
	case 379: // STATE 39 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][39] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 380: // STATE 40 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 381: // STATE 41 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][41] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 382: // STATE 44 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 383: // STATE 45 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 384: // STATE 48 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 385: // STATE 49 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 386: // STATE 53 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 151) */
		reached[5][56] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 387: // STATE 58 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 388: // STATE 62 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][62] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 389: // STATE 65 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][65] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 390: // STATE 66 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 391: // STATE 68 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (151:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
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
	case 392: // STATE 73 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][73] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 393: // STATE 74 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 394: // STATE 75 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][75] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 395: // STATE 78 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 396: // STATE 79 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][79] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 397: // STATE 82 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 398: // STATE 83 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][83] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 399: // STATE 87 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 151) */
		reached[5][90] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 400: // STATE 92 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 401: // STATE 96 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][96] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 402: // STATE 99 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][99] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 403: // STATE 100 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 404: // STATE 103 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 405: // STATE 107 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][107] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 406: // STATE 108 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[5][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 407: // STATE 111 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[5][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 408: // STATE 115 - modemanager.pml:73 - [transition_id = -(1)] (0:151:1 - 3)
		IfNotBlocked
		reached[5][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[5][116] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 409: // STATE 117 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][117] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 410: // STATE 121 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][121] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 411: // STATE 124 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[5][124] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 125, 151) */
		reached[5][125] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 126, 151) */
		reached[5][126] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 412: // STATE 127 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[5][127] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 128, 151) */
		reached[5][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
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
	case 413: // STATE 133 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][133] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 414: // STATE 137 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][137] = 1;
		if (!((((P5 *)_this)->_1063_6_7_24_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_24_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_24_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 415: // STATE 140 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[5][140] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 141, 151) */
		reached[5][141] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 142, 151) */
		reached[5][142] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 416: // STATE 143 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[5][143] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 144, 151) */
		reached[5][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_24_transition_id;
		((P5 *)_this)->_1063_6_7_24_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_24_transition_id);
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
	case 417: // STATE 156 - modemanager.pml:133 - [((global_state.modemanager.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][156] = 1;
		if (!((now.global_state.modemanager.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 418: // STATE 157 - dataview.pml:8991 - [global_state.modemanager.status2 = Modemanager_actuatortm2_signal_parameter] (0:0:1 - 1)
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
	case 419: // STATE 158 - dataview.pml:8996 - [assert((((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][158] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status2==0)||(now.global_state.modemanager.status2==1))||(now.global_state.modemanager.status2==2)), "(((global_state.modemanager.status2==0)||(global_state.modemanager.status2==1))||(global_state.modemanager.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 420: // STATE 161 - modemanager.pml:4 - [transition_id = 0] (0:301:2 - 1)
		IfNotBlocked
		reached[5][161] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: transition_id = 6(301, 162, 301) */
		reached[5][162] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id = 6;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: .(goto)(0, 302, 301) */
		reached[5][302] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 421: // STATE 163 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][163] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 422: // STATE 165 - modemanager.pml:9 - [((transition_id==0))] (300:0:3 - 1)
		IfNotBlocked
		reached[5][165] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		/* merge: transition_id = -(1)(300, 166, 300) */
		reached[5][166] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
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
	case 423: // STATE 169 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][169] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 424: // STATE 172 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][172] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 425: // STATE 173 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 426: // STATE 174 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][174] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 427: // STATE 177 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 428: // STATE 178 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][178] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 429: // STATE 181 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 430: // STATE 183 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (300:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
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
	case 431: // STATE 188 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][188] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 432: // STATE 189 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 433: // STATE 190 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][190] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 434: // STATE 193 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 435: // STATE 194 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][194] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 436: // STATE 197 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 437: // STATE 198 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][198] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 438: // STATE 202 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 205, 300) */
		reached[5][205] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 439: // STATE 207 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][207] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 440: // STATE 211 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][211] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 441: // STATE 214 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][214] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 442: // STATE 215 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 443: // STATE 217 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (300:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
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
	case 444: // STATE 222 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][222] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 445: // STATE 223 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 446: // STATE 224 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][224] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 447: // STATE 227 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 448: // STATE 228 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][228] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 449: // STATE 231 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 450: // STATE 232 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][232] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 451: // STATE 236 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 239, 300) */
		reached[5][239] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 452: // STATE 241 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][241] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 453: // STATE 245 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][245] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 454: // STATE 248 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][248] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 455: // STATE 249 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 456: // STATE 252 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 457: // STATE 256 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][256] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 458: // STATE 257 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[5][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 459: // STATE 260 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[5][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 460: // STATE 264 - modemanager.pml:73 - [transition_id = -(1)] (0:300:1 - 3)
		IfNotBlocked
		reached[5][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[5][265] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 461: // STATE 266 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][266] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 462: // STATE 270 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][270] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 463: // STATE 273 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[5][273] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 274, 300) */
		reached[5][274] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 275, 300) */
		reached[5][275] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 464: // STATE 276 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[5][276] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 277, 300) */
		reached[5][277] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
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
	case 465: // STATE 282 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][282] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 466: // STATE 286 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][286] = 1;
		if (!((((P5 *)_this)->_1063_6_7_26_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1063_6_7_26_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_1063_6_7_26_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 467: // STATE 289 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[5][289] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 290, 300) */
		reached[5][290] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 291, 300) */
		reached[5][291] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 468: // STATE 292 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[5][292] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 293, 300) */
		reached[5][293] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_1063_6_7_26_transition_id;
		((P5 *)_this)->_1063_6_7_26_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm2:transition_id", ((P5 *)_this)->_1063_6_7_26_transition_id);
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
	case 469: // STATE 311 - system.pml:209 - [(empty(Modemanager_actuatorTm2_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][311] = 1;
		if (!((q_len(now.Modemanager_actuatorTm2_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 470: // STATE 312 - system.pml:210 - [(1)] (315:0:0 - 1)
		IfNotBlocked
		reached[5][312] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 314, 315) */
		reached[5][314] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 471: // STATE 315 - system.pml:212 - [Modemanager_lock!1] (0:0:0 - 1)
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
	case 472: // STATE 320 - system.pml:215 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[5][320] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_actuatorTm1 */
	case 473: // STATE 1 - system.pml:176 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[4][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 474: // STATE 2 - system.pml:179 - [(nempty(Modemanager_actuatorTm1_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][2] = 1;
		if (!((q_len(now.Modemanager_actuatorTm1_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 475: // STATE 3 - system.pml:180 - [Modemanager_lock?_] (0:0:1 - 1)
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
	case 476: // STATE 4 - system.pml:183 - [(nempty(Modemanager_actuatorTm1_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][4] = 1;
		if (!((q_len(now.Modemanager_actuatorTm1_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 477: // STATE 5 - system.pml:184 - [Modemanager_actuatorTm1_channel?Modemanager_actuatortm1_signal_parameter] (309:0:2 - 1)
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
	case 478: // STATE 7 - modemanager.pml:117 - [((global_state.modemanager.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][7] = 1;
		if (!((now.global_state.modemanager.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 479: // STATE 8 - dataview.pml:8991 - [global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter] (0:0:1 - 1)
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
	case 480: // STATE 9 - dataview.pml:8996 - [assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status1==0)||(now.global_state.modemanager.status1==1))||(now.global_state.modemanager.status1==2)), "(((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 481: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:152:2 - 1)
		IfNotBlocked
		reached[4][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: transition_id = 5(152, 13, 152) */
		reached[4][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id = 5;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: .(goto)(0, 153, 152) */
		reached[4][153] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 482: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][14] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 483: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (151:0:3 - 1)
		IfNotBlocked
		reached[4][16] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		/* merge: transition_id = -(1)(151, 17, 151) */
		reached[4][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
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
	case 484: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][20] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 485: // STATE 23 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][23] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 486: // STATE 24 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 487: // STATE 25 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][25] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 488: // STATE 28 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 489: // STATE 29 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 490: // STATE 32 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 491: // STATE 34 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (151:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
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
	case 492: // STATE 39 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][39] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 493: // STATE 40 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 494: // STATE 41 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][41] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 495: // STATE 44 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 496: // STATE 45 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 497: // STATE 48 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 498: // STATE 49 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 499: // STATE 53 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 151) */
		reached[4][56] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 500: // STATE 58 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][58] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 501: // STATE 62 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][62] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 502: // STATE 65 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][65] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 503: // STATE 66 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 504: // STATE 68 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (151:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
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
	case 505: // STATE 73 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][73] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 506: // STATE 74 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 507: // STATE 75 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][75] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 508: // STATE 78 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 509: // STATE 79 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][79] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 510: // STATE 82 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 511: // STATE 83 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][83] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 512: // STATE 87 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 90, 151) */
		reached[4][90] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 513: // STATE 92 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][92] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 514: // STATE 96 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][96] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 515: // STATE 99 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][99] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 516: // STATE 100 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 517: // STATE 103 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 518: // STATE 107 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][107] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 519: // STATE 108 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[4][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 520: // STATE 111 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (151:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[4][116] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 521: // STATE 115 - modemanager.pml:73 - [transition_id = -(1)] (0:151:1 - 3)
		IfNotBlocked
		reached[4][115] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 116, 151) */
		reached[4][116] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 522: // STATE 117 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][117] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 523: // STATE 121 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][121] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 524: // STATE 124 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[4][124] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 125, 151) */
		reached[4][125] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 126, 151) */
		reached[4][126] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 525: // STATE 127 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[4][127] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 128, 151) */
		reached[4][128] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
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
	case 526: // STATE 133 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][133] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 527: // STATE 137 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][137] = 1;
		if (!((((P4 *)_this)->_1062_5_6_20_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_20_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_20_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 528: // STATE 140 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (151:0:1 - 1)
		IfNotBlocked
		reached[4][140] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 141, 151) */
		reached[4][141] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 142, 151) */
		reached[4][142] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 529: // STATE 143 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (151:0:2 - 1)
		IfNotBlocked
		reached[4][143] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(151, 144, 151) */
		reached[4][144] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_20_transition_id;
		((P4 *)_this)->_1062_5_6_20_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_20_transition_id);
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
	case 530: // STATE 156 - modemanager.pml:120 - [((global_state.modemanager.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][156] = 1;
		if (!((now.global_state.modemanager.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 531: // STATE 157 - dataview.pml:8991 - [global_state.modemanager.status1 = Modemanager_actuatortm1_signal_parameter] (0:0:1 - 1)
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
	case 532: // STATE 158 - dataview.pml:8996 - [assert((((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][158] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.status1==0)||(now.global_state.modemanager.status1==1))||(now.global_state.modemanager.status1==2)), "(((global_state.modemanager.status1==0)||(global_state.modemanager.status1==1))||(global_state.modemanager.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 533: // STATE 161 - modemanager.pml:4 - [transition_id = 0] (0:301:2 - 1)
		IfNotBlocked
		reached[4][161] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: transition_id = 7(301, 162, 301) */
		reached[4][162] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id = 7;
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: .(goto)(0, 302, 301) */
		reached[4][302] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 534: // STATE 163 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][163] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 535: // STATE 165 - modemanager.pml:9 - [((transition_id==0))] (300:0:3 - 1)
		IfNotBlocked
		reached[4][165] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		/* merge: transition_id = -(1)(300, 166, 300) */
		reached[4][166] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
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
	case 536: // STATE 169 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][169] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 537: // STATE 172 - modemanager.pml:16 - [((global_state.modemanager.stc.selection==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][172] = 1;
		if (!((now.global_state.modemanager.stc.selection==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 538: // STATE 173 - dataview.pml:8964 - [global_state.modemanager.threshold1 = global_state.modemanager.stc.data.goToOperational.threshold1] (0:0:1 - 1)
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
	case 539: // STATE 174 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][174] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold1>=0)&&(now.global_state.modemanager.threshold1<=255)), "((global_state.modemanager.threshold1>=0)&&(global_state.modemanager.threshold1<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 540: // STATE 177 - dataview.pml:8964 - [global_state.modemanager.threshold2 = global_state.modemanager.stc.data.goToOperational.threshold2] (0:0:1 - 1)
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
	case 541: // STATE 178 - dataview.pml:8969 - [assert(((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][178] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.threshold2>=0)&&(now.global_state.modemanager.threshold2<=255)), "((global_state.modemanager.threshold2>=0)&&(global_state.modemanager.threshold2<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 542: // STATE 181 - system.pml:44 - [Actuator1_actuatorTc_channel!0] (0:0:0 - 1)
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
	case 543: // STATE 183 - system.pml:58 - [Actuator2_actuatorTc_channel!0] (300:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
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
	case 544: // STATE 188 - modemanager.pml:24 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][188] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 545: // STATE 189 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 546: // STATE 190 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][190] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 547: // STATE 193 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 548: // STATE 194 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][194] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 549: // STATE 197 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 550: // STATE 198 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][198] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 551: // STATE 202 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 205, 300) */
		reached[4][205] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 552: // STATE 207 - modemanager.pml:34 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][207] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 553: // STATE 211 - modemanager.pml:37 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][211] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 554: // STATE 214 - modemanager.pml:40 - [((global_state.modemanager.stc.selection==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][214] = 1;
		if (!((now.global_state.modemanager.stc.selection==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 555: // STATE 215 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 556: // STATE 217 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (300:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
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
	case 557: // STATE 222 - modemanager.pml:46 - [((global_state.modemanager.stc.selection==3))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][222] = 1;
		if (!((now.global_state.modemanager.stc.selection==3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 558: // STATE 223 - dataview.pml:8991 - [global_state.modemanager.stm.status1 = global_state.modemanager.status1] (0:0:1 - 1)
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
	case 559: // STATE 224 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][224] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status1==0)||(now.global_state.modemanager.stm.status1==1))||(now.global_state.modemanager.stm.status1==2)), "(((global_state.modemanager.stm.status1==0)||(global_state.modemanager.stm.status1==1))||(global_state.modemanager.stm.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 560: // STATE 227 - dataview.pml:8991 - [global_state.modemanager.stm.status2 = global_state.modemanager.status2] (0:0:1 - 1)
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
	case 561: // STATE 228 - dataview.pml:8996 - [assert((((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][228] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.modemanager.stm.status2==0)||(now.global_state.modemanager.stm.status2==1))||(now.global_state.modemanager.stm.status2==2)), "(((global_state.modemanager.stm.status2==0)||(global_state.modemanager.stm.status2==1))||(global_state.modemanager.stm.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 562: // STATE 231 - dataview.pml:8964 - [global_state.modemanager.stm.reading = global_state.modemanager.r] (0:0:1 - 1)
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
	case 563: // STATE 232 - dataview.pml:8969 - [assert(((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][232] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.stm.reading>=0)&&(now.global_state.modemanager.stm.reading<=255)), "((global_state.modemanager.stm.reading>=0)&&(global_state.modemanager.stm.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 564: // STATE 236 - system.pml:72 - [Egse_systemTm_channel!global_state.modemanager.stm.reading,global_state.modemanager.stm.status1,global_state.modemanager.stm.status2] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 239, 300) */
		reached[4][239] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 565: // STATE 241 - modemanager.pml:56 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][241] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 566: // STATE 245 - modemanager.pml:59 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][245] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 567: // STATE 248 - modemanager.pml:62 - [((global_state.modemanager.r<global_state.modemanager.threshold1))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][248] = 1;
		if (!((now.global_state.modemanager.r<now.global_state.modemanager.threshold1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 568: // STATE 249 - system.pml:44 - [Actuator1_actuatorTc_channel!1] (0:0:0 - 1)
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
	case 569: // STATE 252 - system.pml:44 - [Actuator1_actuatorTc_channel!2] (0:0:0 - 1)
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
	case 570: // STATE 256 - modemanager.pml:68 - [((global_state.modemanager.r>global_state.modemanager.threshold2))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][256] = 1;
		if (!((now.global_state.modemanager.r>now.global_state.modemanager.threshold2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 571: // STATE 257 - system.pml:58 - [Actuator2_actuatorTc_channel!1] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[4][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 572: // STATE 260 - system.pml:58 - [Actuator2_actuatorTc_channel!2] (300:0:1 - 1)
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
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[4][265] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 573: // STATE 264 - modemanager.pml:73 - [transition_id = -(1)] (0:300:1 - 3)
		IfNotBlocked
		reached[4][264] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 265, 300) */
		reached[4][265] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 574: // STATE 266 - modemanager.pml:75 - [((transition_id==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][266] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 575: // STATE 270 - modemanager.pml:78 - [((transition_id==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][270] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 576: // STATE 273 - modemanager.pml:82 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[4][273] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 274, 300) */
		reached[4][274] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 275, 300) */
		reached[4][275] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 577: // STATE 276 - modemanager.pml:85 - [((((global_state.modemanager.status1==0)&&(global_state.modemanager.status2==0))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[4][276] = 1;
		if (!((((now.global_state.modemanager.status1==0)&&(now.global_state.modemanager.status2==0))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 277, 300) */
		reached[4][277] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
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
	case 578: // STATE 282 - modemanager.pml:90 - [((transition_id==6))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][282] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==6)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 579: // STATE 286 - modemanager.pml:93 - [((transition_id==7))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][286] = 1;
		if (!((((P4 *)_this)->_1062_5_6_22_transition_id==7)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1062_5_6_22_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_1062_5_6_22_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 580: // STATE 289 - modemanager.pml:97 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==0))] (300:0:1 - 1)
		IfNotBlocked
		reached[4][289] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==0)))
			continue;
		/* merge: transition_id = -(1)(0, 290, 300) */
		reached[4][290] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 291, 300) */
		reached[4][291] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 581: // STATE 292 - modemanager.pml:100 - [((((global_state.modemanager.status1==2)&&(global_state.modemanager.status2==2))==1))] (300:0:2 - 1)
		IfNotBlocked
		reached[4][292] = 1;
		if (!((((now.global_state.modemanager.status1==2)&&(now.global_state.modemanager.status2==2))==1)))
			continue;
		/* merge: transition_id = -(1)(300, 293, 300) */
		reached[4][293] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_1062_5_6_22_transition_id;
		((P4 *)_this)->_1062_5_6_22_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_actuatorTm1:transition_id", ((P4 *)_this)->_1062_5_6_22_transition_id);
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
	case 582: // STATE 311 - system.pml:188 - [(empty(Modemanager_actuatorTm1_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][311] = 1;
		if (!((q_len(now.Modemanager_actuatorTm1_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 583: // STATE 312 - system.pml:189 - [(1)] (315:0:0 - 1)
		IfNotBlocked
		reached[4][312] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 314, 315) */
		reached[4][314] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 584: // STATE 315 - system.pml:191 - [Modemanager_lock!1] (0:0:0 - 1)
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
	case 585: // STATE 320 - system.pml:194 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[4][320] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Egse_systemTc */
	case 586: // STATE 1 - system.pml:165 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[3][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 587: // STATE 2 - system.pml:167 - [] (0:0:0 - 1)
		IfNotBlocked
		reached[3][2] = 1;
		;
		_m = 3; goto P999; /* 0 */
	case 588: // STATE 4 - env_inlines.pml:31 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][4] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_16_Dummy_tmp;
		((P3 *)_this)->_1061_4_4_16_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1061_4_4_16_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 589: // STATE 5 - env_inlines.pml:33 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][5] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_16_Dummy_tmp;
		((P3 *)_this)->_1061_4_4_16_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1061_4_4_16_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 590: // STATE 8 - env_inlines.pml:33 - [goto :b12] (0:98:2 - 1)
		IfNotBlocked
		reached[3][8] = 1;
		;
		/* merge: value.data.goToIdle = Dummy_tmp(98, 15, 98) */
		reached[3][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToIdle;
		((P3 *)_this)->value.data.goToIdle = ((P3 *)_this)->_1061_4_4_16_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToIdle", ((P3 *)_this)->value.data.goToIdle);
#endif
		;
		/* merge: value.selection = 1(98, 17, 98) */
		reached[3][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 1;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 591: // STATE 6 - env_inlines.pml:33 - [((Dummy_tmp<0))] (9:0:1 - 1)
		IfNotBlocked
		reached[3][6] = 1;
		if (!((((P3 *)_this)->_1061_4_4_16_Dummy_tmp<0)))
			continue;
		/* merge: Dummy_tmp = (Dummy_tmp+1)(0, 7, 9) */
		reached[3][7] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_16_Dummy_tmp;
		((P3 *)_this)->_1061_4_4_16_Dummy_tmp = (((P3 *)_this)->_1061_4_4_16_Dummy_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1061_4_4_16_Dummy_tmp);
#endif
		;
		/* merge: .(goto)(0, 10, 9) */
		reached[3][10] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 592: // STATE 15 - env_inlines.pml:36 - [value.data.goToIdle = Dummy_tmp] (0:98:2 - 3)
		IfNotBlocked
		reached[3][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToIdle;
		((P3 *)_this)->value.data.goToIdle = ((P3 *)_this)->_1061_4_4_16_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToIdle", ((P3 *)_this)->value.data.goToIdle);
#endif
		;
		/* merge: value.selection = 1(98, 17, 98) */
		reached[3][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 1;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 593: // STATE 19 - env_inlines.pml:10 - [OptimizedDataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][19] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 594: // STATE 20 - env_inlines.pml:12 - [OptimizedDataItem_tmp = 10] (0:0:1 - 1)
		IfNotBlocked
		reached[3][20] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = 10;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 595: // STATE 23 - env_inlines.pml:12 - [goto :b13] (0:76:1 - 1)
		IfNotBlocked
		reached[3][23] = 1;
		;
		/* merge: value.data.goToOperational.threshold1 = OptimizedDataItem_tmp(76, 46, 76) */
		reached[3][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->value.data.goToOperational.threshold1;
		((P3 *)_this)->value.data.goToOperational.threshold1 = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold1", ((P3 *)_this)->value.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 596: // STATE 21 - env_inlines.pml:12 - [((OptimizedDataItem_tmp<10))] (24:0:1 - 1)
		IfNotBlocked
		reached[3][21] = 1;
		if (!((((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp<10)))
			continue;
		/* merge: OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)(0, 22, 24) */
		reached[3][22] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = (((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 25, 24) */
		reached[3][25] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 597: // STATE 28 - env_inlines.pml:15 - [OptimizedDataItem_tmp = 50] (0:0:1 - 1)
		IfNotBlocked
		reached[3][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = 50;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 598: // STATE 31 - env_inlines.pml:15 - [goto :b14] (0:76:1 - 1)
		IfNotBlocked
		reached[3][31] = 1;
		;
		/* merge: value.data.goToOperational.threshold1 = OptimizedDataItem_tmp(76, 46, 76) */
		reached[3][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->value.data.goToOperational.threshold1;
		((P3 *)_this)->value.data.goToOperational.threshold1 = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold1", ((P3 *)_this)->value.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 599: // STATE 29 - env_inlines.pml:15 - [((OptimizedDataItem_tmp<50))] (32:0:1 - 1)
		IfNotBlocked
		reached[3][29] = 1;
		if (!((((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp<50)))
			continue;
		/* merge: OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)(0, 30, 32) */
		reached[3][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = (((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 33, 32) */
		reached[3][33] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 600: // STATE 36 - env_inlines.pml:18 - [OptimizedDataItem_tmp = 100] (0:0:1 - 1)
		IfNotBlocked
		reached[3][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = 100;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 601: // STATE 39 - env_inlines.pml:18 - [goto :b15] (0:76:1 - 1)
		IfNotBlocked
		reached[3][39] = 1;
		;
		/* merge: value.data.goToOperational.threshold1 = OptimizedDataItem_tmp(76, 46, 76) */
		reached[3][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->value.data.goToOperational.threshold1;
		((P3 *)_this)->value.data.goToOperational.threshold1 = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold1", ((P3 *)_this)->value.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 602: // STATE 37 - env_inlines.pml:18 - [((OptimizedDataItem_tmp<100))] (40:0:1 - 1)
		IfNotBlocked
		reached[3][37] = 1;
		if (!((((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp<100)))
			continue;
		/* merge: OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)(0, 38, 40) */
		reached[3][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp = (((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 41, 40) */
		reached[3][41] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 603: // STATE 46 - env_inlines.pml:21 - [value.data.goToOperational.threshold1 = OptimizedDataItem_tmp] (0:76:1 - 7)
		IfNotBlocked
		reached[3][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->value.data.goToOperational.threshold1;
		((P3 *)_this)->value.data.goToOperational.threshold1 = ((P3 *)_this)->_1061_4_4_17_53_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold1", ((P3 *)_this)->value.data.goToOperational.threshold1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 604: // STATE 48 - env_inlines.pml:10 - [OptimizedDataItem_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 605: // STATE 49 - env_inlines.pml:12 - [OptimizedDataItem_tmp = 10] (0:0:1 - 1)
		IfNotBlocked
		reached[3][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = 10;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 606: // STATE 52 - env_inlines.pml:12 - [goto :b16] (0:98:2 - 1)
		IfNotBlocked
		reached[3][52] = 1;
		;
		/* merge: value.data.goToOperational.threshold2 = OptimizedDataItem_tmp(98, 75, 98) */
		reached[3][75] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToOperational.threshold2;
		((P3 *)_this)->value.data.goToOperational.threshold2 = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold2", ((P3 *)_this)->value.data.goToOperational.threshold2);
#endif
		;
		/* merge: value.selection = 2(98, 78, 98) */
		reached[3][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 607: // STATE 50 - env_inlines.pml:12 - [((OptimizedDataItem_tmp<10))] (53:0:1 - 1)
		IfNotBlocked
		reached[3][50] = 1;
		if (!((((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp<10)))
			continue;
		/* merge: OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)(0, 51, 53) */
		reached[3][51] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = (((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 54, 53) */
		reached[3][54] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 608: // STATE 57 - env_inlines.pml:15 - [OptimizedDataItem_tmp = 50] (0:0:1 - 1)
		IfNotBlocked
		reached[3][57] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = 50;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 609: // STATE 60 - env_inlines.pml:15 - [goto :b17] (0:98:2 - 1)
		IfNotBlocked
		reached[3][60] = 1;
		;
		/* merge: value.data.goToOperational.threshold2 = OptimizedDataItem_tmp(98, 75, 98) */
		reached[3][75] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToOperational.threshold2;
		((P3 *)_this)->value.data.goToOperational.threshold2 = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold2", ((P3 *)_this)->value.data.goToOperational.threshold2);
#endif
		;
		/* merge: value.selection = 2(98, 78, 98) */
		reached[3][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 610: // STATE 58 - env_inlines.pml:15 - [((OptimizedDataItem_tmp<50))] (61:0:1 - 1)
		IfNotBlocked
		reached[3][58] = 1;
		if (!((((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp<50)))
			continue;
		/* merge: OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)(0, 59, 61) */
		reached[3][59] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = (((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 62, 61) */
		reached[3][62] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 611: // STATE 65 - env_inlines.pml:18 - [OptimizedDataItem_tmp = 100] (0:0:1 - 1)
		IfNotBlocked
		reached[3][65] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = 100;
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 612: // STATE 68 - env_inlines.pml:18 - [goto :b18] (0:98:2 - 1)
		IfNotBlocked
		reached[3][68] = 1;
		;
		/* merge: value.data.goToOperational.threshold2 = OptimizedDataItem_tmp(98, 75, 98) */
		reached[3][75] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToOperational.threshold2;
		((P3 *)_this)->value.data.goToOperational.threshold2 = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold2", ((P3 *)_this)->value.data.goToOperational.threshold2);
#endif
		;
		/* merge: value.selection = 2(98, 78, 98) */
		reached[3][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 613: // STATE 66 - env_inlines.pml:18 - [((OptimizedDataItem_tmp<100))] (69:0:1 - 1)
		IfNotBlocked
		reached[3][66] = 1;
		if (!((((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp<100)))
			continue;
		/* merge: OptimizedDataItem_tmp = (OptimizedDataItem_tmp+1)(0, 67, 69) */
		reached[3][67] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
		((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp = (((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:OptimizedDataItem_tmp", ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp);
#endif
		;
		/* merge: .(goto)(0, 70, 69) */
		reached[3][70] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 614: // STATE 75 - env_inlines.pml:21 - [value.data.goToOperational.threshold2 = OptimizedDataItem_tmp] (0:98:2 - 7)
		IfNotBlocked
		reached[3][75] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.goToOperational.threshold2;
		((P3 *)_this)->value.data.goToOperational.threshold2 = ((P3 *)_this)->_1061_4_4_17_54_OptimizedDataItem_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.goToOperational.threshold2", ((P3 *)_this)->value.data.goToOperational.threshold2);
#endif
		;
		/* merge: value.selection = 2(98, 78, 98) */
		reached[3][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 615: // STATE 80 - env_inlines.pml:31 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_18_Dummy_tmp;
		((P3 *)_this)->_1061_4_4_18_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1061_4_4_18_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 616: // STATE 81 - env_inlines.pml:33 - [Dummy_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[3][81] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_18_Dummy_tmp;
		((P3 *)_this)->_1061_4_4_18_Dummy_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1061_4_4_18_Dummy_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 617: // STATE 84 - env_inlines.pml:33 - [goto :b19] (0:98:2 - 1)
		IfNotBlocked
		reached[3][84] = 1;
		;
		/* merge: value.data.doReport = Dummy_tmp(98, 91, 98) */
		reached[3][91] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.doReport;
		((P3 *)_this)->value.data.doReport = ((P3 *)_this)->_1061_4_4_18_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.doReport", ((P3 *)_this)->value.data.doReport);
#endif
		;
		/* merge: value.selection = 3(98, 93, 98) */
		reached[3][93] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 3;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 618: // STATE 82 - env_inlines.pml:33 - [((Dummy_tmp<0))] (85:0:1 - 1)
		IfNotBlocked
		reached[3][82] = 1;
		if (!((((P3 *)_this)->_1061_4_4_18_Dummy_tmp<0)))
			continue;
		/* merge: Dummy_tmp = (Dummy_tmp+1)(0, 83, 85) */
		reached[3][83] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_1061_4_4_18_Dummy_tmp;
		((P3 *)_this)->_1061_4_4_18_Dummy_tmp = (((P3 *)_this)->_1061_4_4_18_Dummy_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_systemTc:Dummy_tmp", ((P3 *)_this)->_1061_4_4_18_Dummy_tmp);
#endif
		;
		/* merge: .(goto)(0, 86, 85) */
		reached[3][86] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 619: // STATE 91 - env_inlines.pml:36 - [value.data.doReport = Dummy_tmp] (0:98:2 - 3)
		IfNotBlocked
		reached[3][91] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->value.data.doReport;
		((P3 *)_this)->value.data.doReport = ((P3 *)_this)->_1061_4_4_18_Dummy_tmp;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.data.doReport", ((P3 *)_this)->value.data.doReport);
#endif
		;
		/* merge: value.selection = 3(98, 93, 98) */
		reached[3][93] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->value.selection;
		((P3 *)_this)->value.selection = 3;
#ifdef VAR_RANGES
		logval("Egse_systemTc:value.selection", ((P3 *)_this)->value.selection);
#endif
		;
		/* merge: .(goto)(0, 95, 98) */
		reached[3][95] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 620: // STATE 97 - system.pml:88 - [Modemanager_systemTc_channel!value.data.goToIdle,value.data.goToOperational.threshold1,value.data.goToOperational.threshold2,value.data.doReport,value.selection] (0:0:0 - 1)
		IfNotBlocked
		reached[3][97] = 1;
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
		_m = 2; goto P999; /* 0 */
	case 621: // STATE 103 - system.pml:173 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[3][103] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Egse_systemTm */
	case 622: // STATE 1 - system.pml:144 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[2][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 623: // STATE 2 - system.pml:147 - [(nempty(Egse_systemTm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][2] = 1;
		if (!((q_len(now.Egse_systemTm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 624: // STATE 3 - system.pml:150 - [(nempty(Egse_systemTm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][3] = 1;
		if (!((q_len(now.Egse_systemTm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 625: // STATE 4 - system.pml:151 - [Egse_systemTm_channel?Egse_systemtm_signal_parameter.reading,Egse_systemtm_signal_parameter.status1,Egse_systemtm_signal_parameter.status2] (6:0:4 - 1)
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
		/* merge: Egse_systemtm_channel_used = 1(0, 5, 6) */
		reached[2][5] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = ((int)Egse_systemtm_channel_used);
		Egse_systemtm_channel_used = 1;
#ifdef VAR_RANGES
		logval("Egse_systemtm_channel_used", ((int)Egse_systemtm_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 626: // STATE 6 - system.pml:153 - [Untitled_msc_lock?_] (0:0:1 - 1)
		reached[2][6] = 1;
		if (q_len(now.Untitled_msc_lock) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.oval = qrecv(now.Untitled_msc_lock, XX-1, 0, 0);
		;
		qrecv(now.Untitled_msc_lock, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Untitled_msc_lock);
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 627: // STATE 7 - Untitled_msc.pml:67 - [((global_state.untitled_msc.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][7] = 1;
		if (!((now.global_state.untitled_msc.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 628: // STATE 8 - Untitled_msc.pml:6 - [transition_id = 0] (0:43:2 - 1)
		IfNotBlocked
		reached[2][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: transition_id = 2(43, 9, 43) */
		reached[2][9] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id = 2;
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: .(goto)(0, 44, 43) */
		reached[2][44] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 629: // STATE 10 - Untitled_msc.pml:9 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][10] = 1;
		if (!((((P2 *)_this)->_1060_3_3_13_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_13_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1060_3_3_13_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_13_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 630: // STATE 12 - Untitled_msc.pml:11 - [((transition_id==0))] (42:0:3 - 1)
		IfNotBlocked
		reached[2][12] = 1;
		if (!((((P2 *)_this)->_1060_3_3_13_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_13_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_13_transition_id = 0;
		/* merge: transition_id = -(1)(42, 13, 42) */
		reached[2][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(42, 14, 42) */
		reached[2][14] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 15, 42) */
		reached[2][15] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 631: // STATE 16 - Untitled_msc.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][16] = 1;
		if (!((((P2 *)_this)->_1060_3_3_13_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_13_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1060_3_3_13_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_13_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 632: // STATE 17 - Untitled_msc.pml:17 - [((Untitled_MSC_0_sig0_param0.selection==3))] (42:0:2 - 1)
		IfNotBlocked
		reached[2][17] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==3)))
			continue;
		/* merge: transition_id = -(1)(42, 18, 42) */
		reached[2][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 2(42, 19, 42) */
		reached[2][19] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 2;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 20, 42) */
		reached[2][20] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 633: // STATE 22 - Untitled_msc.pml:22 - [(1)] (42:0:2 - 1)
		IfNotBlocked
		reached[2][22] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(42, 24, 42) */
		reached[2][24] = 1;
		;
		/* merge: transition_id = -(1)(42, 25, 42) */
		reached[2][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(42, 26, 42) */
		reached[2][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 27, 42) */
		reached[2][27] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 634: // STATE 25 - Untitled_msc.pml:24 - [transition_id = -(1)] (0:42:2 - 2)
		IfNotBlocked
		reached[2][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(42, 26, 42) */
		reached[2][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 27, 42) */
		reached[2][27] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 635: // STATE 28 - Untitled_msc.pml:27 - [((transition_id==2))] (42:0:3 - 1)
		IfNotBlocked
		reached[2][28] = 1;
		if (!((((P2 *)_this)->_1060_3_3_13_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_13_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_13_transition_id = 0;
		/* merge: transition_id = -(1)(42, 29, 42) */
		reached[2][29] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(42, 30, 42) */
		reached[2][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 31, 42) */
		reached[2][31] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 636: // STATE 32 - Untitled_msc.pml:31 - [((transition_id==3))] (42:0:3 - 1)
		IfNotBlocked
		reached[2][32] = 1;
		if (!((((P2 *)_this)->_1060_3_3_13_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_13_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_13_transition_id = 0;
		/* merge: transition_id = -(1)(42, 33, 42) */
		reached[2][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 1(42, 34, 42) */
		reached[2][34] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 1;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 35, 42) */
		reached[2][35] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 637: // STATE 36 - Untitled_msc.pml:35 - [((transition_id==4))] (42:0:3 - 1)
		IfNotBlocked
		reached[2][36] = 1;
		if (!((((P2 *)_this)->_1060_3_3_13_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_13_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_13_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_13_transition_id = 0;
		/* merge: transition_id = -(1)(42, 37, 42) */
		reached[2][37] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_13_transition_id;
		((P2 *)_this)->_1060_3_3_13_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_13_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 3(42, 38, 42) */
		reached[2][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 3;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 39, 42) */
		reached[2][39] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 638: // STATE 47 - Untitled_msc.pml:69 - [((global_state.untitled_msc.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][47] = 1;
		if (!((now.global_state.untitled_msc.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 639: // STATE 48 - dataview.pml:9082 - [Untitled_MSC_0_sig1_param0.reading = Egse_systemtm_signal_parameter.reading] (0:0:1 - 1)
		IfNotBlocked
		reached[2][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig1_param0.reading;
		now.Untitled_MSC_0_sig1_param0.reading = now.Egse_systemtm_signal_parameter.reading;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig1_param0.reading", now.Untitled_MSC_0_sig1_param0.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 640: // STATE 49 - dataview.pml:9087 - [assert(((Untitled_MSC_0_sig1_param0.reading>=0)&&(Untitled_MSC_0_sig1_param0.reading<=255)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.Untitled_MSC_0_sig1_param0.reading>=0)&&(now.Untitled_MSC_0_sig1_param0.reading<=255)), "((Untitled_MSC_0_sig1_param0.reading>=0)&&(Untitled_MSC_0_sig1_param0.reading<=255))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 641: // STATE 52 - dataview.pml:9091 - [Untitled_MSC_0_sig1_param0.status1 = Egse_systemtm_signal_parameter.status1] (0:0:1 - 1)
		IfNotBlocked
		reached[2][52] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig1_param0.status1;
		now.Untitled_MSC_0_sig1_param0.status1 = now.Egse_systemtm_signal_parameter.status1;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig1_param0.status1", now.Untitled_MSC_0_sig1_param0.status1);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 642: // STATE 53 - dataview.pml:9096 - [assert((((Untitled_MSC_0_sig1_param0.status1==0)||(Untitled_MSC_0_sig1_param0.status1==1))||(Untitled_MSC_0_sig1_param0.status1==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][53] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.Untitled_MSC_0_sig1_param0.status1==0)||(now.Untitled_MSC_0_sig1_param0.status1==1))||(now.Untitled_MSC_0_sig1_param0.status1==2)), "(((Untitled_MSC_0_sig1_param0.status1==0)||(Untitled_MSC_0_sig1_param0.status1==1))||(Untitled_MSC_0_sig1_param0.status1==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 643: // STATE 56 - dataview.pml:9100 - [Untitled_MSC_0_sig1_param0.status2 = Egse_systemtm_signal_parameter.status2] (0:0:1 - 1)
		IfNotBlocked
		reached[2][56] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.Untitled_MSC_0_sig1_param0.status2;
		now.Untitled_MSC_0_sig1_param0.status2 = now.Egse_systemtm_signal_parameter.status2;
#ifdef VAR_RANGES
		logval("Untitled_MSC_0_sig1_param0.status2", now.Untitled_MSC_0_sig1_param0.status2);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 644: // STATE 57 - dataview.pml:9105 - [assert((((Untitled_MSC_0_sig1_param0.status2==0)||(Untitled_MSC_0_sig1_param0.status2==1))||(Untitled_MSC_0_sig1_param0.status2==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][57] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.Untitled_MSC_0_sig1_param0.status2==0)||(now.Untitled_MSC_0_sig1_param0.status2==1))||(now.Untitled_MSC_0_sig1_param0.status2==2)), "(((Untitled_MSC_0_sig1_param0.status2==0)||(Untitled_MSC_0_sig1_param0.status2==1))||(Untitled_MSC_0_sig1_param0.status2==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 645: // STATE 61 - Untitled_msc.pml:6 - [transition_id = 0] (0:96:2 - 1)
		IfNotBlocked
		reached[2][61] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id = 0;
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: transition_id = 3(96, 62, 96) */
		reached[2][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id = 3;
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: .(goto)(0, 97, 96) */
		reached[2][97] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 646: // STATE 63 - Untitled_msc.pml:9 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][63] = 1;
		if (!((((P2 *)_this)->_1060_3_3_15_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_15_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1060_3_3_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_15_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 647: // STATE 65 - Untitled_msc.pml:11 - [((transition_id==0))] (95:0:3 - 1)
		IfNotBlocked
		reached[2][65] = 1;
		if (!((((P2 *)_this)->_1060_3_3_15_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_15_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_15_transition_id = 0;
		/* merge: transition_id = -(1)(95, 66, 95) */
		reached[2][66] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(95, 67, 95) */
		reached[2][67] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 68, 95) */
		reached[2][68] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 648: // STATE 69 - Untitled_msc.pml:15 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[2][69] = 1;
		if (!((((P2 *)_this)->_1060_3_3_15_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_15_transition_id */  (trpt+1)->bup.oval = ((P2 *)_this)->_1060_3_3_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_15_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 649: // STATE 70 - Untitled_msc.pml:17 - [((Untitled_MSC_0_sig0_param0.selection==3))] (95:0:2 - 1)
		IfNotBlocked
		reached[2][70] = 1;
		if (!((now.Untitled_MSC_0_sig0_param0.selection==3)))
			continue;
		/* merge: transition_id = -(1)(95, 71, 95) */
		reached[2][71] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 2(95, 72, 95) */
		reached[2][72] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 2;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 73, 95) */
		reached[2][73] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 650: // STATE 75 - Untitled_msc.pml:22 - [(1)] (95:0:2 - 1)
		IfNotBlocked
		reached[2][75] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(95, 77, 95) */
		reached[2][77] = 1;
		;
		/* merge: transition_id = -(1)(95, 78, 95) */
		reached[2][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(95, 79, 95) */
		reached[2][79] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 80, 95) */
		reached[2][80] = 1;
		;
		_m = 3; goto P999; /* 4 */
	case 651: // STATE 78 - Untitled_msc.pml:24 - [transition_id = -(1)] (0:95:2 - 2)
		IfNotBlocked
		reached[2][78] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(95, 79, 95) */
		reached[2][79] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 80, 95) */
		reached[2][80] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 652: // STATE 81 - Untitled_msc.pml:27 - [((transition_id==2))] (95:0:3 - 1)
		IfNotBlocked
		reached[2][81] = 1;
		if (!((((P2 *)_this)->_1060_3_3_15_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_15_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_15_transition_id = 0;
		/* merge: transition_id = -(1)(95, 82, 95) */
		reached[2][82] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 0(95, 83, 95) */
		reached[2][83] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 0;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 84, 95) */
		reached[2][84] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 653: // STATE 85 - Untitled_msc.pml:31 - [((transition_id==3))] (95:0:3 - 1)
		IfNotBlocked
		reached[2][85] = 1;
		if (!((((P2 *)_this)->_1060_3_3_15_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_15_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_15_transition_id = 0;
		/* merge: transition_id = -(1)(95, 86, 95) */
		reached[2][86] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 1(95, 87, 95) */
		reached[2][87] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 1;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 88, 95) */
		reached[2][88] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 654: // STATE 89 - Untitled_msc.pml:35 - [((transition_id==4))] (95:0:3 - 1)
		IfNotBlocked
		reached[2][89] = 1;
		if (!((((P2 *)_this)->_1060_3_3_15_transition_id==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1060_3_3_15_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->_1060_3_3_15_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P2 *)_this)->_1060_3_3_15_transition_id = 0;
		/* merge: transition_id = -(1)(95, 90, 95) */
		reached[2][90] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->_1060_3_3_15_transition_id;
		((P2 *)_this)->_1060_3_3_15_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Egse_systemTm:transition_id", ((P2 *)_this)->_1060_3_3_15_transition_id);
#endif
		;
		/* merge: global_state.untitled_msc.state = 3(95, 91, 95) */
		reached[2][91] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.untitled_msc.state;
		now.global_state.untitled_msc.state = 3;
#ifdef VAR_RANGES
		logval("global_state.untitled_msc.state", now.global_state.untitled_msc.state);
#endif
		;
		/* merge: goto continuous_signals(0, 92, 95) */
		reached[2][92] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 655: // STATE 105 - system.pml:155 - [Untitled_msc_lock!1] (0:0:0 - 8)
		IfNotBlocked
		reached[2][105] = 1;
		if (q_full(now.Untitled_msc_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Untitled_msc_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Untitled_msc_lock, 0, 1, 0, 0, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 656: // STATE 107 - system.pml:157 - [(empty(Egse_systemTm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][107] = 1;
		if (!((q_len(now.Egse_systemTm_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 657: // STATE 108 - system.pml:158 - [(1)] (112:0:0 - 1)
		IfNotBlocked
		reached[2][108] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 110, 112) */
		reached[2][110] = 1;
		;
		/* merge: .(goto)(0, 113, 112) */
		reached[2][113] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 658: // STATE 115 - system.pml:162 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[2][115] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Actuator2_actuatorTc */
	case 659: // STATE 1 - system.pml:123 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[1][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 660: // STATE 2 - system.pml:126 - [(nempty(Actuator2_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][2] = 1;
		if (!((q_len(now.Actuator2_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 661: // STATE 3 - system.pml:127 - [Actuator2_lock?_] (0:0:1 - 1)
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
	case 662: // STATE 4 - system.pml:130 - [(nempty(Actuator2_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][4] = 1;
		if (!((q_len(now.Actuator2_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 663: // STATE 5 - system.pml:131 - [Actuator2_actuatorTc_channel?Actuator2_actuatortc_signal_parameter] (215:0:2 - 1)
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
	case 664: // STATE 7 - actuator2.pml:66 - [((global_state.actuator2.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][7] = 1;
		if (!((now.global_state.actuator2.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 665: // STATE 8 - dataview.pml:8982 - [global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter] (0:0:1 - 1)
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
	case 666: // STATE 9 - dataview.pml:8987 - [assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator2.cmd==0)||(now.global_state.actuator2.cmd==1))||(now.global_state.actuator2.cmd==2)), "(((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 667: // STATE 12 - actuator2.pml:4 - [transition_id = 0] (0:71:2 - 1)
		IfNotBlocked
		reached[1][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
#endif
		;
		/* merge: transition_id = 1(71, 13, 71) */
		reached[1][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
#endif
		;
		/* merge: .(goto)(0, 72, 71) */
		reached[1][72] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 668: // STATE 14 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][14] = 1;
		if (!((((P1 *)_this)->_1059_2_2_8_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 669: // STATE 16 - actuator2.pml:9 - [((transition_id==0))] (70:0:3 - 1)
		IfNotBlocked
		reached[1][16] = 1;
		if (!((((P1 *)_this)->_1059_2_2_8_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_8_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_8_transition_id = 0;
		/* merge: transition_id = -(1)(70, 17, 70) */
		reached[1][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
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
	case 670: // STATE 20 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][20] = 1;
		if (!((((P1 *)_this)->_1059_2_2_8_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 671: // STATE 23 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][23] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 672: // STATE 24 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
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
	case 673: // STATE 29 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][29] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 674: // STATE 30 - system.pml:80 - [Modemanager_actuatorTm2_channel!1] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
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
	case 675: // STATE 36 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 676: // STATE 40 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][40] = 1;
		if (!((((P1 *)_this)->_1059_2_2_8_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 677: // STATE 43 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][43] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 678: // STATE 44 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
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
	case 679: // STATE 50 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 680: // STATE 54 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][54] = 1;
		if (!((((P1 *)_this)->_1059_2_2_8_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_8_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 681: // STATE 57 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][57] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 682: // STATE 58 - system.pml:80 - [Modemanager_actuatorTm2_channel!0] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
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
	case 683: // STATE 64 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_8_transition_id;
		((P1 *)_this)->_1059_2_2_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_8_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 684: // STATE 75 - actuator2.pml:69 - [((global_state.actuator2.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][75] = 1;
		if (!((now.global_state.actuator2.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 685: // STATE 76 - dataview.pml:8982 - [global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter] (0:0:1 - 1)
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
	case 686: // STATE 77 - dataview.pml:8987 - [assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][77] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator2.cmd==0)||(now.global_state.actuator2.cmd==1))||(now.global_state.actuator2.cmd==2)), "(((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 687: // STATE 80 - actuator2.pml:4 - [transition_id = 0] (0:139:2 - 1)
		IfNotBlocked
		reached[1][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
#endif
		;
		/* merge: transition_id = 2(139, 81, 139) */
		reached[1][81] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id = 2;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
#endif
		;
		/* merge: .(goto)(0, 140, 139) */
		reached[1][140] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 688: // STATE 82 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][82] = 1;
		if (!((((P1 *)_this)->_1059_2_2_10_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 689: // STATE 84 - actuator2.pml:9 - [((transition_id==0))] (138:0:3 - 1)
		IfNotBlocked
		reached[1][84] = 1;
		if (!((((P1 *)_this)->_1059_2_2_10_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_10_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_10_transition_id = 0;
		/* merge: transition_id = -(1)(138, 85, 138) */
		reached[1][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
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
	case 690: // STATE 88 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][88] = 1;
		if (!((((P1 *)_this)->_1059_2_2_10_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 691: // STATE 91 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][91] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 692: // STATE 92 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
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
	case 693: // STATE 97 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][97] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 694: // STATE 98 - system.pml:80 - [Modemanager_actuatorTm2_channel!1] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
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
	case 695: // STATE 104 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][104] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 696: // STATE 108 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][108] = 1;
		if (!((((P1 *)_this)->_1059_2_2_10_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 697: // STATE 111 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][111] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 698: // STATE 112 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
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
	case 699: // STATE 118 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][118] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 700: // STATE 122 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][122] = 1;
		if (!((((P1 *)_this)->_1059_2_2_10_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_10_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 701: // STATE 125 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][125] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 702: // STATE 126 - system.pml:80 - [Modemanager_actuatorTm2_channel!0] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
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
	case 703: // STATE 132 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][132] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_10_transition_id;
		((P1 *)_this)->_1059_2_2_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_10_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 704: // STATE 143 - actuator2.pml:72 - [((global_state.actuator2.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][143] = 1;
		if (!((now.global_state.actuator2.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 705: // STATE 144 - dataview.pml:8982 - [global_state.actuator2.cmd = Actuator2_actuatortc_signal_parameter] (0:0:1 - 1)
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
	case 706: // STATE 145 - dataview.pml:8987 - [assert((((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][145] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator2.cmd==0)||(now.global_state.actuator2.cmd==1))||(now.global_state.actuator2.cmd==2)), "(((global_state.actuator2.cmd==0)||(global_state.actuator2.cmd==1))||(global_state.actuator2.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 707: // STATE 148 - actuator2.pml:4 - [transition_id = 0] (0:207:2 - 1)
		IfNotBlocked
		reached[1][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
#endif
		;
		/* merge: transition_id = 3(207, 149, 207) */
		reached[1][149] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id = 3;
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
#endif
		;
		/* merge: .(goto)(0, 208, 207) */
		reached[1][208] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 708: // STATE 150 - actuator2.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][150] = 1;
		if (!((((P1 *)_this)->_1059_2_2_12_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 709: // STATE 152 - actuator2.pml:9 - [((transition_id==0))] (206:0:3 - 1)
		IfNotBlocked
		reached[1][152] = 1;
		if (!((((P1 *)_this)->_1059_2_2_12_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_12_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_12_transition_id = 0;
		/* merge: transition_id = -(1)(206, 153, 206) */
		reached[1][153] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
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
	case 710: // STATE 156 - actuator2.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][156] = 1;
		if (!((((P1 *)_this)->_1059_2_2_12_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 711: // STATE 159 - actuator2.pml:16 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][159] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 712: // STATE 160 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
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
	case 713: // STATE 165 - actuator2.pml:21 - [((global_state.actuator2.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][165] = 1;
		if (!((now.global_state.actuator2.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 714: // STATE 166 - system.pml:80 - [Modemanager_actuatorTm2_channel!1] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
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
	case 715: // STATE 172 - actuator2.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][172] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 716: // STATE 176 - actuator2.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][176] = 1;
		if (!((((P1 *)_this)->_1059_2_2_12_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 717: // STATE 179 - actuator2.pml:33 - [((global_state.actuator2.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][179] = 1;
		if (!((now.global_state.actuator2.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 718: // STATE 180 - system.pml:80 - [Modemanager_actuatorTm2_channel!2] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
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
	case 719: // STATE 186 - actuator2.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 720: // STATE 190 - actuator2.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][190] = 1;
		if (!((((P1 *)_this)->_1059_2_2_12_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1059_2_2_12_transition_id */  (trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->_1059_2_2_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 721: // STATE 193 - actuator2.pml:45 - [((global_state.actuator2.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][193] = 1;
		if (!((now.global_state.actuator2.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 722: // STATE 194 - system.pml:80 - [Modemanager_actuatorTm2_channel!0] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
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
	case 723: // STATE 200 - actuator2.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[1][200] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P1 *)_this)->_1059_2_2_12_transition_id;
		((P1 *)_this)->_1059_2_2_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator2_actuatorTc:transition_id", ((P1 *)_this)->_1059_2_2_12_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 724: // STATE 217 - system.pml:135 - [(empty(Actuator2_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][217] = 1;
		if (!((q_len(now.Actuator2_actuatorTc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 725: // STATE 218 - system.pml:136 - [(1)] (221:0:0 - 1)
		IfNotBlocked
		reached[1][218] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 220, 221) */
		reached[1][220] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 726: // STATE 221 - system.pml:138 - [Actuator2_lock!1] (0:0:0 - 1)
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
	case 727: // STATE 226 - system.pml:141 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[1][226] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Actuator1_actuatorTc */
	case 728: // STATE 1 - system.pml:102 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[0][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 729: // STATE 2 - system.pml:105 - [(nempty(Actuator1_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][2] = 1;
		if (!((q_len(now.Actuator1_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 730: // STATE 3 - system.pml:106 - [Actuator1_lock?_] (0:0:1 - 1)
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
	case 731: // STATE 4 - system.pml:109 - [(nempty(Actuator1_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][4] = 1;
		if (!((q_len(now.Actuator1_actuatorTc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 732: // STATE 5 - system.pml:110 - [Actuator1_actuatorTc_channel?Actuator1_actuatortc_signal_parameter] (215:0:2 - 1)
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
	case 733: // STATE 7 - actuator1.pml:66 - [((global_state.actuator1.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][7] = 1;
		if (!((now.global_state.actuator1.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 734: // STATE 8 - dataview.pml:8982 - [global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter] (0:0:1 - 1)
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
	case 735: // STATE 9 - dataview.pml:8987 - [assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator1.cmd==0)||(now.global_state.actuator1.cmd==1))||(now.global_state.actuator1.cmd==2)), "(((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 736: // STATE 12 - actuator1.pml:4 - [transition_id = 0] (0:71:2 - 1)
		IfNotBlocked
		reached[0][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
#endif
		;
		/* merge: transition_id = 1(71, 13, 71) */
		reached[0][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
#endif
		;
		/* merge: .(goto)(0, 72, 71) */
		reached[0][72] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 737: // STATE 14 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][14] = 1;
		if (!((((P0 *)_this)->_1058_1_1_2_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 738: // STATE 16 - actuator1.pml:9 - [((transition_id==0))] (70:0:3 - 1)
		IfNotBlocked
		reached[0][16] = 1;
		if (!((((P0 *)_this)->_1058_1_1_2_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_2_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_2_transition_id = 0;
		/* merge: transition_id = -(1)(70, 17, 70) */
		reached[0][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
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
	case 739: // STATE 20 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][20] = 1;
		if (!((((P0 *)_this)->_1058_1_1_2_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 740: // STATE 23 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][23] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 741: // STATE 24 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
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
	case 742: // STATE 29 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][29] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 743: // STATE 30 - system.pml:76 - [Modemanager_actuatorTm1_channel!1] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
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
	case 744: // STATE 36 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 745: // STATE 40 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][40] = 1;
		if (!((((P0 *)_this)->_1058_1_1_2_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 746: // STATE 43 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][43] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 747: // STATE 44 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
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
	case 748: // STATE 50 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 749: // STATE 54 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][54] = 1;
		if (!((((P0 *)_this)->_1058_1_1_2_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 750: // STATE 57 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][57] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 751: // STATE 58 - system.pml:76 - [Modemanager_actuatorTm1_channel!0] (70:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
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
	case 752: // STATE 64 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_2_transition_id;
		((P0 *)_this)->_1058_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_2_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 753: // STATE 75 - actuator1.pml:69 - [((global_state.actuator1.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][75] = 1;
		if (!((now.global_state.actuator1.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 754: // STATE 76 - dataview.pml:8982 - [global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter] (0:0:1 - 1)
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
	case 755: // STATE 77 - dataview.pml:8987 - [assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][77] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator1.cmd==0)||(now.global_state.actuator1.cmd==1))||(now.global_state.actuator1.cmd==2)), "(((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 756: // STATE 80 - actuator1.pml:4 - [transition_id = 0] (0:139:2 - 1)
		IfNotBlocked
		reached[0][80] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
#endif
		;
		/* merge: transition_id = 2(139, 81, 139) */
		reached[0][81] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id = 2;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
#endif
		;
		/* merge: .(goto)(0, 140, 139) */
		reached[0][140] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 757: // STATE 82 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][82] = 1;
		if (!((((P0 *)_this)->_1058_1_1_4_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 758: // STATE 84 - actuator1.pml:9 - [((transition_id==0))] (138:0:3 - 1)
		IfNotBlocked
		reached[0][84] = 1;
		if (!((((P0 *)_this)->_1058_1_1_4_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_4_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_4_transition_id = 0;
		/* merge: transition_id = -(1)(138, 85, 138) */
		reached[0][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
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
	case 759: // STATE 88 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][88] = 1;
		if (!((((P0 *)_this)->_1058_1_1_4_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 760: // STATE 91 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][91] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 761: // STATE 92 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
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
	case 762: // STATE 97 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][97] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 763: // STATE 98 - system.pml:76 - [Modemanager_actuatorTm1_channel!1] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
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
	case 764: // STATE 104 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][104] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 765: // STATE 108 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][108] = 1;
		if (!((((P0 *)_this)->_1058_1_1_4_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 766: // STATE 111 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][111] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 767: // STATE 112 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
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
	case 768: // STATE 118 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][118] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 769: // STATE 122 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][122] = 1;
		if (!((((P0 *)_this)->_1058_1_1_4_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 770: // STATE 125 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][125] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 771: // STATE 126 - system.pml:76 - [Modemanager_actuatorTm1_channel!0] (138:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
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
	case 772: // STATE 132 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][132] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_4_transition_id;
		((P0 *)_this)->_1058_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_4_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 773: // STATE 143 - actuator1.pml:72 - [((global_state.actuator1.state==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][143] = 1;
		if (!((now.global_state.actuator1.state==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 774: // STATE 144 - dataview.pml:8982 - [global_state.actuator1.cmd = Actuator1_actuatortc_signal_parameter] (0:0:1 - 1)
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
	case 775: // STATE 145 - dataview.pml:8987 - [assert((((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][145] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((now.global_state.actuator1.cmd==0)||(now.global_state.actuator1.cmd==1))||(now.global_state.actuator1.cmd==2)), "(((global_state.actuator1.cmd==0)||(global_state.actuator1.cmd==1))||(global_state.actuator1.cmd==2))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 776: // STATE 148 - actuator1.pml:4 - [transition_id = 0] (0:207:2 - 1)
		IfNotBlocked
		reached[0][148] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
#endif
		;
		/* merge: transition_id = 3(207, 149, 207) */
		reached[0][149] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id = 3;
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
#endif
		;
		/* merge: .(goto)(0, 208, 207) */
		reached[0][208] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 777: // STATE 150 - actuator1.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][150] = 1;
		if (!((((P0 *)_this)->_1058_1_1_6_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 778: // STATE 152 - actuator1.pml:9 - [((transition_id==0))] (206:0:3 - 1)
		IfNotBlocked
		reached[0][152] = 1;
		if (!((((P0 *)_this)->_1058_1_1_6_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_6_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_6_transition_id = 0;
		/* merge: transition_id = -(1)(206, 153, 206) */
		reached[0][153] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
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
	case 779: // STATE 156 - actuator1.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][156] = 1;
		if (!((((P0 *)_this)->_1058_1_1_6_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 780: // STATE 159 - actuator1.pml:16 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][159] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 781: // STATE 160 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
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
	case 782: // STATE 165 - actuator1.pml:21 - [((global_state.actuator1.cmd==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][165] = 1;
		if (!((now.global_state.actuator1.cmd==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 783: // STATE 166 - system.pml:76 - [Modemanager_actuatorTm1_channel!1] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
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
	case 784: // STATE 172 - actuator1.pml:27 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][172] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 785: // STATE 176 - actuator1.pml:30 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][176] = 1;
		if (!((((P0 *)_this)->_1058_1_1_6_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 786: // STATE 179 - actuator1.pml:33 - [((global_state.actuator1.cmd==2))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][179] = 1;
		if (!((now.global_state.actuator1.cmd==2)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 787: // STATE 180 - system.pml:76 - [Modemanager_actuatorTm1_channel!2] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
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
	case 788: // STATE 186 - actuator1.pml:39 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][186] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 789: // STATE 190 - actuator1.pml:42 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][190] = 1;
		if (!((((P0 *)_this)->_1058_1_1_6_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _1058_1_1_6_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_1058_1_1_6_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 790: // STATE 193 - actuator1.pml:45 - [((global_state.actuator1.cmd==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][193] = 1;
		if (!((now.global_state.actuator1.cmd==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 791: // STATE 194 - system.pml:76 - [Modemanager_actuatorTm1_channel!0] (206:0:2 - 1)
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
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
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
	case 792: // STATE 200 - actuator1.pml:51 - [transition_id = -(1)] (0:0:1 - 1)
		IfNotBlocked
		reached[0][200] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P0 *)_this)->_1058_1_1_6_transition_id;
		((P0 *)_this)->_1058_1_1_6_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator1_actuatorTc:transition_id", ((P0 *)_this)->_1058_1_1_6_transition_id);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 793: // STATE 217 - system.pml:114 - [(empty(Actuator1_actuatorTc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][217] = 1;
		if (!((q_len(now.Actuator1_actuatorTc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 794: // STATE 218 - system.pml:115 - [(1)] (221:0:0 - 1)
		IfNotBlocked
		reached[0][218] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 220, 221) */
		reached[0][220] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 795: // STATE 221 - system.pml:117 - [Actuator1_lock!1] (0:0:0 - 1)
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
	case 796: // STATE 226 - system.pml:120 - [-end-] (0:0:0 - 1)
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

