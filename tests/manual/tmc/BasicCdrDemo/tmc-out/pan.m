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
		reached[8][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 4: // STATE 8 - scl.pml:12 - [((global_state.actuator.x>5))] (14:0:0 - 1)
		
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
		reached[8][8] = 1;
		if (!((now.global_state.actuator.x>5)))
			continue;
		/* merge: printf('SCL violation: never actuator.x  > 5;\\n')(14, 9, 14) */
		reached[8][9] = 1;
		if (TstOnly) return 1; /* T4 */
		Printf("SCL violation: never actuator.x  > 5;\n");
		/* merge: assert(!((global_state.actuator.x>5)))(14, 10, 14) */
		reached[8][10] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert( !((now.global_state.actuator.x>5)), " !((global_state.actuator.x>5))", II, tt, t);
		/* merge: .(goto)(0, 15, 14) */
		reached[8][15] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 5: // STATE 17 - scl.pml:19 - [-end-] (0:0:0 - 1)
		
#if defined(VERI) && !defined(NP)
#if NCLAIMS>1
		{	static int reported17 = 0;
			if (verbose && !reported17)
			{	int nn = (int) ((Pclaim *)pptr(0))->_n;
				printf("depth %ld: Claim %s (%d), state %d (line %d)\n",
					depth, procname[spin_c_typ[nn]], nn, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported17 = 1;
				fflush(stdout);
		}	}
#else
		{	static int reported17 = 0;
			if (verbose && !reported17)
			{	printf("depth %d: Claim, state %d (line %d)\n",
					(int) depth, (int) ((Pclaim *)pptr(0))->_p, src_claim[ (int) ((Pclaim *)pptr(0))->_p ]);
				reported17 = 1;
				fflush(stdout);
		}	}
#endif
#endif
		reached[8][17] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC :init: */
	case 6: // STATE 4 - dataview.pml:12361 - [D_STEP12361]
		IfNotBlocked

		reached[7][4] = 1;
		reached[7][t->st] = 1;
		reached[7][tt] = 1;

		if (TstOnly) return 1;


		reached[7][4] = 1;
		reached[7][t->st] = 1;
		reached[7][tt] = 1;

		sv_save();
		S_473_0: /* 2 */
S_472_0: /* 2 */
S_471_0: /* 2 */
		if (TstOnly) return 1; /* T3 */
		now.nothing.selection = 1;
#ifdef VAR_RANGES
		logval("nothing.selection", now.nothing.selection);
#endif
		;
		goto S_512_0;
S_512_0: /* 1 */

#if defined(C_States) && (HAS_TRACK==1)
		c_update((uchar *) &(now.c_state[0]));
#endif
		_m = 3; goto P999;

	case 7: // STATE 6 - dataview.pml:5144 - [global_state.actuator.sender = 4] (0:0:1 - 1)
		IfNotBlocked
		reached[7][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.sender;
		now.global_state.actuator.sender = 4;
#ifdef VAR_RANGES
		logval("global_state.actuator.sender", now.global_state.actuator.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 8: // STATE 7 - dataview.pml:5149 - [assert((((((global_state.actuator.sender==0)||(global_state.actuator.sender==1))||(global_state.actuator.sender==2))||(global_state.actuator.sender==3))||(global_state.actuator.sender==4)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][7] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((now.global_state.actuator.sender==0)||(now.global_state.actuator.sender==1))||(now.global_state.actuator.sender==2))||(now.global_state.actuator.sender==3))||(now.global_state.actuator.sender==4)), "(((((global_state.actuator.sender==0)||(global_state.actuator.sender==1))||(global_state.actuator.sender==2))||(global_state.actuator.sender==3))||(global_state.actuator.sender==4))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 9: // STATE 10 - actuator.pml:4 - [transition_id = 0] (0:38:2 - 1)
		IfNotBlocked
		reached[7][10] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_8_17_transition_id;
		((P7 *)_this)->_710_8_8_17_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_8_17_transition_id);
#endif
		;
		/* merge: transition_id = 0(38, 11, 38) */
		reached[7][11] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_710_8_8_17_transition_id;
		((P7 *)_this)->_710_8_8_17_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_8_17_transition_id);
#endif
		;
		/* merge: .(goto)(0, 39, 38) */
		reached[7][39] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 10: // STATE 12 - actuator.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][12] = 1;
		if (!((((P7 *)_this)->_710_8_8_17_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_8_17_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_710_8_8_17_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_8_17_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 11: // STATE 14 - actuator.pml:9 - [((transition_id==0))] (37:0:3 - 1)
		IfNotBlocked
		reached[7][14] = 1;
		if (!((((P7 *)_this)->_710_8_8_17_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_8_17_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_8_17_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_8_17_transition_id = 0;
		/* merge: transition_id = -(1)(37, 15, 37) */
		reached[7][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_710_8_8_17_transition_id;
		((P7 *)_this)->_710_8_8_17_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_8_17_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(37, 16, 37) */
		reached[7][16] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 17, 37) */
		reached[7][17] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 12: // STATE 18 - actuator.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][18] = 1;
		if (!((((P7 *)_this)->_710_8_8_17_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_8_17_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_710_8_8_17_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_8_17_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 13: // STATE 21 - actuator.pml:16 - [(((global_state.actuator.x>10)==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][21] = 1;
		if (!(((now.global_state.actuator.x>10)==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 14: // STATE 22 - system.pml:50 - [Modemanager_feedback_channel!global_state.actuator.x] (37:0:2 - 1)
		IfNotBlocked
		reached[7][22] = 1;
		if (q_full(now.Modemanager_feedback_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", now.global_state.actuator.x); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_feedback_channel, 0, now.global_state.actuator.x, 0, 0, 1);
		/* merge: transition_id = -(1)(37, 24, 37) */
		reached[7][24] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_8_17_transition_id;
		((P7 *)_this)->_710_8_8_17_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_8_17_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 1(37, 25, 37) */
		reached[7][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 26, 37) */
		reached[7][26] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 15: // STATE 27 - actuator.pml:21 - [(((global_state.actuator.x>10)==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][27] = 1;
		if (!(((now.global_state.actuator.x>10)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 16: // STATE 28 - system.pml:50 - [Modemanager_feedback_channel!0] (37:0:2 - 1)
		IfNotBlocked
		reached[7][28] = 1;
		if (q_full(now.Modemanager_feedback_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_feedback_channel, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(37, 30, 37) */
		reached[7][30] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_8_17_transition_id;
		((P7 *)_this)->_710_8_8_17_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_8_17_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(37, 31, 37) */
		reached[7][31] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 32, 37) */
		reached[7][32] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 17: // STATE 43 - system.pml:203 - [Actuator_lock!1] (0:0:0 - 3)
		IfNotBlocked
		reached[7][43] = 1;
		if (q_full(now.Actuator_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 18: // STATE 44 - dataview.pml:5144 - [global_state.modemanager.sender = 4] (0:0:1 - 1)
		IfNotBlocked
		reached[7][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.sender;
		now.global_state.modemanager.sender = 4;
#ifdef VAR_RANGES
		logval("global_state.modemanager.sender", now.global_state.modemanager.sender);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 19: // STATE 45 - dataview.pml:5149 - [assert((((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][45] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert((((((now.global_state.modemanager.sender==0)||(now.global_state.modemanager.sender==1))||(now.global_state.modemanager.sender==2))||(now.global_state.modemanager.sender==3))||(now.global_state.modemanager.sender==4)), "(((((global_state.modemanager.sender==0)||(global_state.modemanager.sender==1))||(global_state.modemanager.sender==2))||(global_state.modemanager.sender==3))||(global_state.modemanager.sender==4))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 20: // STATE 48 - modemanager.pml:4 - [transition_id = 0] (0:96:2 - 1)
		IfNotBlocked
		reached[7][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_9_19_transition_id;
		((P7 *)_this)->_710_8_9_19_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_9_19_transition_id);
#endif
		;
		/* merge: transition_id = 0(96, 49, 96) */
		reached[7][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_710_8_9_19_transition_id;
		((P7 *)_this)->_710_8_9_19_transition_id = 0;
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_9_19_transition_id);
#endif
		;
		/* merge: .(goto)(0, 97, 96) */
		reached[7][97] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 21: // STATE 50 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][50] = 1;
		if (!((((P7 *)_this)->_710_8_9_19_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_9_19_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_710_8_9_19_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_9_19_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 22: // STATE 52 - modemanager.pml:9 - [((transition_id==0))] (95:0:3 - 1)
		IfNotBlocked
		reached[7][52] = 1;
		if (!((((P7 *)_this)->_710_8_9_19_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_9_19_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_9_19_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_9_19_transition_id = 0;
		/* merge: transition_id = -(1)(95, 53, 95) */
		reached[7][53] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P7 *)_this)->_710_8_9_19_transition_id;
		((P7 *)_this)->_710_8_9_19_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_9_19_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(95, 54, 95) */
		reached[7][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 55, 95) */
		reached[7][55] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 23: // STATE 56 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][56] = 1;
		if (!((((P7 *)_this)->_710_8_9_19_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_9_19_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_710_8_9_19_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_9_19_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 24: // STATE 59 - dataview.pml:5021 - [global_state.modemanager.rprt.reading = global_state.modemanager.xin] (0:0:1 - 1)
		IfNotBlocked
		reached[7][59] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.reading;
		now.global_state.modemanager.rprt.reading = now.global_state.modemanager.xin;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.reading", now.global_state.modemanager.rprt.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 25: // STATE 60 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][60] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.reading>=0)&&(now.global_state.modemanager.rprt.reading<=10000)), "((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 26: // STATE 63 - dataview.pml:5021 - [global_state.modemanager.rprt.ctrl = global_state.modemanager.xout] (0:0:1 - 1)
		IfNotBlocked
		reached[7][63] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.ctrl;
		now.global_state.modemanager.rprt.ctrl = now.global_state.modemanager.xout;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.ctrl", now.global_state.modemanager.rprt.ctrl);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 27: // STATE 64 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][64] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.ctrl>=0)&&(now.global_state.modemanager.rprt.ctrl<=10000)), "((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 28: // STATE 67 - dataview.pml:5021 - [global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback] (0:0:1 - 1)
		IfNotBlocked
		reached[7][67] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.feedback;
		now.global_state.modemanager.rprt.feedback = now.global_state.modemanager.xfeedback;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.feedback", now.global_state.modemanager.rprt.feedback);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 29: // STATE 68 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][68] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.feedback>=0)&&(now.global_state.modemanager.rprt.feedback<=10000)), "((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 30: // STATE 72 - system.pml:46 - [Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback] (95:0:1 - 1)
		IfNotBlocked
		reached[7][72] = 1;
		if (q_full(now.Egse_tm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_tm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.ctrl); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.feedback); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_tm_channel, 0, now.global_state.modemanager.rprt.reading, now.global_state.modemanager.rprt.ctrl, now.global_state.modemanager.rprt.feedback, 3);
		/* merge: transition_id = -(1)(0, 74, 95) */
		reached[7][74] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_710_8_9_19_transition_id;
		((P7 *)_this)->_710_8_9_19_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_9_19_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 75, 95) */
		reached[7][75] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 31: // STATE 76 - modemanager.pml:23 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][76] = 1;
		if (!((((P7 *)_this)->_710_8_9_19_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_9_19_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_710_8_9_19_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_9_19_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 32: // STATE 79 - dataview.pml:5021 - [global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)] (0:0:1 - 1)
		IfNotBlocked
		reached[7][79] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.xout;
		now.global_state.modemanager.xout = ((now.global_state.modemanager.xin*now.global_state.modemanager.cfg.param_a)+now.global_state.modemanager.cfg.param_b);
#ifdef VAR_RANGES
		logval("global_state.modemanager.xout", now.global_state.modemanager.xout);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 33: // STATE 80 - dataview.pml:5026 - [assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[7][80] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.xout>=0)&&(now.global_state.modemanager.xout<=10000)), "((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 34: // STATE 83 - system.pml:32 - [Actuator_ctrl_channel!global_state.modemanager.xout] (95:0:1 - 1)
		IfNotBlocked
		reached[7][83] = 1;
		if (q_full(now.Actuator_ctrl_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_ctrl_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.xout); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_ctrl_channel, 0, now.global_state.modemanager.xout, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 85, 95) */
		reached[7][85] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P7 *)_this)->_710_8_9_19_transition_id;
		((P7 *)_this)->_710_8_9_19_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_9_19_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 86, 95) */
		reached[7][86] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 35: // STATE 87 - modemanager.pml:29 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[7][87] = 1;
		if (!((((P7 *)_this)->_710_8_9_19_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _710_8_9_19_transition_id */  (trpt+1)->bup.oval = ((P7 *)_this)->_710_8_9_19_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P7 *)_this)->_710_8_9_19_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 36: // STATE 88 - system.pml:68 - [(1)] (95:0:2 - 1)
		IfNotBlocked
		reached[7][88] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(95, 90, 95) */
		reached[7][90] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P7 *)_this)->_710_8_9_19_transition_id;
		((P7 *)_this)->_710_8_9_19_transition_id =  -(1);
#ifdef VAR_RANGES
		logval(":init::transition_id", ((P7 *)_this)->_710_8_9_19_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(95, 91, 95) */
		reached[7][91] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 92, 95) */
		reached[7][92] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 37: // STATE 101 - system.pml:205 - [Modemanager_lock!1] (104:0:1 - 3)
		IfNotBlocked
		reached[7][101] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 1);
		/* merge: inited = 1(0, 102, 104) */
		reached[7][102] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.inited;
		now.inited = 1;
#ifdef VAR_RANGES
		logval("inited", now.inited);
#endif
		;
		_m = 2; goto P999; /* 1 */
	case 38: // STATE 104 - system.pml:208 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[7][104] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Sensor_reading */
	case 39: // STATE 1 - system.pml:187 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[6][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 40: // STATE 2 - system.pml:189 - [value = 0] (0:41:3 - 1)
		IfNotBlocked
		reached[6][2] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P6 *)_this)->value;
		((P6 *)_this)->value = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P6 *)_this)->value);
#endif
		;
		/* merge: inputVectorCounter = 0(41, 3, 41) */
		reached[6][3] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P6 *)_this)->inputVectorCounter;
		((P6 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:inputVectorCounter", ((P6 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: inputVectorCounter = 0(41, 4, 41) */
		reached[6][4] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((P6 *)_this)->inputVectorCounter;
		((P6 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:inputVectorCounter", ((P6 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 42, 41) */
		reached[6][42] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 41: // STATE 5 - system.pml:190 - [((inputVectorCounter<=3))] (0:0:0 - 1)
		IfNotBlocked
		reached[6][5] = 1;
		if (!((((P6 *)_this)->inputVectorCounter<=3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 42: // STATE 6 - env_inlines.pml:25 - [Sensor_reading_x_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[6][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 43: // STATE 7 - env_inlines.pml:27 - [Sensor_reading_x_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[6][7] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = 0;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 44: // STATE 10 - env_inlines.pml:27 - [goto :b18] (0:36:1 - 1)
		IfNotBlocked
		reached[6][10] = 1;
		;
		/* merge: value = Sensor_reading_x_tmp(36, 33, 36) */
		reached[6][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->value;
		((P6 *)_this)->value = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P6 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 45: // STATE 8 - env_inlines.pml:27 - [((Sensor_reading_x_tmp<0))] (11:0:1 - 1)
		IfNotBlocked
		reached[6][8] = 1;
		if (!((((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp<0)))
			continue;
		/* merge: Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)(0, 9, 11) */
		reached[6][9] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = (((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		/* merge: .(goto)(0, 12, 11) */
		reached[6][12] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 46: // STATE 15 - env_inlines.pml:30 - [Sensor_reading_x_tmp = 10] (0:0:1 - 1)
		IfNotBlocked
		reached[6][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = 10;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 47: // STATE 18 - env_inlines.pml:30 - [goto :b19] (0:36:1 - 1)
		IfNotBlocked
		reached[6][18] = 1;
		;
		/* merge: value = Sensor_reading_x_tmp(36, 33, 36) */
		reached[6][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->value;
		((P6 *)_this)->value = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P6 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 48: // STATE 16 - env_inlines.pml:30 - [((Sensor_reading_x_tmp<10))] (19:0:1 - 1)
		IfNotBlocked
		reached[6][16] = 1;
		if (!((((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp<10)))
			continue;
		/* merge: Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)(0, 17, 19) */
		reached[6][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = (((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		/* merge: .(goto)(0, 20, 19) */
		reached[6][20] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 49: // STATE 23 - env_inlines.pml:33 - [Sensor_reading_x_tmp = 100] (0:0:1 - 1)
		IfNotBlocked
		reached[6][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = 100;
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 50: // STATE 26 - env_inlines.pml:33 - [goto :b20] (0:36:1 - 1)
		IfNotBlocked
		reached[6][26] = 1;
		;
		/* merge: value = Sensor_reading_x_tmp(36, 33, 36) */
		reached[6][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->value;
		((P6 *)_this)->value = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P6 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 51: // STATE 24 - env_inlines.pml:33 - [((Sensor_reading_x_tmp<100))] (27:0:1 - 1)
		IfNotBlocked
		reached[6][24] = 1;
		if (!((((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp<100)))
			continue;
		/* merge: Sensor_reading_x_tmp = (Sensor_reading_x_tmp+1)(0, 25, 27) */
		reached[6][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = (((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:Sensor_reading_x_tmp", ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp);
#endif
		;
		/* merge: .(goto)(0, 28, 27) */
		reached[6][28] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 52: // STATE 33 - env_inlines.pml:36 - [value = Sensor_reading_x_tmp] (0:36:1 - 7)
		IfNotBlocked
		reached[6][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->value;
		((P6 *)_this)->value = ((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp;
#ifdef VAR_RANGES
		logval("Sensor_reading:value", ((P6 *)_this)->value);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 53: // STATE 35 - system.pml:54 - [Modemanager_reading_channel!value] (41:0:1 - 1)
		IfNotBlocked
		reached[6][35] = 1;
		if (q_full(now.Modemanager_reading_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_reading_channel);
		sprintf(simtmp, "%d", ((P6 *)_this)->value); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_reading_channel, 0, ((P6 *)_this)->value, 0, 0, 1);
		/* merge: inputVectorCounter = (inputVectorCounter+1)(0, 38, 41) */
		reached[6][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P6 *)_this)->inputVectorCounter;
		((P6 *)_this)->inputVectorCounter = (((P6 *)_this)->inputVectorCounter+1);
#ifdef VAR_RANGES
		logval("Sensor_reading:inputVectorCounter", ((P6 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 42, 41) */
		reached[6][42] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 54: // STATE 44 - system.pml:197 - [-end-] (0:0:0 - 3)
		IfNotBlocked
		reached[6][44] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_tc */
	case 55: // STATE 1 - system.pml:166 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[5][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 56: // STATE 2 - system.pml:169 - [(nempty(Modemanager_tc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][2] = 1;
		if (!((q_len(now.Modemanager_tc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 57: // STATE 3 - system.pml:170 - [Modemanager_lock?_] (0:0:1 - 1)
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
	case 58: // STATE 4 - system.pml:173 - [(nempty(Modemanager_tc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][4] = 1;
		if (!((q_len(now.Modemanager_tc_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 59: // STATE 5 - system.pml:174 - [Modemanager_tc_channel?Modemanager_tc_signal_parameter.param_a,Modemanager_tc_signal_parameter.param_b] (73:0:3 - 1)
		reached[5][5] = 1;
		if (q_len(now.Modemanager_tc_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = now.Modemanager_tc_signal_parameter.param_a;
		(trpt+1)->bup.ovals[1] = now.Modemanager_tc_signal_parameter.param_b;
		;
		now.Modemanager_tc_signal_parameter.param_a = qrecv(now.Modemanager_tc_channel, XX-1, 0, 0);
#ifdef VAR_RANGES
		logval("Modemanager_tc_signal_parameter.param_a", now.Modemanager_tc_signal_parameter.param_a);
#endif
		;
		now.Modemanager_tc_signal_parameter.param_b = qrecv(now.Modemanager_tc_channel, XX-1, 1, 1);
#ifdef VAR_RANGES
		logval("Modemanager_tc_signal_parameter.param_b", now.Modemanager_tc_signal_parameter.param_b);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_tc_channel);
		sprintf(simtmp, "%d", now.Modemanager_tc_signal_parameter.param_a); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Modemanager_tc_signal_parameter.param_b); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Modemanager_tc_channel_used = 1(0, 6, 73) */
		reached[5][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = ((int)Modemanager_tc_channel_used);
		Modemanager_tc_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_tc_channel_used", ((int)Modemanager_tc_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 60: // STATE 7 - modemanager.pml:66 - [((global_state.modemanager.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][7] = 1;
		if (!((now.global_state.modemanager.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 61: // STATE 8 - dataview.pml:5030 - [global_state.modemanager.cfg.param_a = Modemanager_tc_signal_parameter.param_a] (0:0:1 - 1)
		IfNotBlocked
		reached[5][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.cfg.param_a;
		now.global_state.modemanager.cfg.param_a = now.Modemanager_tc_signal_parameter.param_a;
#ifdef VAR_RANGES
		logval("global_state.modemanager.cfg.param_a", now.global_state.modemanager.cfg.param_a);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 62: // STATE 9 - dataview.pml:5035 - [assert(((global_state.modemanager.cfg.param_a>=0)&&(global_state.modemanager.cfg.param_a<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.cfg.param_a>=0)&&(now.global_state.modemanager.cfg.param_a<=10000)), "((global_state.modemanager.cfg.param_a>=0)&&(global_state.modemanager.cfg.param_a<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 63: // STATE 12 - dataview.pml:5039 - [global_state.modemanager.cfg.param_b = Modemanager_tc_signal_parameter.param_b] (0:0:1 - 1)
		IfNotBlocked
		reached[5][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.cfg.param_b;
		now.global_state.modemanager.cfg.param_b = now.Modemanager_tc_signal_parameter.param_b;
#ifdef VAR_RANGES
		logval("global_state.modemanager.cfg.param_b", now.global_state.modemanager.cfg.param_b);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 64: // STATE 13 - dataview.pml:5044 - [assert(((global_state.modemanager.cfg.param_b>=0)&&(global_state.modemanager.cfg.param_b<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][13] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.cfg.param_b>=0)&&(now.global_state.modemanager.cfg.param_b<=10000)), "((global_state.modemanager.cfg.param_b>=0)&&(global_state.modemanager.cfg.param_b<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 65: // STATE 17 - modemanager.pml:4 - [transition_id = 0] (0:65:2 - 1)
		IfNotBlocked
		reached[5][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_708_6_5_12_transition_id;
		((P5 *)_this)->_708_6_5_12_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_tc:transition_id", ((P5 *)_this)->_708_6_5_12_transition_id);
#endif
		;
		/* merge: transition_id = 3(65, 18, 65) */
		reached[5][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_708_6_5_12_transition_id;
		((P5 *)_this)->_708_6_5_12_transition_id = 3;
#ifdef VAR_RANGES
		logval("Modemanager_tc:transition_id", ((P5 *)_this)->_708_6_5_12_transition_id);
#endif
		;
		/* merge: .(goto)(0, 66, 65) */
		reached[5][66] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 66: // STATE 19 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][19] = 1;
		if (!((((P5 *)_this)->_708_6_5_12_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _708_6_5_12_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_708_6_5_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_708_6_5_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 67: // STATE 21 - modemanager.pml:9 - [((transition_id==0))] (64:0:3 - 1)
		IfNotBlocked
		reached[5][21] = 1;
		if (!((((P5 *)_this)->_708_6_5_12_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _708_6_5_12_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_708_6_5_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_708_6_5_12_transition_id = 0;
		/* merge: transition_id = -(1)(64, 22, 64) */
		reached[5][22] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P5 *)_this)->_708_6_5_12_transition_id;
		((P5 *)_this)->_708_6_5_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_tc:transition_id", ((P5 *)_this)->_708_6_5_12_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(64, 23, 64) */
		reached[5][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 24, 64) */
		reached[5][24] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 68: // STATE 25 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][25] = 1;
		if (!((((P5 *)_this)->_708_6_5_12_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _708_6_5_12_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_708_6_5_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_708_6_5_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 69: // STATE 28 - dataview.pml:5021 - [global_state.modemanager.rprt.reading = global_state.modemanager.xin] (0:0:1 - 1)
		IfNotBlocked
		reached[5][28] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.reading;
		now.global_state.modemanager.rprt.reading = now.global_state.modemanager.xin;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.reading", now.global_state.modemanager.rprt.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 70: // STATE 29 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][29] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.reading>=0)&&(now.global_state.modemanager.rprt.reading<=10000)), "((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 71: // STATE 32 - dataview.pml:5021 - [global_state.modemanager.rprt.ctrl = global_state.modemanager.xout] (0:0:1 - 1)
		IfNotBlocked
		reached[5][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.ctrl;
		now.global_state.modemanager.rprt.ctrl = now.global_state.modemanager.xout;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.ctrl", now.global_state.modemanager.rprt.ctrl);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 72: // STATE 33 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][33] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.ctrl>=0)&&(now.global_state.modemanager.rprt.ctrl<=10000)), "((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 73: // STATE 36 - dataview.pml:5021 - [global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback] (0:0:1 - 1)
		IfNotBlocked
		reached[5][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.feedback;
		now.global_state.modemanager.rprt.feedback = now.global_state.modemanager.xfeedback;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.feedback", now.global_state.modemanager.rprt.feedback);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 74: // STATE 37 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][37] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.feedback>=0)&&(now.global_state.modemanager.rprt.feedback<=10000)), "((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 75: // STATE 41 - system.pml:46 - [Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback] (64:0:1 - 1)
		IfNotBlocked
		reached[5][41] = 1;
		if (q_full(now.Egse_tm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_tm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.ctrl); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.feedback); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_tm_channel, 0, now.global_state.modemanager.rprt.reading, now.global_state.modemanager.rprt.ctrl, now.global_state.modemanager.rprt.feedback, 3);
		/* merge: transition_id = -(1)(0, 43, 64) */
		reached[5][43] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_708_6_5_12_transition_id;
		((P5 *)_this)->_708_6_5_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_tc:transition_id", ((P5 *)_this)->_708_6_5_12_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 44, 64) */
		reached[5][44] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 76: // STATE 45 - modemanager.pml:23 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][45] = 1;
		if (!((((P5 *)_this)->_708_6_5_12_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _708_6_5_12_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_708_6_5_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_708_6_5_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 77: // STATE 48 - dataview.pml:5021 - [global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)] (0:0:1 - 1)
		IfNotBlocked
		reached[5][48] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.xout;
		now.global_state.modemanager.xout = ((now.global_state.modemanager.xin*now.global_state.modemanager.cfg.param_a)+now.global_state.modemanager.cfg.param_b);
#ifdef VAR_RANGES
		logval("global_state.modemanager.xout", now.global_state.modemanager.xout);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 78: // STATE 49 - dataview.pml:5026 - [assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][49] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.xout>=0)&&(now.global_state.modemanager.xout<=10000)), "((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 79: // STATE 52 - system.pml:32 - [Actuator_ctrl_channel!global_state.modemanager.xout] (64:0:1 - 1)
		IfNotBlocked
		reached[5][52] = 1;
		if (q_full(now.Actuator_ctrl_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_ctrl_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.xout); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_ctrl_channel, 0, now.global_state.modemanager.xout, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 54, 64) */
		reached[5][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P5 *)_this)->_708_6_5_12_transition_id;
		((P5 *)_this)->_708_6_5_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_tc:transition_id", ((P5 *)_this)->_708_6_5_12_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 55, 64) */
		reached[5][55] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 80: // STATE 56 - modemanager.pml:29 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[5][56] = 1;
		if (!((((P5 *)_this)->_708_6_5_12_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _708_6_5_12_transition_id */  (trpt+1)->bup.oval = ((P5 *)_this)->_708_6_5_12_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P5 *)_this)->_708_6_5_12_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 81: // STATE 57 - system.pml:68 - [(1)] (64:0:2 - 1)
		IfNotBlocked
		reached[5][57] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(64, 59, 64) */
		reached[5][59] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P5 *)_this)->_708_6_5_12_transition_id;
		((P5 *)_this)->_708_6_5_12_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_tc:transition_id", ((P5 *)_this)->_708_6_5_12_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(64, 60, 64) */
		reached[5][60] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 61, 64) */
		reached[5][61] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 82: // STATE 75 - system.pml:178 - [(empty(Modemanager_tc_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[5][75] = 1;
		if (!((q_len(now.Modemanager_tc_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 83: // STATE 76 - system.pml:179 - [(1)] (79:0:0 - 1)
		IfNotBlocked
		reached[5][76] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 78, 79) */
		reached[5][78] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 84: // STATE 79 - system.pml:181 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[5][79] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 85: // STATE 84 - system.pml:184 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[5][84] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_reading */
	case 86: // STATE 1 - system.pml:145 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[4][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 87: // STATE 2 - system.pml:148 - [(nempty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][2] = 1;
		if (!((q_len(now.Modemanager_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 88: // STATE 3 - system.pml:149 - [Modemanager_lock?_] (0:0:1 - 1)
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
	case 89: // STATE 4 - system.pml:152 - [(nempty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][4] = 1;
		if (!((q_len(now.Modemanager_reading_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 90: // STATE 5 - system.pml:153 - [Modemanager_reading_channel?Modemanager_reading_signal_parameter] (68:0:2 - 1)
		reached[4][5] = 1;
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
		/* merge: Modemanager_reading_channel_used = 1(0, 6, 68) */
		reached[4][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Modemanager_reading_channel_used);
		Modemanager_reading_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_reading_channel_used", ((int)Modemanager_reading_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 91: // STATE 7 - modemanager.pml:56 - [((global_state.modemanager.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][7] = 1;
		if (!((now.global_state.modemanager.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 92: // STATE 8 - dataview.pml:5021 - [global_state.modemanager.xin = Modemanager_reading_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[4][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.xin;
		now.global_state.modemanager.xin = now.Modemanager_reading_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.xin", now.global_state.modemanager.xin);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 93: // STATE 9 - dataview.pml:5026 - [assert(((global_state.modemanager.xin>=0)&&(global_state.modemanager.xin<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.xin>=0)&&(now.global_state.modemanager.xin<=10000)), "((global_state.modemanager.xin>=0)&&(global_state.modemanager.xin<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 94: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:60:2 - 1)
		IfNotBlocked
		reached[4][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_707_5_4_10_transition_id;
		((P4 *)_this)->_707_5_4_10_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P4 *)_this)->_707_5_4_10_transition_id);
#endif
		;
		/* merge: transition_id = 2(60, 13, 60) */
		reached[4][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_707_5_4_10_transition_id;
		((P4 *)_this)->_707_5_4_10_transition_id = 2;
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P4 *)_this)->_707_5_4_10_transition_id);
#endif
		;
		/* merge: .(goto)(0, 61, 60) */
		reached[4][61] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 95: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][14] = 1;
		if (!((((P4 *)_this)->_707_5_4_10_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _707_5_4_10_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_707_5_4_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_707_5_4_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 96: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (59:0:3 - 1)
		IfNotBlocked
		reached[4][16] = 1;
		if (!((((P4 *)_this)->_707_5_4_10_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _707_5_4_10_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_707_5_4_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_707_5_4_10_transition_id = 0;
		/* merge: transition_id = -(1)(59, 17, 59) */
		reached[4][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P4 *)_this)->_707_5_4_10_transition_id;
		((P4 *)_this)->_707_5_4_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P4 *)_this)->_707_5_4_10_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(59, 18, 59) */
		reached[4][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 59) */
		reached[4][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 97: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][20] = 1;
		if (!((((P4 *)_this)->_707_5_4_10_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _707_5_4_10_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_707_5_4_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_707_5_4_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 98: // STATE 23 - dataview.pml:5021 - [global_state.modemanager.rprt.reading = global_state.modemanager.xin] (0:0:1 - 1)
		IfNotBlocked
		reached[4][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.reading;
		now.global_state.modemanager.rprt.reading = now.global_state.modemanager.xin;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.reading", now.global_state.modemanager.rprt.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 99: // STATE 24 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][24] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.reading>=0)&&(now.global_state.modemanager.rprt.reading<=10000)), "((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 100: // STATE 27 - dataview.pml:5021 - [global_state.modemanager.rprt.ctrl = global_state.modemanager.xout] (0:0:1 - 1)
		IfNotBlocked
		reached[4][27] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.ctrl;
		now.global_state.modemanager.rprt.ctrl = now.global_state.modemanager.xout;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.ctrl", now.global_state.modemanager.rprt.ctrl);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 101: // STATE 28 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][28] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.ctrl>=0)&&(now.global_state.modemanager.rprt.ctrl<=10000)), "((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 102: // STATE 31 - dataview.pml:5021 - [global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback] (0:0:1 - 1)
		IfNotBlocked
		reached[4][31] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.feedback;
		now.global_state.modemanager.rprt.feedback = now.global_state.modemanager.xfeedback;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.feedback", now.global_state.modemanager.rprt.feedback);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 103: // STATE 32 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][32] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.feedback>=0)&&(now.global_state.modemanager.rprt.feedback<=10000)), "((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 104: // STATE 36 - system.pml:46 - [Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback] (59:0:1 - 1)
		IfNotBlocked
		reached[4][36] = 1;
		if (q_full(now.Egse_tm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_tm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.ctrl); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.feedback); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_tm_channel, 0, now.global_state.modemanager.rprt.reading, now.global_state.modemanager.rprt.ctrl, now.global_state.modemanager.rprt.feedback, 3);
		/* merge: transition_id = -(1)(0, 38, 59) */
		reached[4][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_707_5_4_10_transition_id;
		((P4 *)_this)->_707_5_4_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P4 *)_this)->_707_5_4_10_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 39, 59) */
		reached[4][39] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 105: // STATE 40 - modemanager.pml:23 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][40] = 1;
		if (!((((P4 *)_this)->_707_5_4_10_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _707_5_4_10_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_707_5_4_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_707_5_4_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 106: // STATE 43 - dataview.pml:5021 - [global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)] (0:0:1 - 1)
		IfNotBlocked
		reached[4][43] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.xout;
		now.global_state.modemanager.xout = ((now.global_state.modemanager.xin*now.global_state.modemanager.cfg.param_a)+now.global_state.modemanager.cfg.param_b);
#ifdef VAR_RANGES
		logval("global_state.modemanager.xout", now.global_state.modemanager.xout);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 107: // STATE 44 - dataview.pml:5026 - [assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][44] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.xout>=0)&&(now.global_state.modemanager.xout<=10000)), "((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 108: // STATE 47 - system.pml:32 - [Actuator_ctrl_channel!global_state.modemanager.xout] (59:0:1 - 1)
		IfNotBlocked
		reached[4][47] = 1;
		if (q_full(now.Actuator_ctrl_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_ctrl_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.xout); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_ctrl_channel, 0, now.global_state.modemanager.xout, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 49, 59) */
		reached[4][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P4 *)_this)->_707_5_4_10_transition_id;
		((P4 *)_this)->_707_5_4_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P4 *)_this)->_707_5_4_10_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 50, 59) */
		reached[4][50] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 109: // STATE 51 - modemanager.pml:29 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[4][51] = 1;
		if (!((((P4 *)_this)->_707_5_4_10_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _707_5_4_10_transition_id */  (trpt+1)->bup.oval = ((P4 *)_this)->_707_5_4_10_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P4 *)_this)->_707_5_4_10_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 110: // STATE 52 - system.pml:68 - [(1)] (59:0:2 - 1)
		IfNotBlocked
		reached[4][52] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(59, 54, 59) */
		reached[4][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P4 *)_this)->_707_5_4_10_transition_id;
		((P4 *)_this)->_707_5_4_10_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_reading:transition_id", ((P4 *)_this)->_707_5_4_10_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(59, 55, 59) */
		reached[4][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 59) */
		reached[4][56] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 111: // STATE 70 - system.pml:157 - [(empty(Modemanager_reading_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[4][70] = 1;
		if (!((q_len(now.Modemanager_reading_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 112: // STATE 71 - system.pml:158 - [(1)] (74:0:0 - 1)
		IfNotBlocked
		reached[4][71] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 73, 74) */
		reached[4][73] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 113: // STATE 74 - system.pml:160 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[4][74] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 114: // STATE 79 - system.pml:163 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[4][79] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Modemanager_feedback */
	case 115: // STATE 1 - system.pml:124 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[3][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 116: // STATE 2 - system.pml:127 - [(nempty(Modemanager_feedback_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][2] = 1;
		if (!((q_len(now.Modemanager_feedback_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 117: // STATE 3 - system.pml:128 - [Modemanager_lock?_] (0:0:1 - 1)
		reached[3][3] = 1;
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
	case 118: // STATE 4 - system.pml:131 - [(nempty(Modemanager_feedback_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][4] = 1;
		if (!((q_len(now.Modemanager_feedback_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 119: // STATE 5 - system.pml:132 - [Modemanager_feedback_channel?Modemanager_feedback_signal_parameter] (68:0:2 - 1)
		reached[3][5] = 1;
		if (q_len(now.Modemanager_feedback_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Modemanager_feedback_signal_parameter;
		;
		now.Modemanager_feedback_signal_parameter = qrecv(now.Modemanager_feedback_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Modemanager_feedback_signal_parameter", now.Modemanager_feedback_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", now.Modemanager_feedback_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Modemanager_feedback_channel_used = 1(0, 6, 68) */
		reached[3][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Modemanager_feedback_channel_used);
		Modemanager_feedback_channel_used = 1;
#ifdef VAR_RANGES
		logval("Modemanager_feedback_channel_used", ((int)Modemanager_feedback_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 120: // STATE 7 - modemanager.pml:46 - [((global_state.modemanager.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][7] = 1;
		if (!((now.global_state.modemanager.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 121: // STATE 8 - dataview.pml:5021 - [global_state.modemanager.xfeedback = Modemanager_feedback_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[3][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.xfeedback;
		now.global_state.modemanager.xfeedback = now.Modemanager_feedback_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.modemanager.xfeedback", now.global_state.modemanager.xfeedback);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 122: // STATE 9 - dataview.pml:5026 - [assert(((global_state.modemanager.xfeedback>=0)&&(global_state.modemanager.xfeedback<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.xfeedback>=0)&&(now.global_state.modemanager.xfeedback<=10000)), "((global_state.modemanager.xfeedback>=0)&&(global_state.modemanager.xfeedback<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 123: // STATE 12 - modemanager.pml:4 - [transition_id = 0] (0:60:2 - 1)
		IfNotBlocked
		reached[3][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->_706_4_3_8_transition_id;
		((P3 *)_this)->_706_4_3_8_transition_id = 0;
#ifdef VAR_RANGES
		logval("Modemanager_feedback:transition_id", ((P3 *)_this)->_706_4_3_8_transition_id);
#endif
		;
		/* merge: transition_id = 1(60, 13, 60) */
		reached[3][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->_706_4_3_8_transition_id;
		((P3 *)_this)->_706_4_3_8_transition_id = 1;
#ifdef VAR_RANGES
		logval("Modemanager_feedback:transition_id", ((P3 *)_this)->_706_4_3_8_transition_id);
#endif
		;
		/* merge: .(goto)(0, 61, 60) */
		reached[3][61] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 124: // STATE 14 - modemanager.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][14] = 1;
		if (!((((P3 *)_this)->_706_4_3_8_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _706_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_706_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_706_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 125: // STATE 16 - modemanager.pml:9 - [((transition_id==0))] (59:0:3 - 1)
		IfNotBlocked
		reached[3][16] = 1;
		if (!((((P3 *)_this)->_706_4_3_8_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _706_4_3_8_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->_706_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_706_4_3_8_transition_id = 0;
		/* merge: transition_id = -(1)(59, 17, 59) */
		reached[3][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P3 *)_this)->_706_4_3_8_transition_id;
		((P3 *)_this)->_706_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_feedback:transition_id", ((P3 *)_this)->_706_4_3_8_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 1(59, 18, 59) */
		reached[3][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 1;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 59) */
		reached[3][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 126: // STATE 20 - modemanager.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][20] = 1;
		if (!((((P3 *)_this)->_706_4_3_8_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _706_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_706_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_706_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 127: // STATE 23 - dataview.pml:5021 - [global_state.modemanager.rprt.reading = global_state.modemanager.xin] (0:0:1 - 1)
		IfNotBlocked
		reached[3][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.reading;
		now.global_state.modemanager.rprt.reading = now.global_state.modemanager.xin;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.reading", now.global_state.modemanager.rprt.reading);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 128: // STATE 24 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][24] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.reading>=0)&&(now.global_state.modemanager.rprt.reading<=10000)), "((global_state.modemanager.rprt.reading>=0)&&(global_state.modemanager.rprt.reading<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 129: // STATE 27 - dataview.pml:5021 - [global_state.modemanager.rprt.ctrl = global_state.modemanager.xout] (0:0:1 - 1)
		IfNotBlocked
		reached[3][27] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.ctrl;
		now.global_state.modemanager.rprt.ctrl = now.global_state.modemanager.xout;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.ctrl", now.global_state.modemanager.rprt.ctrl);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 130: // STATE 28 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][28] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.ctrl>=0)&&(now.global_state.modemanager.rprt.ctrl<=10000)), "((global_state.modemanager.rprt.ctrl>=0)&&(global_state.modemanager.rprt.ctrl<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 131: // STATE 31 - dataview.pml:5021 - [global_state.modemanager.rprt.feedback = global_state.modemanager.xfeedback] (0:0:1 - 1)
		IfNotBlocked
		reached[3][31] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.rprt.feedback;
		now.global_state.modemanager.rprt.feedback = now.global_state.modemanager.xfeedback;
#ifdef VAR_RANGES
		logval("global_state.modemanager.rprt.feedback", now.global_state.modemanager.rprt.feedback);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 132: // STATE 32 - dataview.pml:5026 - [assert(((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][32] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.rprt.feedback>=0)&&(now.global_state.modemanager.rprt.feedback<=10000)), "((global_state.modemanager.rprt.feedback>=0)&&(global_state.modemanager.rprt.feedback<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 133: // STATE 36 - system.pml:46 - [Egse_tm_channel!global_state.modemanager.rprt.reading,global_state.modemanager.rprt.ctrl,global_state.modemanager.rprt.feedback] (59:0:1 - 1)
		IfNotBlocked
		reached[3][36] = 1;
		if (q_full(now.Egse_tm_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Egse_tm_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.ctrl); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.global_state.modemanager.rprt.feedback); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Egse_tm_channel, 0, now.global_state.modemanager.rprt.reading, now.global_state.modemanager.rprt.ctrl, now.global_state.modemanager.rprt.feedback, 3);
		/* merge: transition_id = -(1)(0, 38, 59) */
		reached[3][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_706_4_3_8_transition_id;
		((P3 *)_this)->_706_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_feedback:transition_id", ((P3 *)_this)->_706_4_3_8_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 39, 59) */
		reached[3][39] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 134: // STATE 40 - modemanager.pml:23 - [((transition_id==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][40] = 1;
		if (!((((P3 *)_this)->_706_4_3_8_transition_id==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _706_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_706_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_706_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 135: // STATE 43 - dataview.pml:5021 - [global_state.modemanager.xout = ((global_state.modemanager.xin*global_state.modemanager.cfg.param_a)+global_state.modemanager.cfg.param_b)] (0:0:1 - 1)
		IfNotBlocked
		reached[3][43] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.modemanager.xout;
		now.global_state.modemanager.xout = ((now.global_state.modemanager.xin*now.global_state.modemanager.cfg.param_a)+now.global_state.modemanager.cfg.param_b);
#ifdef VAR_RANGES
		logval("global_state.modemanager.xout", now.global_state.modemanager.xout);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 136: // STATE 44 - dataview.pml:5026 - [assert(((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][44] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.modemanager.xout>=0)&&(now.global_state.modemanager.xout<=10000)), "((global_state.modemanager.xout>=0)&&(global_state.modemanager.xout<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 137: // STATE 47 - system.pml:32 - [Actuator_ctrl_channel!global_state.modemanager.xout] (59:0:1 - 1)
		IfNotBlocked
		reached[3][47] = 1;
		if (q_full(now.Actuator_ctrl_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_ctrl_channel);
		sprintf(simtmp, "%d", now.global_state.modemanager.xout); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_ctrl_channel, 0, now.global_state.modemanager.xout, 0, 0, 1);
		/* merge: transition_id = -(1)(0, 49, 59) */
		reached[3][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P3 *)_this)->_706_4_3_8_transition_id;
		((P3 *)_this)->_706_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_feedback:transition_id", ((P3 *)_this)->_706_4_3_8_transition_id);
#endif
		;
		/* merge: goto continuous_signals(0, 50, 59) */
		reached[3][50] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 138: // STATE 51 - modemanager.pml:29 - [((transition_id==3))] (0:0:1 - 1)
		IfNotBlocked
		reached[3][51] = 1;
		if (!((((P3 *)_this)->_706_4_3_8_transition_id==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _706_4_3_8_transition_id */  (trpt+1)->bup.oval = ((P3 *)_this)->_706_4_3_8_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P3 *)_this)->_706_4_3_8_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 139: // STATE 52 - system.pml:68 - [(1)] (59:0:2 - 1)
		IfNotBlocked
		reached[3][52] = 1;
		if (!(1))
			continue;
		/* merge: transition_id = -(1)(59, 54, 59) */
		reached[3][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P3 *)_this)->_706_4_3_8_transition_id;
		((P3 *)_this)->_706_4_3_8_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Modemanager_feedback:transition_id", ((P3 *)_this)->_706_4_3_8_transition_id);
#endif
		;
		/* merge: global_state.modemanager.state = 0(59, 55, 59) */
		reached[3][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.modemanager.state;
		now.global_state.modemanager.state = 0;
#ifdef VAR_RANGES
		logval("global_state.modemanager.state", now.global_state.modemanager.state);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 59) */
		reached[3][56] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 140: // STATE 70 - system.pml:136 - [(empty(Modemanager_feedback_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[3][70] = 1;
		if (!((q_len(now.Modemanager_feedback_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 141: // STATE 71 - system.pml:137 - [(1)] (74:0:0 - 1)
		IfNotBlocked
		reached[3][71] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 73, 74) */
		reached[3][73] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 142: // STATE 74 - system.pml:139 - [Modemanager_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[3][74] = 1;
		if (q_full(now.Modemanager_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 143: // STATE 79 - system.pml:142 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[3][79] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Egse_tc */
	case 144: // STATE 1 - system.pml:111 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[2][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 145: // STATE 2 - system.pml:113 - [] (0:71:2 - 1)
		IfNotBlocked
		reached[2][2] = 1;
		;
		/* merge: inputVectorCounter = 0(71, 3, 71) */
		reached[2][3] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P2 *)_this)->inputVectorCounter;
		((P2 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Egse_tc:inputVectorCounter", ((P2 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: inputVectorCounter = 0(71, 4, 71) */
		reached[2][4] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P2 *)_this)->inputVectorCounter;
		((P2 *)_this)->inputVectorCounter = 0;
#ifdef VAR_RANGES
		logval("Egse_tc:inputVectorCounter", ((P2 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 72, 71) */
		reached[2][72] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 146: // STATE 5 - system.pml:114 - [((inputVectorCounter<=3))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][5] = 1;
		if (!((((P2 *)_this)->inputVectorCounter<=3)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 147: // STATE 6 - env_inlines.pml:4 - [T_SmallInteger_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[2][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 148: // STATE 7 - env_inlines.pml:6 - [T_SmallInteger_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[2][7] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 149: // STATE 10 - env_inlines.pml:6 - [goto :b5] (0:63:1 - 1)
		IfNotBlocked
		reached[2][10] = 1;
		;
		/* merge: value.param_a = T_SmallInteger_tmp(63, 33, 63) */
		reached[2][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_a;
		((P2 *)_this)->value.param_a = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_a", ((P2 *)_this)->value.param_a);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 150: // STATE 8 - env_inlines.pml:6 - [((T_SmallInteger_tmp<0))] (11:0:1 - 1)
		IfNotBlocked
		reached[2][8] = 1;
		if (!((((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp<0)))
			continue;
		/* merge: T_SmallInteger_tmp = (T_SmallInteger_tmp+1)(0, 9, 11) */
		reached[2][9] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = (((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		/* merge: .(goto)(0, 12, 11) */
		reached[2][12] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 151: // STATE 15 - env_inlines.pml:9 - [T_SmallInteger_tmp = 10] (0:0:1 - 1)
		IfNotBlocked
		reached[2][15] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = 10;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 152: // STATE 18 - env_inlines.pml:9 - [goto :b6] (0:63:1 - 1)
		IfNotBlocked
		reached[2][18] = 1;
		;
		/* merge: value.param_a = T_SmallInteger_tmp(63, 33, 63) */
		reached[2][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_a;
		((P2 *)_this)->value.param_a = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_a", ((P2 *)_this)->value.param_a);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 153: // STATE 16 - env_inlines.pml:9 - [((T_SmallInteger_tmp<10))] (19:0:1 - 1)
		IfNotBlocked
		reached[2][16] = 1;
		if (!((((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp<10)))
			continue;
		/* merge: T_SmallInteger_tmp = (T_SmallInteger_tmp+1)(0, 17, 19) */
		reached[2][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = (((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		/* merge: .(goto)(0, 20, 19) */
		reached[2][20] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 154: // STATE 23 - env_inlines.pml:12 - [T_SmallInteger_tmp = 100] (0:0:1 - 1)
		IfNotBlocked
		reached[2][23] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = 100;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 155: // STATE 26 - env_inlines.pml:12 - [goto :b7] (0:63:1 - 1)
		IfNotBlocked
		reached[2][26] = 1;
		;
		/* merge: value.param_a = T_SmallInteger_tmp(63, 33, 63) */
		reached[2][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_a;
		((P2 *)_this)->value.param_a = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_a", ((P2 *)_this)->value.param_a);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 156: // STATE 24 - env_inlines.pml:12 - [((T_SmallInteger_tmp<100))] (27:0:1 - 1)
		IfNotBlocked
		reached[2][24] = 1;
		if (!((((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp<100)))
			continue;
		/* merge: T_SmallInteger_tmp = (T_SmallInteger_tmp+1)(0, 25, 27) */
		reached[2][25] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = (((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp);
#endif
		;
		/* merge: .(goto)(0, 28, 27) */
		reached[2][28] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 157: // STATE 33 - env_inlines.pml:15 - [value.param_a = T_SmallInteger_tmp] (0:63:1 - 7)
		IfNotBlocked
		reached[2][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_a;
		((P2 *)_this)->value.param_a = ((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_a", ((P2 *)_this)->value.param_a);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 158: // STATE 35 - env_inlines.pml:4 - [T_SmallInteger_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[2][35] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 159: // STATE 36 - env_inlines.pml:6 - [T_SmallInteger_tmp = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[2][36] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = 0;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 160: // STATE 39 - env_inlines.pml:6 - [goto :b8] (0:66:1 - 1)
		IfNotBlocked
		reached[2][39] = 1;
		;
		/* merge: value.param_b = T_SmallInteger_tmp(66, 62, 66) */
		reached[2][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_b;
		((P2 *)_this)->value.param_b = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_b", ((P2 *)_this)->value.param_b);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 161: // STATE 37 - env_inlines.pml:6 - [((T_SmallInteger_tmp<0))] (40:0:1 - 1)
		IfNotBlocked
		reached[2][37] = 1;
		if (!((((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp<0)))
			continue;
		/* merge: T_SmallInteger_tmp = (T_SmallInteger_tmp+1)(0, 38, 40) */
		reached[2][38] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = (((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		/* merge: .(goto)(0, 41, 40) */
		reached[2][41] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 162: // STATE 44 - env_inlines.pml:9 - [T_SmallInteger_tmp = 10] (0:0:1 - 1)
		IfNotBlocked
		reached[2][44] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = 10;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 163: // STATE 47 - env_inlines.pml:9 - [goto :b9] (0:66:1 - 1)
		IfNotBlocked
		reached[2][47] = 1;
		;
		/* merge: value.param_b = T_SmallInteger_tmp(66, 62, 66) */
		reached[2][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_b;
		((P2 *)_this)->value.param_b = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_b", ((P2 *)_this)->value.param_b);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 164: // STATE 45 - env_inlines.pml:9 - [((T_SmallInteger_tmp<10))] (48:0:1 - 1)
		IfNotBlocked
		reached[2][45] = 1;
		if (!((((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp<10)))
			continue;
		/* merge: T_SmallInteger_tmp = (T_SmallInteger_tmp+1)(0, 46, 48) */
		reached[2][46] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = (((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		/* merge: .(goto)(0, 49, 48) */
		reached[2][49] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 165: // STATE 52 - env_inlines.pml:12 - [T_SmallInteger_tmp = 100] (0:0:1 - 1)
		IfNotBlocked
		reached[2][52] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = 100;
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 166: // STATE 55 - env_inlines.pml:12 - [goto :b10] (0:66:1 - 1)
		IfNotBlocked
		reached[2][55] = 1;
		;
		/* merge: value.param_b = T_SmallInteger_tmp(66, 62, 66) */
		reached[2][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_b;
		((P2 *)_this)->value.param_b = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_b", ((P2 *)_this)->value.param_b);
#endif
		;
		_m = 3; goto P999; /* 1 */
	case 167: // STATE 53 - env_inlines.pml:12 - [((T_SmallInteger_tmp<100))] (56:0:1 - 1)
		IfNotBlocked
		reached[2][53] = 1;
		if (!((((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp<100)))
			continue;
		/* merge: T_SmallInteger_tmp = (T_SmallInteger_tmp+1)(0, 54, 56) */
		reached[2][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = (((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp+1);
#ifdef VAR_RANGES
		logval("Egse_tc:T_SmallInteger_tmp", ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp);
#endif
		;
		/* merge: .(goto)(0, 57, 56) */
		reached[2][57] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 168: // STATE 62 - env_inlines.pml:15 - [value.param_b = T_SmallInteger_tmp] (0:66:1 - 7)
		IfNotBlocked
		reached[2][62] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->value.param_b;
		((P2 *)_this)->value.param_b = ((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp;
#ifdef VAR_RANGES
		logval("Egse_tc:value.param_b", ((P2 *)_this)->value.param_b);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 169: // STATE 65 - system.pml:58 - [Modemanager_tc_channel!value.param_a,value.param_b] (71:0:1 - 1)
		IfNotBlocked
		reached[2][65] = 1;
		if (q_full(now.Modemanager_tc_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_tc_channel);
		sprintf(simtmp, "%d", ((P2 *)_this)->value.param_a); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", ((P2 *)_this)->value.param_b); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_tc_channel, 0, ((P2 *)_this)->value.param_a, ((P2 *)_this)->value.param_b, 0, 2);
		/* merge: inputVectorCounter = (inputVectorCounter+1)(0, 68, 71) */
		reached[2][68] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = ((P2 *)_this)->inputVectorCounter;
		((P2 *)_this)->inputVectorCounter = (((P2 *)_this)->inputVectorCounter+1);
#ifdef VAR_RANGES
		logval("Egse_tc:inputVectorCounter", ((P2 *)_this)->inputVectorCounter);
#endif
		;
		/* merge: .(goto)(0, 72, 71) */
		reached[2][72] = 1;
		;
		_m = 2; goto P999; /* 2 */
	case 170: // STATE 74 - system.pml:121 - [-end-] (0:0:0 - 3)
		IfNotBlocked
		reached[2][74] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Egse_tm */
	case 171: // STATE 1 - system.pml:93 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[1][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 172: // STATE 2 - system.pml:96 - [(nempty(Egse_tm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][2] = 1;
		if (!((q_len(now.Egse_tm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 173: // STATE 3 - system.pml:99 - [(nempty(Egse_tm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][3] = 1;
		if (!((q_len(now.Egse_tm_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 174: // STATE 4 - system.pml:100 - [Egse_tm_channel?Egse_tm_signal_parameter.reading,Egse_tm_signal_parameter.ctrl,Egse_tm_signal_parameter.feedback] (9:0:4 - 1)
		reached[1][4] = 1;
		if (q_len(now.Egse_tm_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(4);
		(trpt+1)->bup.ovals[0] = now.Egse_tm_signal_parameter.reading;
		(trpt+1)->bup.ovals[1] = now.Egse_tm_signal_parameter.ctrl;
		(trpt+1)->bup.ovals[2] = now.Egse_tm_signal_parameter.feedback;
		;
		now.Egse_tm_signal_parameter.reading = qrecv(now.Egse_tm_channel, XX-1, 0, 0);
#ifdef VAR_RANGES
		logval("Egse_tm_signal_parameter.reading", now.Egse_tm_signal_parameter.reading);
#endif
		;
		now.Egse_tm_signal_parameter.ctrl = qrecv(now.Egse_tm_channel, XX-1, 1, 0);
#ifdef VAR_RANGES
		logval("Egse_tm_signal_parameter.ctrl", now.Egse_tm_signal_parameter.ctrl);
#endif
		;
		now.Egse_tm_signal_parameter.feedback = qrecv(now.Egse_tm_channel, XX-1, 2, 1);
#ifdef VAR_RANGES
		logval("Egse_tm_signal_parameter.feedback", now.Egse_tm_signal_parameter.feedback);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Egse_tm_channel);
		sprintf(simtmp, "%d", now.Egse_tm_signal_parameter.reading); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Egse_tm_signal_parameter.ctrl); strcat(simvals, simtmp);		strcat(simvals, ",");
		sprintf(simtmp, "%d", now.Egse_tm_signal_parameter.feedback); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Egse_tm_channel_used = 1(0, 5, 9) */
		reached[1][5] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[3] = ((int)Egse_tm_channel_used);
		Egse_tm_channel_used = 1;
#ifdef VAR_RANGES
		logval("Egse_tm_channel_used", ((int)Egse_tm_channel_used));
#endif
		;
		/* merge: goto Egse_tm_loop(0, 6, 9) */
		reached[1][6] = 1;
		;
		_m = 4; goto P999; /* 2 */
	case 175: // STATE 7 - system.pml:103 - [(empty(Egse_tm_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[1][7] = 1;
		if (!((q_len(now.Egse_tm_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 176: // STATE 8 - system.pml:104 - [(1)] (12:0:0 - 1)
		IfNotBlocked
		reached[1][8] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 10, 12) */
		reached[1][10] = 1;
		;
		/* merge: .(goto)(0, 13, 12) */
		reached[1][13] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 177: // STATE 15 - system.pml:108 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[1][15] = 1;
		if (TstOnly)
			return (II+1 == now._nr_pr);
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC Actuator_ctrl */
	case 178: // STATE 1 - system.pml:72 - [(inited)] (0:0:0 - 1)
		IfNotBlocked
		reached[0][1] = 1;
		if (!(now.inited))
			continue;
		_m = 3; goto P999; /* 0 */
	case 179: // STATE 2 - system.pml:75 - [(nempty(Actuator_ctrl_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][2] = 1;
		if (!((q_len(now.Actuator_ctrl_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 180: // STATE 3 - system.pml:76 - [Actuator_lock?_] (0:0:1 - 1)
		reached[0][3] = 1;
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
		sprintf(simtmp, "%d", ((int)_)); strcat(simvals, simtmp);		}
#endif
		;
		_m = 4; goto P999; /* 0 */
	case 181: // STATE 4 - system.pml:79 - [(nempty(Actuator_ctrl_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][4] = 1;
		if (!((q_len(now.Actuator_ctrl_channel)>0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 182: // STATE 5 - system.pml:80 - [Actuator_ctrl_channel?Actuator_ctrl_signal_parameter] (85:0:2 - 1)
		reached[0][5] = 1;
		if (q_len(now.Actuator_ctrl_channel) == 0) continue;

		XX=1;
		if (TstOnly) return 1 /* T2 */;
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = now.Actuator_ctrl_signal_parameter;
		;
		now.Actuator_ctrl_signal_parameter = qrecv(now.Actuator_ctrl_channel, XX-1, 0, 1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl_signal_parameter", now.Actuator_ctrl_signal_parameter);
#endif
		;
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", now.Actuator_ctrl_channel);
		sprintf(simtmp, "%d", now.Actuator_ctrl_signal_parameter); strcat(simvals, simtmp);		}
#endif
		;
		/* merge: Actuator_ctrl_channel_used = 1(0, 6, 85) */
		reached[0][6] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((int)Actuator_ctrl_channel_used);
		Actuator_ctrl_channel_used = 1;
#ifdef VAR_RANGES
		logval("Actuator_ctrl_channel_used", ((int)Actuator_ctrl_channel_used));
#endif
		;
		_m = 4; goto P999; /* 1 */
	case 183: // STATE 7 - actuator.pml:39 - [((global_state.actuator.state==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][7] = 1;
		if (!((now.global_state.actuator.state==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 184: // STATE 8 - dataview.pml:5021 - [global_state.actuator.x = Actuator_ctrl_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[0][8] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.x;
		now.global_state.actuator.x = now.Actuator_ctrl_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator.x", now.global_state.actuator.x);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 185: // STATE 9 - dataview.pml:5026 - [assert(((global_state.actuator.x>=0)&&(global_state.actuator.x<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][9] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.x>=0)&&(now.global_state.actuator.x<=10000)), "((global_state.actuator.x>=0)&&(global_state.actuator.x<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 186: // STATE 12 - actuator.pml:4 - [transition_id = 0] (0:40:2 - 1)
		IfNotBlocked
		reached[0][12] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_2_transition_id;
		((P0 *)_this)->_703_1_1_2_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_2_transition_id);
#endif
		;
		/* merge: transition_id = 1(40, 13, 40) */
		reached[0][13] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_703_1_1_2_transition_id;
		((P0 *)_this)->_703_1_1_2_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_2_transition_id);
#endif
		;
		/* merge: .(goto)(0, 41, 40) */
		reached[0][41] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 187: // STATE 14 - actuator.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][14] = 1;
		if (!((((P0 *)_this)->_703_1_1_2_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _703_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_703_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_703_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 188: // STATE 16 - actuator.pml:9 - [((transition_id==0))] (39:0:3 - 1)
		IfNotBlocked
		reached[0][16] = 1;
		if (!((((P0 *)_this)->_703_1_1_2_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _703_1_1_2_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_703_1_1_2_transition_id = 0;
		/* merge: transition_id = -(1)(39, 17, 39) */
		reached[0][17] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_703_1_1_2_transition_id;
		((P0 *)_this)->_703_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(39, 18, 39) */
		reached[0][18] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 19, 39) */
		reached[0][19] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 189: // STATE 20 - actuator.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][20] = 1;
		if (!((((P0 *)_this)->_703_1_1_2_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _703_1_1_2_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_703_1_1_2_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_703_1_1_2_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 190: // STATE 23 - actuator.pml:16 - [(((global_state.actuator.x>10)==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][23] = 1;
		if (!(((now.global_state.actuator.x>10)==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 191: // STATE 24 - system.pml:50 - [Modemanager_feedback_channel!global_state.actuator.x] (39:0:2 - 1)
		IfNotBlocked
		reached[0][24] = 1;
		if (q_full(now.Modemanager_feedback_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", now.global_state.actuator.x); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_feedback_channel, 0, now.global_state.actuator.x, 0, 0, 1);
		/* merge: transition_id = -(1)(39, 26, 39) */
		reached[0][26] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_2_transition_id;
		((P0 *)_this)->_703_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 1(39, 27, 39) */
		reached[0][27] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 28, 39) */
		reached[0][28] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 192: // STATE 29 - actuator.pml:21 - [(((global_state.actuator.x>10)==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][29] = 1;
		if (!(((now.global_state.actuator.x>10)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 193: // STATE 30 - system.pml:50 - [Modemanager_feedback_channel!0] (39:0:2 - 1)
		IfNotBlocked
		reached[0][30] = 1;
		if (q_full(now.Modemanager_feedback_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_feedback_channel, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(39, 32, 39) */
		reached[0][32] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_2_transition_id;
		((P0 *)_this)->_703_1_1_2_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_2_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(39, 33, 39) */
		reached[0][33] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 34, 39) */
		reached[0][34] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 194: // STATE 44 - actuator.pml:42 - [((global_state.actuator.state==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][44] = 1;
		if (!((now.global_state.actuator.state==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 195: // STATE 45 - dataview.pml:5021 - [global_state.actuator.x = Actuator_ctrl_signal_parameter] (0:0:1 - 1)
		IfNotBlocked
		reached[0][45] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.oval = now.global_state.actuator.x;
		now.global_state.actuator.x = now.Actuator_ctrl_signal_parameter;
#ifdef VAR_RANGES
		logval("global_state.actuator.x", now.global_state.actuator.x);
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 196: // STATE 46 - dataview.pml:5026 - [assert(((global_state.actuator.x>=0)&&(global_state.actuator.x<=10000)))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][46] = 1;
		if (TstOnly) return 1; /* T7 */
		spin_assert(((now.global_state.actuator.x>=0)&&(now.global_state.actuator.x<=10000)), "((global_state.actuator.x>=0)&&(global_state.actuator.x<=10000))", II, tt, t);
		_m = 3; goto P999; /* 0 */
	case 197: // STATE 49 - actuator.pml:4 - [transition_id = 0] (0:77:2 - 1)
		IfNotBlocked
		reached[0][49] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_4_transition_id;
		((P0 *)_this)->_703_1_1_4_transition_id = 0;
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_4_transition_id);
#endif
		;
		/* merge: transition_id = 1(77, 50, 77) */
		reached[0][50] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_703_1_1_4_transition_id;
		((P0 *)_this)->_703_1_1_4_transition_id = 1;
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_4_transition_id);
#endif
		;
		/* merge: .(goto)(0, 78, 77) */
		reached[0][78] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 198: // STATE 51 - actuator.pml:7 - [((transition_id==-(1)))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][51] = 1;
		if (!((((P0 *)_this)->_703_1_1_4_transition_id== -(1))))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _703_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_703_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_703_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 199: // STATE 53 - actuator.pml:9 - [((transition_id==0))] (76:0:3 - 1)
		IfNotBlocked
		reached[0][53] = 1;
		if (!((((P0 *)_this)->_703_1_1_4_transition_id==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _703_1_1_4_transition_id */  (trpt+1)->bup.ovals = grab_ints(3);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_703_1_1_4_transition_id = 0;
		/* merge: transition_id = -(1)(76, 54, 76) */
		reached[0][54] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = ((P0 *)_this)->_703_1_1_4_transition_id;
		((P0 *)_this)->_703_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(76, 55, 76) */
		reached[0][55] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[2] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 56, 76) */
		reached[0][56] = 1;
		;
		_m = 3; goto P999; /* 3 */
	case 200: // STATE 57 - actuator.pml:13 - [((transition_id==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][57] = 1;
		if (!((((P0 *)_this)->_703_1_1_4_transition_id==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: _703_1_1_4_transition_id */  (trpt+1)->bup.oval = ((P0 *)_this)->_703_1_1_4_transition_id;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->_703_1_1_4_transition_id = 0;
		_m = 3; goto P999; /* 0 */
	case 201: // STATE 60 - actuator.pml:16 - [(((global_state.actuator.x>10)==1))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][60] = 1;
		if (!(((now.global_state.actuator.x>10)==1)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 202: // STATE 61 - system.pml:50 - [Modemanager_feedback_channel!global_state.actuator.x] (76:0:2 - 1)
		IfNotBlocked
		reached[0][61] = 1;
		if (q_full(now.Modemanager_feedback_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", now.global_state.actuator.x); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_feedback_channel, 0, now.global_state.actuator.x, 0, 0, 1);
		/* merge: transition_id = -(1)(76, 63, 76) */
		reached[0][63] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_4_transition_id;
		((P0 *)_this)->_703_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 1(76, 64, 76) */
		reached[0][64] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 1;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 65, 76) */
		reached[0][65] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 203: // STATE 66 - actuator.pml:21 - [(((global_state.actuator.x>10)==0))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][66] = 1;
		if (!(((now.global_state.actuator.x>10)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 204: // STATE 67 - system.pml:50 - [Modemanager_feedback_channel!0] (76:0:2 - 1)
		IfNotBlocked
		reached[0][67] = 1;
		if (q_full(now.Modemanager_feedback_channel))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Modemanager_feedback_channel);
		sprintf(simtmp, "%d", 0); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Modemanager_feedback_channel, 0, 0, 0, 0, 1);
		/* merge: transition_id = -(1)(76, 69, 76) */
		reached[0][69] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P0 *)_this)->_703_1_1_4_transition_id;
		((P0 *)_this)->_703_1_1_4_transition_id =  -(1);
#ifdef VAR_RANGES
		logval("Actuator_ctrl:transition_id", ((P0 *)_this)->_703_1_1_4_transition_id);
#endif
		;
		/* merge: global_state.actuator.state = 0(76, 70, 76) */
		reached[0][70] = 1;
		if (TstOnly) return 1; /* T3 */
		(trpt+1)->bup.ovals[1] = now.global_state.actuator.state;
		now.global_state.actuator.state = 0;
#ifdef VAR_RANGES
		logval("global_state.actuator.state", now.global_state.actuator.state);
#endif
		;
		/* merge: goto continuous_signals(0, 71, 76) */
		reached[0][71] = 1;
		;
		_m = 2; goto P999; /* 3 */
	case 205: // STATE 87 - system.pml:84 - [(empty(Actuator_ctrl_channel))] (0:0:0 - 1)
		IfNotBlocked
		reached[0][87] = 1;
		if (!((q_len(now.Actuator_ctrl_channel)==0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 206: // STATE 88 - system.pml:85 - [(1)] (91:0:0 - 1)
		IfNotBlocked
		reached[0][88] = 1;
		if (!(1))
			continue;
		/* merge: .(goto)(0, 90, 91) */
		reached[0][90] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 207: // STATE 91 - system.pml:87 - [Actuator_lock!1] (0:0:0 - 1)
		IfNotBlocked
		reached[0][91] = 1;
		if (q_full(now.Actuator_lock))
			continue;
		if (TstOnly) return 1; /* T1 */
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", now.Actuator_lock);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(now.Actuator_lock, 0, 1, 0, 0, 1);
		_m = 2; goto P999; /* 0 */
	case 208: // STATE 96 - system.pml:90 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[0][96] = 1;
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

