	switch (t->back) {
	default: Uerror("bad return move");
	case  0: goto R999; /* nothing to undo */

		 /* CLAIM never_0 */
;
		;
		;
		
	case 4: // STATE 8
		goto R999;

	case 5: // STATE 17
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC :init: */
	case 6: // STATE 4
		sv_restor();
		goto R999;

	case 7: // STATE 6
		;
		now.global_state.actuator.sender = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 9: // STATE 11
		;
		((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.ovals[1];
		((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 10: // STATE 12
		;
	/* 0 */	((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 11: // STATE 16
		;
		now.global_state.actuator.state = trpt->bup.ovals[2];
		((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 12: // STATE 18
		;
	/* 0 */	((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.oval;
		;
		;
		goto R999;
;
		;
		
	case 14: // STATE 25
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Modemanager_feedback_channel);
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 16: // STATE 31
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P7 *)_this)->_710_8_8_17_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Modemanager_feedback_channel);
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 17: // STATE 43
		;
		_m = unsend(now.Actuator_lock);
		;
		goto R999;

	case 18: // STATE 44
		;
		now.global_state.modemanager.sender = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 20: // STATE 49
		;
		((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.ovals[1];
		((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 21: // STATE 50
		;
	/* 0 */	((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 22: // STATE 54
		;
		now.global_state.modemanager.state = trpt->bup.ovals[2];
		((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 23: // STATE 56
		;
	/* 0 */	((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 24: // STATE 59
		;
		now.global_state.modemanager.rprt.reading = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 26: // STATE 63
		;
		now.global_state.modemanager.rprt.ctrl = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 28: // STATE 67
		;
		now.global_state.modemanager.rprt.feedback = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 30: // STATE 74
		;
		((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.oval;
		_m = unsend(now.Egse_tm_channel);
		;
		goto R999;

	case 31: // STATE 76
		;
	/* 0 */	((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 32: // STATE 79
		;
		now.global_state.modemanager.xout = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 34: // STATE 85
		;
		((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_ctrl_channel);
		;
		goto R999;

	case 35: // STATE 87
		;
	/* 0 */	((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 36: // STATE 91
		;
		now.global_state.modemanager.state = trpt->bup.ovals[1];
		((P7 *)_this)->_710_8_9_19_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 37: // STATE 102
		;
		now.inited = trpt->bup.oval;
		_m = unsend(now.Modemanager_lock);
		;
		goto R999;

	case 38: // STATE 104
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Sensor_reading */
;
		;
		
	case 40: // STATE 4
		;
		((P6 *)_this)->inputVectorCounter = trpt->bup.ovals[2];
		((P6 *)_this)->inputVectorCounter = trpt->bup.ovals[1];
		((P6 *)_this)->value = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;
;
		;
		
	case 42: // STATE 6
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 43: // STATE 7
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 44: // STATE 33
		;
		((P6 *)_this)->value = trpt->bup.oval;
		;
		goto R999;

	case 45: // STATE 9
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 46: // STATE 15
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 47: // STATE 33
		;
		((P6 *)_this)->value = trpt->bup.oval;
		;
		goto R999;

	case 48: // STATE 17
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 49: // STATE 23
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 50: // STATE 33
		;
		((P6 *)_this)->value = trpt->bup.oval;
		;
		goto R999;

	case 51: // STATE 25
		;
		((P6 *)_this)->_709_7_6_13_Sensor_reading_x_tmp = trpt->bup.oval;
		;
		goto R999;

	case 52: // STATE 33
		;
		((P6 *)_this)->value = trpt->bup.oval;
		;
		goto R999;

	case 53: // STATE 38
		;
		((P6 *)_this)->inputVectorCounter = trpt->bup.oval;
		_m = unsend(now.Modemanager_reading_channel);
		;
		goto R999;

	case 54: // STATE 44
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Modemanager_tc */
;
		;
		;
		;
		
	case 57: // STATE 3
		;
		XX = 1;
		unrecv(now.Modemanager_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 59: // STATE 6
		;
		Modemanager_tc_channel_used = trpt->bup.ovals[2];
		XX = 1;
		unrecv(now.Modemanager_tc_channel, XX-1, 0, now.Modemanager_tc_signal_parameter.param_a, 1);
		unrecv(now.Modemanager_tc_channel, XX-1, 1, now.Modemanager_tc_signal_parameter.param_b, 0);
		now.Modemanager_tc_signal_parameter.param_a = trpt->bup.ovals[0];
		now.Modemanager_tc_signal_parameter.param_b = trpt->bup.ovals[1];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;
;
		;
		
	case 61: // STATE 8
		;
		now.global_state.modemanager.cfg.param_a = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 63: // STATE 12
		;
		now.global_state.modemanager.cfg.param_b = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 65: // STATE 18
		;
		((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.ovals[1];
		((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 66: // STATE 19
		;
	/* 0 */	((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 67: // STATE 23
		;
		now.global_state.modemanager.state = trpt->bup.ovals[2];
		((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 68: // STATE 25
		;
	/* 0 */	((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 69: // STATE 28
		;
		now.global_state.modemanager.rprt.reading = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 71: // STATE 32
		;
		now.global_state.modemanager.rprt.ctrl = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 73: // STATE 36
		;
		now.global_state.modemanager.rprt.feedback = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 75: // STATE 43
		;
		((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.oval;
		_m = unsend(now.Egse_tm_channel);
		;
		goto R999;

	case 76: // STATE 45
		;
	/* 0 */	((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 77: // STATE 48
		;
		now.global_state.modemanager.xout = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 79: // STATE 54
		;
		((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_ctrl_channel);
		;
		goto R999;

	case 80: // STATE 56
		;
	/* 0 */	((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 81: // STATE 60
		;
		now.global_state.modemanager.state = trpt->bup.ovals[1];
		((P5 *)_this)->_708_6_5_12_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		;
		
	case 83: // STATE 76
		goto R999;

	case 84: // STATE 79
		;
		_m = unsend(now.Modemanager_lock);
		;
		goto R999;

	case 85: // STATE 84
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Modemanager_reading */
;
		;
		;
		;
		
	case 88: // STATE 3
		;
		XX = 1;
		unrecv(now.Modemanager_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 90: // STATE 6
		;
		Modemanager_reading_channel_used = trpt->bup.ovals[1];
		XX = 1;
		unrecv(now.Modemanager_reading_channel, XX-1, 0, now.Modemanager_reading_signal_parameter, 1);
		now.Modemanager_reading_signal_parameter = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 92: // STATE 8
		;
		now.global_state.modemanager.xin = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 94: // STATE 13
		;
		((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.ovals[1];
		((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 95: // STATE 14
		;
	/* 0 */	((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 96: // STATE 18
		;
		now.global_state.modemanager.state = trpt->bup.ovals[2];
		((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 97: // STATE 20
		;
	/* 0 */	((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 98: // STATE 23
		;
		now.global_state.modemanager.rprt.reading = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 100: // STATE 27
		;
		now.global_state.modemanager.rprt.ctrl = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 102: // STATE 31
		;
		now.global_state.modemanager.rprt.feedback = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 104: // STATE 38
		;
		((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.oval;
		_m = unsend(now.Egse_tm_channel);
		;
		goto R999;

	case 105: // STATE 40
		;
	/* 0 */	((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 106: // STATE 43
		;
		now.global_state.modemanager.xout = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 108: // STATE 49
		;
		((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_ctrl_channel);
		;
		goto R999;

	case 109: // STATE 51
		;
	/* 0 */	((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 110: // STATE 55
		;
		now.global_state.modemanager.state = trpt->bup.ovals[1];
		((P4 *)_this)->_707_5_4_10_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		;
		
	case 112: // STATE 71
		goto R999;

	case 113: // STATE 74
		;
		_m = unsend(now.Modemanager_lock);
		;
		goto R999;

	case 114: // STATE 79
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Modemanager_feedback */
;
		;
		;
		;
		
	case 117: // STATE 3
		;
		XX = 1;
		unrecv(now.Modemanager_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 119: // STATE 6
		;
		Modemanager_feedback_channel_used = trpt->bup.ovals[1];
		XX = 1;
		unrecv(now.Modemanager_feedback_channel, XX-1, 0, now.Modemanager_feedback_signal_parameter, 1);
		now.Modemanager_feedback_signal_parameter = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 121: // STATE 8
		;
		now.global_state.modemanager.xfeedback = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 123: // STATE 13
		;
		((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.ovals[1];
		((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 124: // STATE 14
		;
	/* 0 */	((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 125: // STATE 18
		;
		now.global_state.modemanager.state = trpt->bup.ovals[2];
		((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 126: // STATE 20
		;
	/* 0 */	((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 127: // STATE 23
		;
		now.global_state.modemanager.rprt.reading = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 129: // STATE 27
		;
		now.global_state.modemanager.rprt.ctrl = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 131: // STATE 31
		;
		now.global_state.modemanager.rprt.feedback = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 133: // STATE 38
		;
		((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.oval;
		_m = unsend(now.Egse_tm_channel);
		;
		goto R999;

	case 134: // STATE 40
		;
	/* 0 */	((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 135: // STATE 43
		;
		now.global_state.modemanager.xout = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 137: // STATE 49
		;
		((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_ctrl_channel);
		;
		goto R999;

	case 138: // STATE 51
		;
	/* 0 */	((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 139: // STATE 55
		;
		now.global_state.modemanager.state = trpt->bup.ovals[1];
		((P3 *)_this)->_706_4_3_8_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		;
		
	case 141: // STATE 71
		goto R999;

	case 142: // STATE 74
		;
		_m = unsend(now.Modemanager_lock);
		;
		goto R999;

	case 143: // STATE 79
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Egse_tc */
;
		;
		
	case 145: // STATE 4
		;
		((P2 *)_this)->inputVectorCounter = trpt->bup.ovals[1];
		((P2 *)_this)->inputVectorCounter = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 147: // STATE 6
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 148: // STATE 7
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 149: // STATE 33
		;
		((P2 *)_this)->value.param_a = trpt->bup.oval;
		;
		goto R999;

	case 150: // STATE 9
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 151: // STATE 15
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 152: // STATE 33
		;
		((P2 *)_this)->value.param_a = trpt->bup.oval;
		;
		goto R999;

	case 153: // STATE 17
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 154: // STATE 23
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 155: // STATE 33
		;
		((P2 *)_this)->value.param_a = trpt->bup.oval;
		;
		goto R999;

	case 156: // STATE 25
		;
		((P2 *)_this)->_705_3_2_5_9_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 157: // STATE 33
		;
		((P2 *)_this)->value.param_a = trpt->bup.oval;
		;
		goto R999;

	case 158: // STATE 35
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 159: // STATE 36
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 160: // STATE 62
		;
		((P2 *)_this)->value.param_b = trpt->bup.oval;
		;
		goto R999;

	case 161: // STATE 38
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 162: // STATE 44
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 163: // STATE 62
		;
		((P2 *)_this)->value.param_b = trpt->bup.oval;
		;
		goto R999;

	case 164: // STATE 46
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 165: // STATE 52
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 166: // STATE 62
		;
		((P2 *)_this)->value.param_b = trpt->bup.oval;
		;
		goto R999;

	case 167: // STATE 54
		;
		((P2 *)_this)->_705_3_2_5_10_T_SmallInteger_tmp = trpt->bup.oval;
		;
		goto R999;

	case 168: // STATE 62
		;
		((P2 *)_this)->value.param_b = trpt->bup.oval;
		;
		goto R999;

	case 169: // STATE 68
		;
		((P2 *)_this)->inputVectorCounter = trpt->bup.oval;
		_m = unsend(now.Modemanager_tc_channel);
		;
		goto R999;

	case 170: // STATE 74
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Egse_tm */
;
		;
		;
		;
		;
		;
		
	case 174: // STATE 5
		;
		Egse_tm_channel_used = trpt->bup.ovals[3];
		XX = 1;
		unrecv(now.Egse_tm_channel, XX-1, 0, now.Egse_tm_signal_parameter.reading, 1);
		unrecv(now.Egse_tm_channel, XX-1, 1, now.Egse_tm_signal_parameter.ctrl, 0);
		unrecv(now.Egse_tm_channel, XX-1, 2, now.Egse_tm_signal_parameter.feedback, 0);
		now.Egse_tm_signal_parameter.reading = trpt->bup.ovals[0];
		now.Egse_tm_signal_parameter.ctrl = trpt->bup.ovals[1];
		now.Egse_tm_signal_parameter.feedback = trpt->bup.ovals[2];
		;
		;
		ungrab_ints(trpt->bup.ovals, 4);
		goto R999;
;
		;
		;
		
	case 176: // STATE 8
		goto R999;

	case 177: // STATE 15
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Actuator_ctrl */
;
		;
		;
		;
		
	case 180: // STATE 3
		;
		XX = 1;
		unrecv(now.Actuator_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 182: // STATE 6
		;
		Actuator_ctrl_channel_used = trpt->bup.ovals[1];
		XX = 1;
		unrecv(now.Actuator_ctrl_channel, XX-1, 0, now.Actuator_ctrl_signal_parameter, 1);
		now.Actuator_ctrl_signal_parameter = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 184: // STATE 8
		;
		now.global_state.actuator.x = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 186: // STATE 13
		;
		((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.ovals[1];
		((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 187: // STATE 14
		;
	/* 0 */	((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 188: // STATE 18
		;
		now.global_state.actuator.state = trpt->bup.ovals[2];
		((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 189: // STATE 20
		;
	/* 0 */	((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.oval;
		;
		;
		goto R999;
;
		;
		
	case 191: // STATE 27
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Modemanager_feedback_channel);
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 193: // STATE 33
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P0 *)_this)->_703_1_1_2_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Modemanager_feedback_channel);
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 195: // STATE 45
		;
		now.global_state.actuator.x = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 197: // STATE 50
		;
		((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.ovals[1];
		((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 198: // STATE 51
		;
	/* 0 */	((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 199: // STATE 55
		;
		now.global_state.actuator.state = trpt->bup.ovals[2];
		((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_ints(trpt->bup.ovals, 3);
		goto R999;

	case 200: // STATE 57
		;
	/* 0 */	((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.oval;
		;
		;
		goto R999;
;
		;
		
	case 202: // STATE 64
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Modemanager_feedback_channel);
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 204: // STATE 70
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P0 *)_this)->_703_1_1_4_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Modemanager_feedback_channel);
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		;
		
	case 206: // STATE 88
		goto R999;

	case 207: // STATE 91
		;
		_m = unsend(now.Actuator_lock);
		;
		goto R999;

	case 208: // STATE 96
		;
		p_restor(II);
		;
		;
		goto R999;
	}

