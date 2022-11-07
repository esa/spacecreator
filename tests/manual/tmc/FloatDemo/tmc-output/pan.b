	switch (t->back) {
	default: Uerror("bad return move");
	case  0: goto R999; /* nothing to undo */

		 /* PROC :init: */
	case 3: // STATE 13
		sv_restor();
		goto R999;

	case 4: // STATE 15
		;
		now.global_state.sensor.sender = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 6: // STATE 20
		;
		((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 7: // STATE 21
		;
	/* 0 */	((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 8: // STATE 23
		;
	/* 0 */	((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 9: // STATE 24
		;
		now.global_state.sensor.x = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 11: // STATE 33
		;
		now.global_state.sensor.state = trpt->bup.ovals[3];
		((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.ovals[2];
		now.global_state.timers.sensor.trigger.timer_enabled = trpt->bup.ovals[1];
		now.global_state.timers.sensor.trigger.interval = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 4);
		goto R999;

	case 12: // STATE 35
		;
	/* 0 */	((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 13: // STATE 38
		;
		now.global_state.sensor.x = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 15: // STATE 42
		;
		_m = unsend(now.Controlsystem_reading_channel);
		;
		goto R999;

	case 16: // STATE 49
		;
		now.global_state.sensor.state = trpt->bup.ovals[3];
		((P7 *)_this)->_1211_8_8_12_transition_id = trpt->bup.ovals[2];
		now.global_state.timers.sensor.trigger.timer_enabled = trpt->bup.ovals[1];
		now.global_state.timers.sensor.trigger.interval = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 4);
		goto R999;

	case 17: // STATE 59
		;
		_m = unsend(now.Sensor_lock);
		;
		goto R999;

	case 18: // STATE 60
		;
		now.global_state.actuator.sender = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 20: // STATE 64
		;
		now.global_state.actuator.setting = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 22: // STATE 69
		;
		((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 23: // STATE 70
		;
	/* 0 */	((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 24: // STATE 74
		;
		now.global_state.actuator.state = trpt->bup.ovals[2];
		((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 3);
		goto R999;

	case 25: // STATE 76
		;
	/* 0 */	((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 26: // STATE 79
		;
		now.global_state.actuator.setting = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 28: // STATE 85
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 29: // STATE 89
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_9_15_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 30: // STATE 101
		;
		_m = unsend(now.Actuator_lock);
		;
		goto R999;

	case 31: // STATE 102
		;
		now.global_state.controlsystem.sender = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 33: // STATE 106
		;
		now.global_state.controlsystem.i = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 35: // STATE 110
		;
		now.global_state.controlsystem.d = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 37: // STATE 114
		;
		now.global_state.controlsystem.previous = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 39: // STATE 119
		;
		((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 40: // STATE 120
		;
	/* 0 */	((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 41: // STATE 124
		;
		now.global_state.controlsystem.state = trpt->bup.ovals[2];
		((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 3);
		goto R999;

	case 42: // STATE 126
		;
	/* 0 */	((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 43: // STATE 130
		;
		now.global_state.controlsystem.state = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 44: // STATE 132
		;
	/* 0 */	((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 45: // STATE 135
		;
		now.global_state.controlsystem.i = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 47: // STATE 139
		;
		now.global_state.controlsystem.d = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 49: // STATE 143
		;
		now.global_state.controlsystem.previous = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 51: // STATE 148
		;
		now.global_state.controlsystem.outputvalue = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 53: // STATE 154
		;
		((P7 *)_this)->_1211_8_10_20_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_controlSignal_channel);
		;
		goto R999;

	case 54: // STATE 164
		;
		_m = unsend(now.Controlsystem_lock);
		;
		goto R999;

	case 55: // STATE 165
		;
		now.global_state.host.sender = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 57: // STATE 170
		;
		((P7 *)_this)->_1211_8_11_22_transition_id = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_11_22_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 58: // STATE 171
		;
	/* 0 */	((P7 *)_this)->_1211_8_11_22_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 59: // STATE 173
		;
	/* 0 */	((P7 *)_this)->_1211_8_11_22_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 60: // STATE 174
		;
		now.global_state.host.cfg.kp = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 62: // STATE 178
		;
		now.global_state.host.cfg.kd = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 64: // STATE 182
		;
		now.global_state.host.cfg.ki = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 66: // STATE 190
		;
		now.global_state.host.state = trpt->bup.ovals[1];
		((P7 *)_this)->_1211_8_11_22_transition_id = trpt->bup.ovals[0];
		_m = unsend(now.Controlsystem_configuration_channel);
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 67: // STATE 201
		;
		now.inited = trpt->bup.oval;
		_m = unsend(now.Host_lock);
		;
		goto R999;

	case 68: // STATE 203
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Sensor_trigger */
;
		;
		;
		;
		
	case 71: // STATE 3
		;
		XX = 1;
		unrecv(now.Sensor_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 73: // STATE 5
		;
		XX = 1;
		unrecv(now.Sensor_trigger_channel, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 75: // STATE 8
		;
		((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.ovals[1];
		((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 76: // STATE 9
		;
	/* 0 */	((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 77: // STATE 11
		;
	/* 0 */	((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 78: // STATE 12
		;
		now.global_state.sensor.x = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 80: // STATE 21
		;
		now.global_state.sensor.state = trpt->bup.ovals[3];
		((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.ovals[2];
		now.global_state.timers.sensor.trigger.timer_enabled = trpt->bup.ovals[1];
		now.global_state.timers.sensor.trigger.interval = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 4);
		goto R999;

	case 81: // STATE 23
		;
	/* 0 */	((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 82: // STATE 26
		;
		now.global_state.sensor.x = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 84: // STATE 30
		;
		_m = unsend(now.Controlsystem_reading_channel);
		;
		goto R999;

	case 85: // STATE 37
		;
		now.global_state.sensor.state = trpt->bup.ovals[3];
		((P6 *)_this)->_1210_7_6_9_transition_id = trpt->bup.ovals[2];
		now.global_state.timers.sensor.trigger.timer_enabled = trpt->bup.ovals[1];
		now.global_state.timers.sensor.trigger.interval = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 4);
		goto R999;
;
		;
		;
		
	case 87: // STATE 53
		goto R999;

	case 88: // STATE 56
		;
		_m = unsend(now.Sensor_lock);
		;
		goto R999;

	case 89: // STATE 61
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Sensor_tc */
;
		;
		;
		;
		
	case 92: // STATE 3
		;
		XX = 1;
		unrecv(now.Sensor_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 94: // STATE 5
		;
		XX = 1;
		unrecv(now.Sensor_tc_channel, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		;
		
	case 96: // STATE 13
		goto R999;

	case 97: // STATE 16
		;
		_m = unsend(now.Sensor_lock);
		;
		goto R999;

	case 98: // STATE 21
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Host_tm */
;
		;
		;
		;
		
	case 101: // STATE 3
		;
		XX = 1;
		unrecv(now.Host_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 103: // STATE 5
		;
		XX = 1;
		unrecv(now.Host_tm_channel, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		;
		
	case 105: // STATE 13
		goto R999;

	case 106: // STATE 16
		;
		_m = unsend(now.Host_lock);
		;
		goto R999;

	case 107: // STATE 21
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Controlsystem_reading */
;
		;
		;
		;
		
	case 110: // STATE 3
		;
		XX = 1;
		unrecv(now.Controlsystem_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 112: // STATE 6
		;
		Controlsystem_reading_channel_used = trpt->bup.ovals[1];
		XX = 1;
		unrecv(now.Controlsystem_reading_channel, XX-1, 0, now.Controlsystem_reading_signal_parameter, 1);
		now.Controlsystem_reading_signal_parameter = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 114: // STATE 8
		;
		now.global_state.controlsystem.inputvalue = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 116: // STATE 13
		;
		((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.ovals[1];
		((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 117: // STATE 14
		;
	/* 0 */	((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 118: // STATE 18
		;
		now.global_state.controlsystem.state = trpt->bup.ovals[2];
		((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 3);
		goto R999;

	case 119: // STATE 20
		;
	/* 0 */	((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 120: // STATE 24
		;
		now.global_state.controlsystem.state = trpt->bup.ovals[1];
		((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 121: // STATE 26
		;
	/* 0 */	((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 122: // STATE 29
		;
		now.global_state.controlsystem.i = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 124: // STATE 33
		;
		now.global_state.controlsystem.d = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 126: // STATE 37
		;
		now.global_state.controlsystem.previous = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 128: // STATE 42
		;
		now.global_state.controlsystem.outputvalue = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 130: // STATE 48
		;
		((P3 *)_this)->_1207_4_3_8_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_controlSignal_channel);
		;
		goto R999;
;
		;
		;
		
	case 132: // STATE 64
		goto R999;

	case 133: // STATE 67
		;
		_m = unsend(now.Controlsystem_lock);
		;
		goto R999;

	case 134: // STATE 72
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Controlsystem_configuration */
;
		;
		;
		;
		
	case 137: // STATE 3
		;
		XX = 1;
		unrecv(now.Controlsystem_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 139: // STATE 6
		;
		Controlsystem_configuration_channel_used = trpt->bup.ovals[3];
		XX = 1;
		unrecv(now.Controlsystem_configuration_channel, XX-1, 0, now.Controlsystem_configuration_signal_parameter.kp, 1);
		unrecv(now.Controlsystem_configuration_channel, XX-1, 1, now.Controlsystem_configuration_signal_parameter.ki, 0);
		unrecv(now.Controlsystem_configuration_channel, XX-1, 2, now.Controlsystem_configuration_signal_parameter.kd, 0);
		now.Controlsystem_configuration_signal_parameter.kp = trpt->bup.ovals[0];
		now.Controlsystem_configuration_signal_parameter.ki = trpt->bup.ovals[1];
		now.Controlsystem_configuration_signal_parameter.kd = trpt->bup.ovals[2];
		;
		;
		ungrab_values(trpt->bup.ovals, 4);
		goto R999;
;
		;
		
	case 141: // STATE 8
		;
		now.global_state.controlsystem.cfg.kp = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 143: // STATE 12
		;
		now.global_state.controlsystem.cfg.ki = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 145: // STATE 16
		;
		now.global_state.controlsystem.cfg.kd = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 147: // STATE 22
		;
		((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.ovals[1];
		((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 148: // STATE 23
		;
	/* 0 */	((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 149: // STATE 27
		;
		now.global_state.controlsystem.state = trpt->bup.ovals[2];
		((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 3);
		goto R999;

	case 150: // STATE 29
		;
	/* 0 */	((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 151: // STATE 33
		;
		now.global_state.controlsystem.state = trpt->bup.ovals[1];
		((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 152: // STATE 35
		;
	/* 0 */	((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 153: // STATE 38
		;
		now.global_state.controlsystem.i = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 155: // STATE 42
		;
		now.global_state.controlsystem.d = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 157: // STATE 46
		;
		now.global_state.controlsystem.previous = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 159: // STATE 51
		;
		now.global_state.controlsystem.outputvalue = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 161: // STATE 57
		;
		((P2 *)_this)->_1206_3_2_6_transition_id = trpt->bup.oval;
		_m = unsend(now.Actuator_controlSignal_channel);
		;
		goto R999;
;
		;
		;
		
	case 163: // STATE 73
		goto R999;

	case 164: // STATE 76
		;
		_m = unsend(now.Controlsystem_lock);
		;
		goto R999;

	case 165: // STATE 81
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC Actuator_controlSignal */
;
		;
		;
		;
		
	case 168: // STATE 3
		;
		XX = 1;
		unrecv(now.Actuator_lock, XX-1, 0, trpt->bup.oval, 1);
		;
		;
		goto R999;
;
		;
		
	case 170: // STATE 6
		;
		Actuator_controlsignal_channel_used = trpt->bup.ovals[1];
		XX = 1;
		unrecv(now.Actuator_controlSignal_channel, XX-1, 0, now.Actuator_controlsignal_signal_parameter, 1);
		now.Actuator_controlsignal_signal_parameter = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 172: // STATE 8
		;
		now.global_state.actuator.inputcontrol = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 174: // STATE 13
		;
		((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.ovals[1];
		((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 175: // STATE 14
		;
	/* 0 */	((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 176: // STATE 18
		;
		now.global_state.actuator.state = trpt->bup.ovals[2];
		((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 3);
		goto R999;

	case 177: // STATE 20
		;
	/* 0 */	((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 178: // STATE 23
		;
		now.global_state.actuator.setting = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 180: // STATE 29
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 181: // STATE 33
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P1 *)_this)->_1205_2_1_2_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 183: // STATE 45
		;
		now.global_state.actuator.inputcontrol = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 185: // STATE 50
		;
		((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.ovals[1];
		((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 186: // STATE 51
		;
	/* 0 */	((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 187: // STATE 55
		;
		now.global_state.actuator.state = trpt->bup.ovals[2];
		((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.ovals[1];
	/* 0 */	((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.ovals[0];
		;
		;
		ungrab_values(trpt->bup.ovals, 3);
		goto R999;

	case 188: // STATE 57
		;
	/* 0 */	((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.oval;
		;
		;
		goto R999;

	case 189: // STATE 60
		;
		now.global_state.actuator.setting = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 191: // STATE 66
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;

	case 192: // STATE 70
		;
		now.global_state.actuator.state = trpt->bup.ovals[1];
		((P1 *)_this)->_1205_2_1_4_transition_id = trpt->bup.ovals[0];
		;
		ungrab_values(trpt->bup.ovals, 2);
		goto R999;
;
		;
		;
		
	case 194: // STATE 88
		goto R999;

	case 195: // STATE 91
		;
		_m = unsend(now.Actuator_lock);
		;
		goto R999;

	case 196: // STATE 96
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC timer_manager_proc */
;
		;
		;
		;
		
	case 199: // STATE 5
		;
		now.global_state.timers.sensor.trigger.timer_enabled = trpt->bup.oval;
		_m = unsend(now.Sensor_trigger_channel);
		;
		goto R999;
;
		
	case 200: // STATE 9
		goto R999;
;
		
	case 201: // STATE 7
		goto R999;

	case 202: // STATE 14
		;
		p_restor(II);
		;
		;
		goto R999;
	}

